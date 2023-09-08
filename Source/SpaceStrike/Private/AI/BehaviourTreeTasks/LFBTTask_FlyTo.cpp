// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#include "AI/BehaviourTreeTasks/LFBTTask_FlyTo.h"
#include "CPathVolume.h"
#include "AI/LFBaseShipBotPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/LFBlueprintHelperFunctionLibrary.h"


DEFINE_LOG_CATEGORY_STATIC(LogLFAITask_FlyTo, All, All);

TMap<ECPathfindingFailReason, FString> ULFAITask_FlyTo::PathfindingFailReasonToErrorMessage =
												{
													{EndLocationUnreachable,TEXT("End Location Unreachable")},
													{WrongStartLocation,	 TEXT("Wrong Start Location")},
													{VolumeNotGenerated,    TEXT("Vomlume Not Generated")},
													{WrongEndLocation,      TEXT("Wrong End Location")},
													{VolumeNotValid,        TEXT("Volume Not Valid")},
													{Timeout,               TEXT("Timeout")},
													{Unknown,               TEXT("Unknown")},
													{None,                  TEXT("Pathfinding Success")}
												};


FLFAIFlyRequest::FLFAIFlyRequest() : 
	GoalActor(nullptr), GoalLocation(FAISystem::InvalidLocation), 
	bInitialized(false), bFlyToActor(false),
	bUsePathfinding(true), bAllowPartialPath(true), bProjectGoalOnNavigation(true),
	bReachTestIncludesAgentRadius(true), bReachTestIncludesGoalRadius(true), UserFlags(0)
{
	AcceptanceRadius = UPathFollowingComponent::DefaultAcceptanceRadius;
}

FLFAIFlyRequest::FLFAIFlyRequest(const TObjectPtr<AActor> InGoalActor) :
	GoalActor(InGoalActor), GoalLocation(FAISystem::InvalidLocation), 
	bInitialized(true), bFlyToActor(true),
	bUsePathfinding(true), bAllowPartialPath(true), bProjectGoalOnNavigation(true),
	bReachTestIncludesAgentRadius(true), bReachTestIncludesGoalRadius(true), UserFlags(0)
{
	AcceptanceRadius = UPathFollowingComponent::DefaultAcceptanceRadius;
}

FLFAIFlyRequest::FLFAIFlyRequest(const FVector& InGoalLocation) :
	GoalActor(nullptr), GoalLocation(InGoalLocation),
	bInitialized(true), bFlyToActor(false),
	bUsePathfinding(true), bAllowPartialPath(true), bProjectGoalOnNavigation(true),
	bReachTestIncludesAgentRadius(true), bReachTestIncludesGoalRadius(true), UserFlags(0)
{
	AcceptanceRadius = UPathFollowingComponent::DefaultAcceptanceRadius;
}

void FLFAIFlyRequest::SetGoalActor(const TObjectPtr<AActor> InGoalActor)
{
	if (!bInitialized)
	{
		GoalActor = InGoalActor;
		bFlyToActor = true;
		bInitialized = true;
	}
}

void FLFAIFlyRequest::SetGoalLocation(const FVector& InGoalLocation)
{
	if (!bInitialized)
	{
		GoalLocation = InGoalLocation;
		bInitialized = true;
	}
}

bool FLFAIFlyRequest::UpdateGoalLocation(const FVector& NewLocation) const
{
	if (!bFlyToActor)
	{
		GoalLocation = NewLocation;
		return true;
	}

	return false;
}

ULFAITask_FlyTo* ULFAITask_FlyTo::FlyToActor(ALFBaseShipBotPawn* InShipBotPawn, AActor* InGoalActor, bool bTrackMovingGoal, float InAcceptableRadius)
{
	ULFAITask_FlyTo* Instance = NewObject<ULFAITask_FlyTo>();

	Instance->RegisterWithGameInstance(InShipBotPawn->GetGameInstance());

	Instance->ShipBotPawn = InShipBotPawn;
	
	if(IsValid(InGoalActor))
	{
		FLFAIFlyRequest NewFlayRequest;
		
		bTrackMovingGoal ? NewFlayRequest.SetGoalActor(InGoalActor) : NewFlayRequest.SetGoalLocation(InGoalActor->GetActorLocation());

		Instance->FlayRequest = NewFlayRequest;

		Instance->SetContinuousGoalTracking(bTrackMovingGoal);

		Instance->FlayRequest.SetAcceptanceRadius(InAcceptableRadius);
	}
		
	return Instance;
}

ULFAITask_FlyTo* ULFAITask_FlyTo::FlyToLocation(ALFBaseShipBotPawn* InShipBotPawn, const FVector& GoalLocation, float InAcceptableRadius)
{
	ULFAITask_FlyTo* Instance = NewObject<ULFAITask_FlyTo>();

	Instance->RegisterWithGameInstance(InShipBotPawn->GetGameInstance());

	Instance->ShipBotPawn = InShipBotPawn;

	FLFAIFlyRequest NewFlayRequest;
	if(GoalLocation!= FVector::ZeroVector)
	{
		NewFlayRequest.SetGoalLocation(GoalLocation);
	}

	NewFlayRequest.SetAcceptanceRadius(InAcceptableRadius);
 	
	Instance->FlayRequest = NewFlayRequest;
 	
	return Instance;
}


void ULFAITask_FlyTo::Activate()
{
	Super::Activate();
	
	if(bUseContinuousTracking)
	{
		UE_LOG(LogLFAITask_FlyTo, Display,TEXT("Continuous goal tracking requested, fly to: %s"),
				FlayRequest.IsValid() && FlayRequest.IsFlyToActorRequest() ? TEXT("actor => looping of successful flights") : TEXT("location => will NOT loop"));
	}

	PerformFly();
}

void ULFAITask_FlyTo::BeginDestroy()
{
	Super::BeginDestroy();
	
	bEnableTaskTick = false;

	PathFollowingStatus = EPathFollowingStatus::Idle;

	PathFollowingResult = EPathFollowingResult::Type::Invalid;

	PathfindingStatus = EPathfindingStatus::None;
	
	RemoveFromRoot();
}

ETickableTickType ULFAITask_FlyTo::GetTickableTickType() const 
{
	return ETickableTickType::Conditional;
}

bool ULFAITask_FlyTo::IsTickable() const 
{
	return bEnableTaskTick;
}
	
TStatId ULFAITask_FlyTo::GetStatId() const 
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULFAI_FlyToAsync, STATGROUP_Tickables);
}

bool ULFAITask_FlyTo::IsTickableWhenPaused() const
{
	return false;
}

bool ULFAITask_FlyTo::IsTickableInEditor() const 
{
	return false;
}

void ULFAITask_FlyTo::Tick(float DeltaTime)
{
	if (LastTickFrame == GFrameCounter)
	{
		return;
	}

	LastTickFrame = GFrameCounter;
	
	TickFollowingPath(DeltaTime);
		
}

void ULFAITask_FlyTo::PerformFly()
{
	if(!IsValid(ShipBotPawn))
	{
		FinishFlyTask(EPathFollowingResult::Invalid);
		return;
	}

	if(!FlayRequest.IsValid())
	{
		FinishFlyTask(EPathFollowingResult::Invalid);
		return;
	}

	const TObjectPtr<ACPathVolume> NavigationVolume = ULFBlueprintHelperFunctionLibrary::GetCustomNavigationVolume3D(ShipBotPawn);
	if(!IsValid(NavigationVolume))
	{
		FinishFlyTask(EPathFollowingResult::Invalid);
		return;
	}

	CurrentGoalLocation = FlayRequest.GetDestination();
	
	FindPathAsyncTaskPtr =  UCPathAsyncFindPath::FindPathAsync( NavigationVolume, ShipBotPawn->GetActorLocation(), CurrentGoalLocation);
	if(IsValid(FindPathAsyncTaskPtr))
	{			
		FindPathAsyncTaskPtr->Success.AddDynamic(this, &ULFAITask_FlyTo::OnFindPathCompletedCallback);
		FindPathAsyncTaskPtr->Failure.AddDynamic(this, &ULFAITask_FlyTo::OnFindPathCompletedCallback);
		PathFollowingStatus = EPathFollowingStatus::Idle;
		PathfindingStatus = EPathfindingStatus::InProgress;
		
		FindPathAsyncTaskPtr->Activate();
	}
	else
	{
		FinishFlyTask(EPathFollowingResult::Invalid);
	}
}

void ULFAITask_FlyTo::TickFollowingPath(float DeltaSeconds)
{
	if(IsPathAvailable())
	{
		if(!IsGoalLocationReached())
		{
			if(PathFollowingStatus != EPathFollowingStatus::Moving)
			{
				PathFollowingStatus = EPathFollowingStatus::Moving;
			}

			const FVector CurrentLocation =  ShipBotPawn->GetActorLocation();
			const FVector CurrentPathPoint =  PathPoints[CurrentPathPointIndex].WorldLocation;

			const FRotator	RotateToTarget = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, CurrentPathPoint); 
			const FRotator InterpolatedRotator = UKismetMathLibrary::RInterpTo(ShipBotPawn->GetActorRotation(), RotateToTarget,DeltaSeconds, 10);
			ShipBotPawn->SetActorRotation(InterpolatedRotator);

			const float  CurrentSpeedForward = MoveBoostForwardScaleCurrent * ShipBotPawn->MoveBoostForwardSpeed;
			MoveBoostForwardScaleCurrent = FMath::Lerp(MoveBoostForwardScaleCurrent, 1, ShipBotPawn->MoveBoostForwardSpeedAcceleration);		
			ShipBotPawn->SetActorLocation(CurrentLocation + ShipBotPawn->GetActorForwardVector() * CurrentSpeedForward * DeltaSeconds, true);
		
			const float DistanceX2 = UKismetMathLibrary::Vector_DistanceSquared(ShipBotPawn->GetActorLocation(), CurrentPathPoint);
			const float AcceptableRadius = FlayRequest.GetAcceptanceRadius();
			if(DistanceX2 < (AcceptableRadius * AcceptableRadius))
			{
				if(CurrentPathPointIndex < (PathPoints.Num() - 1))
				{
					++CurrentPathPointIndex;
				}
				else
				{
					OnRequestFinished(EPathFollowingResult::Success);
				}
			}
		}
		else
		{
			OnRequestFinished(EPathFollowingResult::Success);
		}
	}
}

bool ULFAITask_FlyTo::IsGoalLocationReached() const
{
	bool bResult = false;
	
	if(FlayRequest.IsValid())
	{
		const float AcceptableRadius = FlayRequest.GetAcceptanceRadius();
		const FVector LocationOffset = FVector(CurrentGoalLocation.X, CurrentGoalLocation.Y - AcceptableRadius, CurrentGoalLocation.Z);
		
		const float DistanceX2 = UKismetMathLibrary::Vector_DistanceSquared(ShipBotPawn->GetActorLocation(), LocationOffset);
		
		bResult = DistanceX2 < (AcceptableRadius * AcceptableRadius);
	}

	return bResult;
}

bool ULFAITask_FlyTo::IsPathAvailable() const
{
	return PathfindingStatus == EPathfindingStatus::Success && !PathPoints.IsEmpty() && PathPoints.IsValidIndex(CurrentPathPointIndex);
}

void ULFAITask_FlyTo::OnFindPathCompletedCallback(const TArray<FCPathNode>& Path, TEnumAsByte<ECPathfindingFailReason> FailReason)
{
	if(FailReason == ECPathfindingFailReason::None)
	{
		UE_LOG(LogLFAITask_FlyTo, Display, TEXT("Pathfinding success!"));
		
		if(!bEnableTaskTick)
		{
			bEnableTaskTick = true;
		}
		
		PathPoints.Empty();	
		PathPoints.Reserve(Path.Num());	
		PathPoints.Append(Path);
		
		PathfindingStatus = EPathfindingStatus::Success;
		
		CurrentPathPointIndex = 0;
	}

	else
	{
		UE_LOG(LogLFAITask_FlyTo, Error, TEXT("Pathfinding finished with faill -> "), *PathfindingFailReasonToErrorMessage[FailReason])
		FinishFlyTask(EPathFollowingResult::Type::Invalid);
	}
}

void ULFAITask_FlyTo::FinishFlyTask(EPathFollowingResult::Type InResult)
{
	bEnableTaskTick = false;

	OnFlyFinished.Broadcast(InResult);

	FindPathAsyncTaskPtr = nullptr;
	
	SetReadyToDestroy();
}

void ULFAITask_FlyTo::OnRequestFinished(EPathFollowingResult::Type InResult)
{
	const FRotator Rotator =  FRotator(0.0f,  ShipBotPawn->GetActorRotation().Yaw, 0.0f);
	ShipBotPawn->SetActorRotation(Rotator);
	
	if(FlayRequest.IsFlyToActorRequest() && bUseContinuousTracking && InResult == EPathFollowingResult::Success)
	{
		bEnableTaskTick = false;
	
		UE_LOG(LogLFAITask_FlyTo, Log, TEXT("%s> received OnRequestFinished and goal tracking is active! Flying again in next tick"), *GetName());
		ShipBotPawn->GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULFAITask_FlyTo::PerformFly);
	}
	else
	{
		FinishFlyTask(InResult);
	}
}


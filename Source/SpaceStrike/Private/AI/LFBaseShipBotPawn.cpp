// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.
#include "AI/LFBaseShipBotPawn.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Core/LFBlueprintHelperFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseShipBotPawn, All, All);

ALFBaseShipBotPawn::ALFBaseShipBotPawn()
	:Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALFBaseShipBotPawn::BeginPlay()
{
	Super::BeginPlay();

	if(const ALFBaseShipPawn* PlayerShip = Cast<ALFBaseShipPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		CachedPlayerShipUniqueId = PlayerShip->GetUniqueID();
	}
	
	if(RadarComponent)
	{
		RadarComponent->OnShipAppeared.AddDynamic(this, &ALFBaseShipBotPawn::OnShipAppeared);
		RadarComponent->OnShipDisappeared.AddDynamic(this, &ALFBaseShipBotPawn::OnShipDisappeared);
	}
}

void ALFBaseShipBotPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALFBaseShipBotPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALFBaseShipBotPawn::OnShipAppeared(int32 ShipUniqueId)
{
	if(CachedPlayerShipUniqueId == ShipUniqueId)
	{
		if(const APawn* PlayerShip = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			CurrentShipBotState = ECurrentShipBotState::ChasingTarget;
			
		}
	}
}

void ALFBaseShipBotPawn::OnShipDisappeared(int32 ShipUniqueId)
{
	if(CachedPlayerShipUniqueId == ShipUniqueId)
	{
		CurrentShipBotState = ECurrentShipBotState::MovingToPatrolLocation;
	}
}

FRotator ALFBaseShipBotPawn::RInterpConstantToNoNormalized(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
	if (DeltaTime == 0.f || Current == Target)
	{
		return Current;
	}

	// If no interp speed, jump to target value
	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;

	const FRotator DeltaMove = (Target - Current);
	FRotator Result = Current;
	Result.Pitch += FMath::Clamp(DeltaMove.Pitch, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Yaw += FMath::Clamp(DeltaMove.Yaw, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Roll += FMath::Clamp(DeltaMove.Roll, -DeltaInterpSpeed, DeltaInterpSpeed);
	return Result;
}

FRotator ALFBaseShipBotPawn::GetLocalRotateToPosition(FVector Position)
{
	FVector TargetDirection = FVector(Position - GetActorLocation());
	FVector TargetLocalPosition = GetActorTransform().InverseTransformVector(TargetDirection);
	return TargetLocalPosition.Rotation();
}




// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Components/LFRadarComponent.h"

#include "LFBaseShipPawn.h"
#include "LFGameStateBase.h"
#include "Core/LFBlueprintHelperFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULFRadarComponent::ULFRadarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DetectionRange = 3000.0f;

	ScanningRate = 0.0f;

	RadarResetTime = 0.0f;

	DefaultRadarBehaviour = EDefaultRadarBehaviour::Disabled;
}

// Called when the game starts
void ULFRadarComponent::BeginPlay()
{
	Super::BeginPlay();

	RadarOwner = Cast<ALFBaseShipPawn>(GetOwner());

	//TheGameState->OnShipUnregistered.AddDynamic(this, &ULFRadarComponent::OnRemoveShip);

	TryAutoActivateRadar();
}

void ULFRadarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RepositoryOfShips.Empty();
}

// Called every frame
void ULFRadarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarDetectionComponent_TickCompont);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ScanShips();
}

void ULFRadarComponent::OnRemoveShip(ALFBaseShipPawn* InShip)
{
	RepositoryOfShips.Remove(InShip->GetUniqueID());
}

void ULFRadarComponent::SetScanningRate(const float NewRate)
{
	ScanningRate = NewRate;

	SetComponentTickInterval(ScanningRate);
}

void ULFRadarComponent::SetNewDetectionRange(const float NewRange)
{
	DetectionRange = NewRange;
}

float ULFRadarComponent::GetDetectionRange() const
{
	return DetectionRange;
}

void ULFRadarComponent::TryAutoActivateRadar()
{
	if (DefaultRadarBehaviour == EDefaultRadarBehaviour::AutoActivated)
	{
		ActivateRadar();
	}
	else
	{
		DeactivateRadar();
	}
}

void ULFRadarComponent::ActivateRadar()
{
	SetComponentTickInterval(ScanningRate);

	PrimaryComponentTick.bStartWithTickEnabled = true;

	SetComponentTickEnabled(true);
}

void ULFRadarComponent::DeactivateRadar()
{
	SetComponentTickEnabled(false);

	PrimaryComponentTick.bStartWithTickEnabled = false;

	RepositoryOfShips.Empty();
}

void ULFRadarComponent::ScanShips()
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarComponent_ScanShips);

	const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	
	if (!IsValid(TheGameState) || !IsValid(RadarOwner))
	{
		return;
	}
	
	const float DetectionRangeX2 = DetectionRange * DetectionRange;

	const TMap<int32, ALFBaseShipPawn*>& Ships = TheGameState->GetAllShips();
	for (const TPair<int32, ALFBaseShipPawn*>& Item : Ships)
	{
		ALFBaseShipPawn* const CheckShip = Item.Value;
		const uint32 ShipId = Item.Key;
		
		if (CheckShip && RadarOwner != CheckShip)
		{
			const float DistanceX2 = FVector::DistSquared(RadarOwner->GetActorLocation(), CheckShip->GetActorLocation());
			if (DistanceX2 < DetectionRangeX2)
			{
				if (RepositoryOfShips.Find(Item.Key))
				{
					continue;
				}
				
				RepositoryOfShips.Add(ShipId,CheckShip);
					
				//Notify about that radar find the new ship appeared.
				OnShipAppeared.Broadcast(ShipId);
			}
			else
			{
				if (RepositoryOfShips.Remove(ShipId) > 0)
				{
					OnShipDisappeared.Broadcast(ShipId);
				}
			}
		}
	}
}

int32 ULFRadarComponent::GetShipsByFactionRelationship(TArray<ALFBaseShipPawn*>& OutShips,
                                                       const ETypeOfRelationship& TypeOfRelationship) const
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarComponent_GetShipsByFactionRelationsType);

	OutShips.Empty();

	TArray<ALFBaseShipPawn*> DetectedShips;
	RepositoryOfShips.GenerateValueArray(DetectedShips);

	OutShips.Reserve(DetectedShips.Num());

	for (ALFBaseShipPawn* CheckShip : DetectedShips)
	{
		if (!IsValid(CheckShip) || !IsValid(RadarOwner))
		{
			continue;
		}

		if (ULFBlueprintHelperFunctionLibrary::IsInRelationships(RadarOwner, CheckShip->GetFactionType(),
		                                                         RadarOwner->GetFactionType(), TypeOfRelationship))
		{
			OutShips.Add(CheckShip);
		}
	}

	return OutShips.Num();
}

int32 ULFRadarComponent::GetEnemyShips(TArray<ALFBaseShipPawn*>& OutShips)
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarComponent_GetShipsByFactionRelationsType);

	OutShips.Empty();

	TArray<ALFBaseShipPawn*> DetectedShips;
	RepositoryOfShips.GenerateValueArray(DetectedShips);

	OutShips.Reserve(DetectedShips.Num());

	for (ALFBaseShipPawn* CheckShip : DetectedShips)
	{
		if (!IsValid(CheckShip) || !IsValid(RadarOwner))
		{
			continue;
		}

		if (ULFBlueprintHelperFunctionLibrary::IsInRelationships(RadarOwner, CheckShip->GetFactionType(),
		                                                         RadarOwner->GetFactionType(),
		                                                         ETypeOfRelationship::Aggressive))
		{
			OutShips.Add(CheckShip);
		}
	}

	return OutShips.Num();
}

int32 ULFRadarComponent::GetAllDetectedShips(TArray<ALFBaseShipPawn*>& OutShips) const
{
	RepositoryOfShips.GenerateValueArray(OutShips);

	return RepositoryOfShips.Num();
}

ALFBaseShipPawn* ULFRadarComponent::GetNearestEnemyShip()
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarComponent_GetNearestEnemyShip);

	if (!IsValid(RadarOwner))
	{
		return nullptr;
	}

	TObjectPtr<ALFBaseShipPawn> NearestShip = nullptr;

	TArray<ALFBaseShipPawn*> Ships;
	RepositoryOfShips.GenerateValueArray(Ships);

	const FVector RadarOwnerLocation = RadarOwner->GetActorLocation();

	float DistanceFromNearestShip = TNumericLimits<float>::Max();

	for (ALFBaseShipPawn* ShipToCheck : Ships)
	{
		if (ShipToCheck && ULFBlueprintHelperFunctionLibrary::IsInRelationships(
			this, RadarOwner->GetFactionType(), ShipToCheck->GetFactionType(), ETypeOfRelationship::Aggressive))
		{
			const float DistanceFromRadarOwnerToShip = (RadarOwnerLocation - ShipToCheck->GetActorLocation()).
				SizeSquared();

			if (DistanceFromRadarOwnerToShip < DistanceFromNearestShip)
			{
				NearestShip = ShipToCheck;
				DistanceFromNearestShip = DistanceFromRadarOwnerToShip;
			}
		}
	}

	return NearestShip;
}

float ULFRadarComponent::GetDistanceToNearestEnemy()
{
	QUICK_SCOPE_CYCLE_COUNTER(ULFRadarComponent_GetDistanceToNearestEnemy);

	const TObjectPtr<ALFBaseShipPawn> NearestEnemy = GetNearestEnemyShip();

	if (!RadarOwner || !IsValid(NearestEnemy))
	{
		return 0.0f;
	}

	return FVector::Distance(RadarOwner->GetActorLocation(), NearestEnemy->GetActorLocation());
}

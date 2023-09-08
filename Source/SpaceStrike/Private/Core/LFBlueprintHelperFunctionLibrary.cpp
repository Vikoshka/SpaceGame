// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#include "Core/LFBlueprintHelperFunctionLibrary.h"
#include "Core/FactionRelationships.h"
#include "Kismet/GameplayStatics.h"
#include "CPathfinding/Public/CPathVolume.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogLFBlueprintHelperFunctionLibrary, All, All);

void ULFBlueprintHelperFunctionLibrary::GetFactionsByRelationshipType(const UObject* WorldContextObject, const EFactionType FactionType, const ETypeOfRelationship RelationshipType, TArray<EFactionType>& OutFactions)
{
	if(!IsValid(WorldContextObject))
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("WorldContextObject nullptr"));
		return;
	}
	
	const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(WorldContextObject->GetWorld()->GetGameState());
	if(!IsValid(TheGameState))
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("TheGameState nullptr"));
		return;
	}

	if(FactionType == EFactionType::Unknown)
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("Unknown Faction Type"));		
		return;
	}
	
	const TArray<FFactionRelationship>& FactionRelationships =  TheGameState->GetFactionRelationships();				
	for(const FFactionRelationship& CurrentFactionRelationship: FactionRelationships)
	{
		if(CurrentFactionRelationship.FactionType == FactionType)
		{
			const TMap<EFactionType, ETypeOfRelationship>& Relationships = CurrentFactionRelationship.Relationships;
			for(const TPair<EFactionType, ETypeOfRelationship>& Item: Relationships)
			{
				if(Item.Value == RelationshipType)
				{
					OutFactions.Add(Item.Key);
				}
			}
				
			break;
		}
	}
}

ETypeOfRelationship ULFBlueprintHelperFunctionLibrary::GetRelationshipsBetweenTwoFactions(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2) 
{
	//If the factions are the same, then they are allies.
	if(Faction1 == Faction2)
	{
		return ETypeOfRelationship::Friendly;
	}
	
	if(IsValid(WorldContextObject))
	{
		if(const ALFGameStateBase* TheGameState = Cast<ALFGameStateBase>(WorldContextObject->GetWorld()->GetGameState()))
		{
			const TArray<FFactionRelationship>& FactionRelationships =  TheGameState->GetFactionRelationships();
			const FFactionRelationship* Result = FactionRelationships.FindByPredicate([&Faction1](const FFactionRelationship& Item)
			{
				 return Item.FactionType == Faction1;
			});

			//TODO: If the Result is invalid, the function must somehow report that the result was processed incorrectly and return an error code.
			return  Result ? Result->Relationships[Faction2] : ETypeOfRelationship::Neutral;
		}
	}

	//TODO: If we got here, it means that the function did not process the request. We need to tell the caller the reason and return the error code.
	return ETypeOfRelationship::Neutral;
}

void ULFBlueprintHelperFunctionLibrary::GetFriendlyFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions)
{
	GetFactionsByRelationshipType(WorldContextObject, FactionType, ETypeOfRelationship::Friendly, OutFactions);
}

void ULFBlueprintHelperFunctionLibrary::GetHostileFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions)
{
	GetFactionsByRelationshipType(WorldContextObject, FactionType, ETypeOfRelationship::Aggressive, OutFactions);
}

void ULFBlueprintHelperFunctionLibrary::GetNeutralFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions)
{
	GetFactionsByRelationshipType(WorldContextObject, FactionType, ETypeOfRelationship::Neutral, OutFactions);
}

void ULFBlueprintHelperFunctionLibrary::GetShipsByRelationshipType(const ALFBaseShipPawn* InShipPawnContext, TArray<ALFBaseShipPawn*>& OutShips, const ETypeOfRelationship RelationshipType)
{
	if(IsValid(InShipPawnContext))
	{
		if(const ALFGameStateBase* TheGameState = Cast<ALFGameStateBase>(InShipPawnContext->GetWorld()->GetGameState()))
		{
			const TMap<int32, ALFBaseShipPawn*>& ShipsMap =  TheGameState->GetAllShips();

			TArray<ALFBaseShipPawn*> ShipsArray;
			ShipsMap.GenerateValueArray(ShipsArray);

			for(ALFBaseShipPawn* CheckShip: ShipsArray)
			{
				if(CheckShip && CheckShip!= InShipPawnContext)
				{
					if(IsInRelationships(InShipPawnContext, CheckShip->GetFactionType(), InShipPawnContext->GetFactionType(), RelationshipType))
					{
						OutShips.Add(CheckShip);
					}
				}
			}
		}
	}
}

bool ULFBlueprintHelperFunctionLibrary::IsInRelationships(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2, const ETypeOfRelationship RelationshipType)
{
	if(!IsValid(WorldContextObject))
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("WorldContextObject nullptr"));
		return false;
	}

	const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(WorldContextObject->GetWorld()->GetGameState());
	if(!IsValid(TheGameState))
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("TheGameState nullptr"));
		return false;
	}

	if(Faction1 == EFactionType::Unknown || Faction2 == EFactionType::Unknown)
	{
		UE_LOG(LogLFBlueprintHelperFunctionLibrary, Warning, TEXT("Unknown Faction Type"))
		return false;
	}

	bool bResult = false;
	
	const TArray<FFactionRelationship>& FactionRelationships = TheGameState->GetFactionRelationships();
			
	if(const FFactionRelationship* FactionRelationship = FactionRelationships.FindByPredicate([&Faction1](const FFactionRelationship& Item)
	{
		 return Item.FactionType == Faction1;
	}))
	{
		bResult = FactionRelationship->Relationships[Faction2] == RelationshipType;
	}

	return  bResult;
}

bool ULFBlueprintHelperFunctionLibrary::IsFactionsFriendly(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2)
{
	return IsInRelationships(WorldContextObject, Faction1, Faction2,ETypeOfRelationship::Friendly);
}

int32 ULFBlueprintHelperFunctionLibrary::GetActorUniqueId(AActor* Actor)
{
	if(IsValid(Actor))
	{
		return Actor->GetUniqueID();
	}

	return -1;
}

ALFBaseShipPawn* ULFBlueprintHelperFunctionLibrary::FindShipByUniqueId(const UObject* WorldContextObject, int32 ShipUniqueId)
{
	TObjectPtr<ALFBaseShipPawn>  OutShip = nullptr;

	const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(UGameplayStatics::GetGameState(WorldContextObject));
	if(IsValid(TheGameState))
	{
		OutShip = TheGameState->GetAllShips()[ShipUniqueId];
	}

	return OutShip;
}

ACPathVolume* ULFBlueprintHelperFunctionLibrary::GetCustomNavigationVolume3D(const UObject* WorldContextObject)
{
	return TObjectPtr<ACPathVolume>(Cast<ACPathVolume>(UGameplayStatics::GetActorOfClass(WorldContextObject, ACPathVolume::StaticClass())));
}

bool ULFBlueprintHelperFunctionLibrary::IsHasLineSightBetweenLocations(const UObject* WorldContextObject, const FVector& StartLocation, const FVector& DestinationLocation, const TArray<AActor*>& ActorsToIgnore, bool bDrawTraceLine) 
{
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(StartLocation, DestinationLocation));
	const float DistanceBetweenPoints = FVector::Distance(StartLocation, DestinationLocation);
	const FVector EndLocation =  StartLocation + (ForwardVector * DistanceBetweenPoints);
	
	FHitResult HitResult;
	return !(UKismetSystemLibrary::LineTraceSingle(WorldContextObject, StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore, EDrawDebugTrace::Persistent, HitResult, false, FLinearColor::Red,  FLinearColor::Green, bDrawTraceLine ? 0 : 5.0));
}

bool ULFBlueprintHelperFunctionLibrary::IsNavigationVolumeGenerated(const UObject* WorldContextObject)
{
	const TObjectPtr<ACPathVolume> CustomNavMesh = GetCustomNavigationVolume3D(WorldContextObject);
	
	return IsValid(CustomNavMesh) ? CustomNavMesh->InitialGenerationCompleteAtom.load() : false;
}



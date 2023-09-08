// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LFBaseShipPawn.h"
#include "Core/FactionRelationships.h"
#include "GameFramework/GameStateBase.h"
#include "LFGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFactionRelationsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipRegistered, ALFBaseShipPawn*, Ship);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipUnregistered, ALFBaseShipPawn*, Ship);

struct FFactionRelationship;

UCLASS()
class SPACESTRIKE_API ALFGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALFGameStateBase();

	//Delegate to report that the faction's global relationship has changed.
	UPROPERTY(BlueprintAssignable)
	FOnFactionRelationsChanged OnFactionRelationsChanged;

	//Delegate to report that the ship has spawned on the level and added to the general pool of ships.
	UPROPERTY(BlueprintAssignable)
	FOnShipRegistered OnShipRegistered;
	
	//Delegate to report that the ship has left the level and has been removed from the general pool of ships.
	UPROPERTY(BlueprintAssignable)
	FOnShipUnregistered OnShipUnregistered;
	
	void RegisterShip(TObjectPtr<ALFBaseShipPawn> InShip);
	void UnregisterShip(TObjectPtr<ALFBaseShipPawn> InShip);

	//Returns all ships present in the game.
	UFUNCTION(BlueprintPure)
	const TMap<int32, ALFBaseShipPawn*>& GetAllShips() const;

	//Returns global relationships between factions .
	const TArray<FFactionRelationship>& GetFactionRelationships() const;
	
protected:
	
	virtual void BeginPlay() override;

	//Stores all the ships that are currently in the game at the level.
	UPROPERTY()
	TMap<int32, ALFBaseShipPawn*> ShipPawns;
	
	//Stores global relationships between factions (Something like the relationship of groups in the game S.T.A.L.K.E.R
	///@Note: Faction type should be unique.
	UPROPERTY(EditAnywhere)
	TArray<FFactionRelationship> FactionRelationships;
};

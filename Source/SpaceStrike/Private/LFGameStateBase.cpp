// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "LFGameStateBase.h"

#include "EngineUtils.h"
#include "LFBaseShipPawn.h"

ALFGameStateBase::ALFGameStateBase()
{
}

void ALFGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALFGameStateBase::RegisterShip(TObjectPtr<ALFBaseShipPawn> InShip)
{
	ShipPawns.Add(InShip->GetUniqueID(),InShip);

	OnShipRegistered.Broadcast(InShip);
}

void ALFGameStateBase::UnregisterShip(TObjectPtr<ALFBaseShipPawn> InShip)
{
	ShipPawns.Remove(InShip->GetUniqueID());

	OnShipUnregistered.Broadcast(InShip);
}

const TMap<int32, ALFBaseShipPawn*>& ALFGameStateBase:: GetAllShips() const
{
	return  ShipPawns;
}

const TArray<FFactionRelationship>& ALFGameStateBase::GetFactionRelationships() const
{
	return  FactionRelationships;
}




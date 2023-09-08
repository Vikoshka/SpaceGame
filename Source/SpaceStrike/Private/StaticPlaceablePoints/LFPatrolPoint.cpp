// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "StaticPlaceablePoints/LFPatrolPoint.h"

ALFPlaceablePoint::ALFPlaceablePoint()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

ALFPlaceablePoint::ALFPlaceablePoint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ALFPlaceablePoint::BeginPlay()
{
	Super::BeginPlay();
}

void ALFPlaceablePoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALFPlaceablePoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

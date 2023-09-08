// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "LFPlaceablePoint.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SPACESTRIKE_API ALFPlaceablePoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ALFPlaceablePoint();

	explicit ALFPlaceablePoint(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaSeconds) override;
};

// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaticPlaceablePoints/LFPlaceablePoint.h"
#include "LFPointOfInterest.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTRIKE_API ALFPointOfInterest : public ALFPlaceablePoint
{
	GENERATED_BODY()

protected:

	bool bIsVisited = false;

public:

	UFUNCTION(BlueprintCallable)
	void MarkAsVisited(bool bVisited) { bIsVisited = bVisited; }

	UFUNCTION(BlueprintPure)
	bool IsVisited() const { return bIsVisited; }
};

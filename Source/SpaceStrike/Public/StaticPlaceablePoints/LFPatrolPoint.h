// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaticPlaceablePoints/LFPlaceablePoint.h"
#include "LFPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SPACESTRIKE_API ALFPatrolPoint : public ALFPlaceablePoint
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0"))
	uint32 IndexInPatrolPath = 0;

	//Belonging to the path of patrol.
	UPROPERTY(EditAnywhere)
	FString PatrolPathTag = TEXT("None");
	
	bool bIsPassed = false;

public:
	
	UFUNCTION(BlueprintCallable)
	void MarkAsPassed(bool bPassed) { bIsPassed = bPassed; }
	
	UFUNCTION(BlueprintPure)
	bool IsWasPassed() const { return bIsPassed; }
	
	UFUNCTION(BlueprintPure)
	int32 GetIndexInPatrolPath() const  { return IndexInPatrolPath; }
};

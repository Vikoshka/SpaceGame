// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LFCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTRIKE_API ULFCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement",
				meta=(ClampMin = "1.5", ClampMax = "10.0"))
	float BoostSpeedModifier = 2.0f;
	
	
	virtual float GetMaxSpeed() const override;
	
};

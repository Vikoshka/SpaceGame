// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LFPlayerShipWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTRIKE_API ULFPlayerShipWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	float GetHealth() const;
	
};

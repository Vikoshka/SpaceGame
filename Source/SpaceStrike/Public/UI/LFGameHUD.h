// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "LFGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACESTRIKE_API ALFGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};

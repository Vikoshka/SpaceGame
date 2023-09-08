// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#include "UI/LFGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void ALFGameHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrossHair();
}

void ALFGameHUD::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void ALFGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LinearColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize,Center.Max,Center.Min + HalfLineSize,Center.Max,LinearColor, LineThickness);
	DrawLine(Center.Min,Center.Max - HalfLineSize,Center.Min, Center.Max + HalfLineSize,LinearColor, LineThickness);
}

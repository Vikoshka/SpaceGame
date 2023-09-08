// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LFPlayerShipPawn.h"
#include "GameFramework/PlayerController.h"
#include "LFPlayerController.generated.h"

UCLASS()
class SPACESTRIKE_API ALFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnMoveUp(float Value);

	void OnTurnRollRight(float Value);
	void OnTurnPitchUp(float Value);
	void OnTurnYawRight(float Value);
	
	void OnTurnPitchUpCamera(float Value);
	void OnTurnYawRightCamera(float Value);

	void OnStartBoostSpeedForward();
	void OnStopBoostSpeedForward();
	
	FVector2f GetInputMousePosition2D() const {return InputMouse;}

	void OnSwapControl();

protected:
	FVector2f ScreenPosition();
	void TargetInTheDirectionOfTheCameraAndMouse();

private:

	TObjectPtr<ALFPlayerShipPawn> PlayerShip;
	
	int ViewportSizeX, ViewportSizeY;
	float RadiusForMaximumTurningForce;
	float MousePositionSizeX, MousePositionSizeY;
	FVector2f InputMouse;

	FHitResult ResultMousePosition;
	FHitResult ResultCameraPosition;
	FVector MousePosition, MouseDirection;

	FVector GetWorldPointByMouseDirection() const
	{
		return MousePosition + MouseDirection * PlayerShip->MaxDistanceTargetMouse;
	}

	FVector GetWorldPointByCameraDirection() const
	{
		return CameraLocation + CameraRotation.Vector() * PlayerShip->MaxDistanceTargetMouse;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
};

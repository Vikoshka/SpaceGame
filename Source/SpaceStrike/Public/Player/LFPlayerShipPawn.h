// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/LFBaseShipPawn.h"
#include "LFPlayerShipPawn.generated.h"

class ALFBaseShipPawn;
class UCameraComponent;
class USpringArmComponent;

UCLASS(BlueprintType)
class SPACESTRIKE_API ALFPlayerShipPawn : public ALFBaseShipPawn
{
	GENERATED_BODY()

public:
	ALFPlayerShipPawn();
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	/**
	* @brief Block of functions and parameters for the movement of the ship
	* @param Movement and Rotation
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Debug Info",
		DisplayName = "Show Mouse Debug info")
	bool DebugInfoMouse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Debug Info",
		DisplayName = "Show Movement Debug info")
	bool DebugInfoMovement = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Mouse | MouseTarget | Distance",
		DisplayName = "Max Target Distance",
		meta=(ClampMin = 0.0f, ClampMax = 1000000.0f))
	float MaxDistanceTargetMouse = 20000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
	Category="Seting Ship | Movement and Rotation Speed | Rotation | Radius Input Mouse",
	DisplayName = "Radius Scale Input Mouse",
	meta=(ClampMin = 1.0f, ClampMax = 10000.0f))
	float RadiusScaleInputMouse = 5.0f;
	
	/**
	* @brief Block of functions and parameters for the movement of the ship
	* @param Movement and Rotation
	*/

	void MoveForward(float Scale);
	void MoveRight(float Scale);
	void MoveUp(float Scale);

	void StartBoostSpeedForward();
	void StopBoostSpeedForward();

	void TurnRollRight(float Scale);
	void TurnPitchUp(float Scale);
	void TurnYawRight(float Scale);
	void TurnPitchUpCamera(float Scale);
	void TurnYawRightCamera(float Scale);

	void SwapControl();
	bool bSwapControl = true;
	bool GetSwapControl() const {return bSwapControl;}
	
protected:
	FRotator MovementCurrentRotation;
	FRotator CameraCurrentRotation;
	
private:
	void MoveForwardShip(float DeltaTime);
	float MoveForwardScaleTarget = 0.0f;
	float MoveForwardScaleCurrent = 0.0f;
	float ForwardScaleTarget = 0.0f;
	float MoveBoostForwardScaleCurrent = 0.0f;
	bool bWantsToSpeedUp = false;
	bool bIsBoostSpeedForward = false;

	void MoveRightShip(float DeltaTime);
	float MoveRightScaleTarget = 0.0f;
	float MoveRightScaleCurrent = 0.0f;

	void MoveUpShip(float DeltaTime);
	float MoveUpScaleTarget = 0.0f;
	float MoveUpScaleCurrent = 0.0f;

	float TurnRollRightScaleTarget = 0.0f;
	float TurnRollRightScaleCurrent = 0.0f;

	float TurnPitchUpScaleTarget = 0.0f;
	float TurnPitchUpScaleCurrent = 0.0f;

	float TurnYawRightScaleTarget = 0.0f;
	float TurnYawRightScaleCurrent = 0.0f;

///////////////////////////////////////////////////////////

	void ChangeOfControl(float DeltaTime);
};

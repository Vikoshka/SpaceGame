// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LFRadarComponent.h"
#include "Core/LFEnums.h"
#include "GameFramework/Pawn.h"
#include "LFBaseShipPawn.generated.h"

class UBoxComponent;
class ULFHealthComponent;
class UTextRenderComponent;
class ULFEulerQuaternion;

UCLASS(Blueprintable,BlueprintType)
class SPACESTRIKE_API ALFBaseShipPawn : public APawn
{
	GENERATED_BODY()

public:	
	 ALFBaseShipPawn();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<ULFHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTextRenderComponent> HealthTextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<ULFRadarComponent> RadarComponent;
	
	UPROPERTY(EditAnywhere, Category = "Setting Ship | Type Of Ship")
	ETypeOfShip TypeOfShip = ETypeOfShip::Unknown;

	//Which faction does this ship belong to?
	UPROPERTY(EditAnywhere, Category = "Setting Ship | Faction Owner")
	EFactionType OwnerShip = EFactionType::Unknown;

	TObjectPtr<AController> OwnerShipController;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	
public:
	
	UFUNCTION(BlueprintPure)
	const EFactionType& GetFactionType() const;

	UFUNCTION(BlueprintPure, Category = "Setting Ship | Radar")
	const ULFRadarComponent* GetRadarComponent() const;
	
	/**
	* @brief Block of functions and parameters for the movement of the ship
	* @param Movement and Rotation
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Forward",
		DisplayName = "Boost Forward Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float MoveBoostForwardSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Forward",
		DisplayName = "Boost Forward Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float MoveBoostForwardSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Forward",
		DisplayName = "Forward Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float MoveForwardSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Forward",
		DisplayName = "Forward Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float MoveForwardSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Right",
		DisplayName = "Right Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float MoveRightSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Right",
		DisplayName = "Right Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float MoveRightSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Up",
		DisplayName = "Up Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float MoveUpSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Movement | Up",
		DisplayName = "Up Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float MoveUpSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Roll",
		DisplayName = "Turn Roll Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float TurnRollRightSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Roll",
		DisplayName = "Turn Roll Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float TurnRollRightSpeed = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Pitch",
		DisplayName = "Turn Pitch Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float TurnPitchUpSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Pitch",
		DisplayName = "Turn Pitch Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float TurnPitchUpSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Yaw",
		DisplayName = "Turn Yaw Acceleration",
		meta=(ClampMin = 0.0f, ClampMax = 10.0f))
	float TurnYawRightSpeedAcceleration = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
		Category="Setting Ship | Movement and Rotation Speed | Rotation | Yaw",
		DisplayName = "Turn Yaw Speed",
		meta=(ClampMin = 0.0f, ClampMax = 10000.0f))
	float TurnYawRightSpeed = 20.0f;

protected:
	void OnDeath();
	bool IsDead = false;
	void OnHealthChanged(float Health);
};

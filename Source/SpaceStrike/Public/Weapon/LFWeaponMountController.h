// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LFWeaponMount.h"
#include "LFWeaponMountController.generated.h"

UENUM()
enum class EWeaponMountControlMode : uint8
{
	Disabled = 0,
	Manual = 1,
	Target = 2,
	Auto = 3,
	PointDefense = 4
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESTRIKE_API ULFWeaponMountController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULFWeaponMountController();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Initialize();

	UFUNCTION(BlueprintCallable)
	void SwitchControlMode(EWeaponMountControlMode Mode);

	// For testing pusposes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TestTarget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	EWeaponMountControlMode TurretControlMode;
	
	TObjectPtr<ALFWeaponMount> Turret;
	
	bool bIsInitialized;

	// TODO: Realization of Radar Class and Interfaces

	// TODO: Implementation of Pilot Class which represents of NPC or Player

	// Calculate predictive target position. Calculation depends on weapon projectile speed, if it exist, target speed and distance.
	FVector GetPredictivePositionOfActor(TObjectPtr<AActor> Target);

	TArray<TObjectPtr<AActor>> TargetPool;

	
};

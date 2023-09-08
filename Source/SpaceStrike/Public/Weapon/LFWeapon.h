// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/LFProjectile.h"
#include "LFWeapon.generated.h"

UENUM()
enum class EWeaponSize : uint8
{
	S,
	M,
	L,
	XL
};

UCLASS()
class SPACESTRIKE_API ALFWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALFWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TObjectPtr<UArrowComponent> ShootingPoint;
	
	TArray<TObjectPtr<ALFProjectile>> Pool;

	FTimerHandle TimeToNextShoot;
	FTimerHandle TimeToReload;
	float DPS;
	float WeaponRange;
	bool IsInitialized;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Initialize();
	
	UFUNCTION(BlueprintCallable)
	bool IsCanShoot();

	UFUNCTION(BlueprintCallable)
	float GetDPS();

	UFUNCTION(BlueprintCallable)
	float GetWeaponRange();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	bool LoadProjectileAmmo(TSubclassOf<ALFProjectile> ProjectileClass);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PoolSettings")
	TSubclassOf<ALFProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Names")
	FString WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Names")
	FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Names")
	FText WeaponShortDesc;

	UPROPERTY(EditAnywhere, Category = "Names")
	FText WeaponDesc;

	// Time in seconds between shooting projectiles
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float RateOfFire;

	// Angle in degrees chosen randomly in additions to shooting direction
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Spread;

	// Time in seconds to cool weapon after overheating
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float OverheatCooldownTime;

	// Time in seconds to fully cool weapon
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float CoolingTime;

	// Amount ammo in magazine
	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 AmmoMax;

	// Time in seconds to refill magazine
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float ReloadTime;

	// Weapon size, using to install weapon in different slots
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	EWeaponSize WeaponSize;
	
};

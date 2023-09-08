// Source Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LFBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULFHealthComponent;
class UTextRenderComponent;
class USGWeaponComponent;

UCLASS()
class SPACESTRIKE_API ALFBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALFBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULFHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//USGWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandDamageVelocity = FVector2D(600.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandDamage = FVector2D(10.0f,100.0f);

	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMovementDirection() const;

private:
	bool IsMovingForward = false;
	bool WantsToRun = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void OnStartRunning();
	void OnStopRunning();

	void OnDeath();
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

};

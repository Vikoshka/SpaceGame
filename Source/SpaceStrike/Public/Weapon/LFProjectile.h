// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include <ThirdParty/hlslcc/hlslcc/src/hlslcc_lib/compiler.h>

#include "Niagara/Public/NiagaraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LFProjectile.generated.h"

UCLASS()
class SPACESTRIKE_API ALFProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALFProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Initialize();

	bool IsActivated();

	UFUNCTION(BlueprintCallable)
	float GetRange() {return Lifetime * Speed;}
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot();

	void Shoot(FVector Position = FVector(), FVector Direction = FVector());

	UFUNCTION(BlueprintImplementableEvent)
	void OnDisable();

	void Disable();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bIsActivated;
	FVector AimDirection;
	FTimerHandle DisableTimer;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> PMComponent;

	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Speed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Lifetime;
};

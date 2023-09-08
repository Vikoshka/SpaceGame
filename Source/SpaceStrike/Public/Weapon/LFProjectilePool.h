// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LFProjectile.h"
#include "LFProjectilePool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESTRIKE_API ULFProjectilePool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULFProjectilePool();

	void Initialize(size_t PoolSize = 1);

	UFUNCTION(BlueprintCallable)
	void Shoot(FVector Position, FVector Direction);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<TObjectPtr<ALFProjectile>> Pool;
	bool isInitialized;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PoolSettings")
	TSubclassOf<ALFProjectile> ProjectileClass;
};

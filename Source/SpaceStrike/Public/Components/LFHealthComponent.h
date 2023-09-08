// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LFHealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPACESTRIKE_API ULFHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULFHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const {return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const {return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const {return Health;}
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health",
		meta=(ClampMin = "0.0", ClampMax = "100000.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(EditCondition = "AutoHeal"))
	float HealUpdateTime = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(EditCondition = "AutoHeal"))
	float HealDelay = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(EditCondition = "AutoHeal"))
	float HealModifier = 5.0f;

	
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);

	
};

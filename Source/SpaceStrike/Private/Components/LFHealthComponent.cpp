// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Components/LFHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

// Sets default values for this component's properties
ULFHealthComponent::ULFHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	
	SetHealth(MaxHealth);

	const TObjectPtr<AActor> ComponentOwner = GetOwner();
	if (IsValid(ComponentOwner))
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ULFHealthComponent::OnTakeAnyDamage);
	}
}

void ULFHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                         AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this,
			&ULFHealthComponent::HealUpdate,HealUpdateTime, true, HealDelay);
	}
}

void ULFHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual( Health , MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void ULFHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f , MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

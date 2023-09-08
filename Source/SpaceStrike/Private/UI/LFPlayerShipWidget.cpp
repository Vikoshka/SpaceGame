// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "UI/LFPlayerShipWidget.h"

#include "LFHealthComponent.h"

float ULFPlayerShipWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(ULFHealthComponent::StaticClass());
	const auto HealthComponent = Cast<ULFHealthComponent>(Component);
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

float ULFPlayerShipWidget::GetMaxHealth() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(ULFHealthComponent::StaticClass());
	const auto HealthComponent = Cast<ULFHealthComponent>(Component);
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetMaxHealth();
}

float ULFPlayerShipWidget::GetHealth() const
{
	const auto Player = GetOwningPlayerPawn();
	if(!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(ULFHealthComponent::StaticClass());
	const auto HealthComponent = Cast<ULFHealthComponent>(Component);
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealth();
}
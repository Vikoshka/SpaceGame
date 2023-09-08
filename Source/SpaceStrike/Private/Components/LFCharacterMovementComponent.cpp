// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Components/LFCharacterMovementComponent.h"
#include "Player/LFBaseCharacter.h"

float ULFCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ALFBaseCharacter* Player = Cast<ALFBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * BoostSpeedModifier: MaxSpeed;
}

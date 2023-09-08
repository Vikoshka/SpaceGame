// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Weapon/LFProjectilePool.h"

// Sets default values for this component's properties
ULFProjectilePool::ULFProjectilePool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	isInitialized = false;
	// ...
}

void ULFProjectilePool::Initialize(size_t PoolSize)
{
	if (ProjectileClass)
	{
		ALFWeapon* Weapon = Cast<ALFWeapon>(GetOwner());
		if (Weapon)
		{
			auto ProjectileCast = Cast<ALFProjectile>(ProjectileClass->GetDefaultObject());
			if (ProjectileCast)
			{
				PoolSize = FMath::CeilToFloat(Weapon->RateOfFire * ProjectileCast->Lifetime);
				
			}
		}
		else
		{
			return;
		}

		for (auto i = 0; i < PoolSize; i++)
		{
			auto p = Cast<ALFProjectile>(GetWorld()->SpawnActor<ALFProjectile>(ProjectileClass, FVector(), FRotator()));
			if (p)
			{
				Pool.Add(p);
				p->Initialize();
			}
		}
	}
	else
	{
		return;
	}
	isInitialized = true;
}

void ULFProjectilePool::Shoot(FVector Position, FVector Direction)
{
	if (isInitialized)
	{
		for (auto& Projectile : Pool)
		{
			if (!Projectile->IsActivated())
			{
				Projectile->Shoot(Position, Direction);
				break;
			}
		}
	}
}


// Called when the game starts
void ULFProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	// ...
	
}


// Called every frame
void ULFProjectilePool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


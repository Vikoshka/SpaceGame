// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Weapon/LFWeapon.h"

#include "../../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/spdlog/include/spdlog/fmt/bundled/format.h"
#include "Components/ArrowComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
ALFWeapon::ALFWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponSize = EWeaponSize::S;
	
	IsInitialized = false;

	if (!ShootingPoint)
	{
		ShootingPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootingPoint"));
		if (!ShootingPoint) return;
	}
}

// Called when the game starts or when spawned
void ALFWeapon::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

// Called every frame
void ALFWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	
}

void ALFWeapon::Initialize()
{
	if (ProjectileClass)
	{
		size_t PoolSize = 1;
		if (auto ProjectileCast = Cast<ALFProjectile>(ProjectileClass->GetDefaultObject()))
		{
			PoolSize = FMath::CeilToFloat(RateOfFire * ProjectileCast->Lifetime);
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
	IsInitialized = true;
}

bool ALFWeapon::IsCanShoot()
{
	return TimeToNextShoot == 0;
}

float ALFWeapon::GetDPS()
{
	return 0;
}

float ALFWeapon::GetWeaponRange()
{
	return 0;
}

void ALFWeapon::Shoot()
{
	if (IsInitialized)
	{
		if (TimeToNextShoot <= 0)
		{
			for (auto& Projectile : Pool)
			{
				if (!Projectile->IsActivated())
				{
					float SpreadRad = FMath::DegreesToRadians(Spread);
					FVector SpreadDirection = FMath::VRandCone(ShootingPoint->GetForwardVector() * Projectile->Speed, SpreadRad, SpreadRad);
					Projectile->Shoot(ShootingPoint->GetComponentLocation(), SpreadDirection);
					GetWorldTimerManager().SetTimer(TimeToNextShoot, this, RateOfFire, false);
					
					break;
				}
			}
		}
	}
}

bool ALFWeapon::LoadProjectileAmmo(TSubclassOf<ALFProjectile> ProjectileClass)
{
	
}


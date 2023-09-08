// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Weapon/LFProjectile.h"

#include "LFBaseCharacter.h"
#include "Physics/PhysicsFiltering.h"
#include "SlateCore/Private/Application/ActiveTimerHandle.h"

// Sets default values
ALFProjectile::ALFProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 1500.0f;
	Damage = 1.0f;
	Lifetime = 5.0f;
	
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!PMComponent)
	{
		PMComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	}

	bIsActivated = false;
}

bool ALFProjectile::Initialize()
{
	if (PMComponent)
	{
		PMComponent->ProjectileGravityScale = 0.0f;
		PMComponent->InitialSpeed = 0;
		PMComponent->MaxSpeed = Speed;
		PMComponent->bRotationFollowsVelocity = true;
	}

	SetActorHiddenInGame(true);
	bIsActivated = false;

	return true;
}

bool ALFProjectile::IsActivated()
{
	return bIsActivated;
}

void ALFProjectile::Disable()
{
	PMComponent->Velocity = FVector();
	bIsActivated = false;
	
	SetActorHiddenInGame(true);

	// EVENT
	OnDisable();
}

// Called when the game starts or when spawned
void ALFProjectile::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	
	Shoot();
}


void ALFProjectile::Shoot(FVector Position, FVector Direction)
{
	if (Position == FVector()) Position = GetActorLocation();
	if (Direction == FVector()) Position = GetActorRotation().Vector();
	if (PMComponent)
	{
		PMComponent->Velocity = Direction * Speed;
		SetActorLocation(Position);
	}
	
	GetWorld()->GetTimerManager().SetTimer(DisableTimer, this, &ALFProjectile::Disable, Lifetime, false);
	
	SetActorHiddenInGame(false);
	bIsActivated = true;

	// EVENT
	OnShoot();
}

// Called every frame
void ALFProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)
	{
		FHitResult Hit;
		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = GetActorLocation() + PMComponent->Velocity * DeltaTime;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 0.1f, 0, 10.0f);

		if (Hit.bBlockingHit && IsValid(Hit.GetActor())){
			Disable();
			
			UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());

			// EVENT
			OnHit();
		}
	}
}



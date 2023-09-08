// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Weapon/LFWeaponMountController.h"

// Sets default values for this component's properties
ULFWeaponMountController::ULFWeaponMountController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	TurretControlMode = EWeaponMountControlMode::Disabled;
	
	bIsInitialized = false;
}


// Called when the game starts
void ULFWeaponMountController::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


// Called every frame
void ULFWeaponMountController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		if (bIsInitialized) {

		switch (TurretControlMode)
		{
		case EWeaponMountControlMode::Disabled:
		{
			if (Turret->GetState() != EWeaponMountState::Idle)
			{
				Turret->PullIn();
			}
			break;
		}
		case EWeaponMountControlMode::Manual:
		{
				
				// TODO: Realize manual control of turrets
				
			break;
		}
		case EWeaponMountControlMode::Target:
		{
			//TObjectPtr<AActor> Target = Radar->GetCurrentTarget();
			// For testing
			TObjectPtr<AActor> Target = TestTarget;
			if (IsValid(Target))
			{
				Turret->TurnToPos(GetPredictivePositionOfActor(Target));
			}

			if (Turret->GetState() == EWeaponMountState::ReadyToShoot)
			{
				/*if (IWeapon* IWeaponInstance = Cast<IWeapon>(Turret->GetWeapon()))
				{
					IWeaponInstance->Shoot();
				}*/
			}

			break;
		}
		case EWeaponMountControlMode::Auto:
		{
			//TargetPool = Radar->GetEnemyPool();

			// Find best target to rotate
			TObjectPtr<AActor> BestTarget = nullptr;
			float BestAngleSquare = 180;
			for (size_t i = 0; i < TargetPool.Num(); i++)
			{
				TObjectPtr<AActor> Target = TargetPool[i];
				FRotator RotatorToTarget = Turret->GetLocalRotateToPosition(Target->GetTargetLocation());
				if (Turret->IsCanRotateTo(RotatorToTarget))
				{
					float AngleSquare = FMath::Sqrt(FMath::Square(RotatorToTarget.Pitch) + FMath::Square(RotatorToTarget.Yaw));
					if (AngleSquare < BestAngleSquare)
					{
						BestAngleSquare = AngleSquare;
						BestTarget = Target;
					}
				}
			}
			if (BestTarget != nullptr)
			{
				Turret->TurnToPos(GetPredictivePositionOfActor(BestTarget));
			}

			if (Turret->GetState() == EWeaponMountState::ReadyToShoot)
			{
				/*if (IWeapon* IWeaponInstance = Cast<IWeapon>(Turret->GetWeapon()))
				{
					IWeaponInstance->Shoot();
				}*/
			}

			break;
		}
		case EWeaponMountControlMode::PointDefense:
		{
			//TargetPool = Radar->GetEnemyMissilePool();

			// Find best target to rotate
			TObjectPtr<AActor> BestTarget = nullptr;
			float BestAngleSquare = 180 * 180;
			for (size_t i = 0; i < TargetPool.Num(); i++)
			{
				TObjectPtr<AActor> Target = TargetPool[i];
				FRotator RotatorToTarget = Turret->GetLocalRotateToPosition(Target->GetTargetLocation());
				if (Turret->IsCanRotateTo(RotatorToTarget))
				{
					float AngleSquare = FMath::Square(RotatorToTarget.Pitch) + FMath::Square(RotatorToTarget.Yaw);
					if (AngleSquare < BestAngleSquare)
					{
						BestAngleSquare = AngleSquare;
						BestTarget = Target;
					}
				}
			}
			if (BestTarget != nullptr)
			{
				Turret->TurnToPos(GetPredictivePositionOfActor(BestTarget));
			}

			if (Turret->GetState() == EWeaponMountState::ReadyToShoot)
			{
				/*if (IWeapon* IWeaponInstance = Cast<IWeapon>(Turret->GetWeapon()))
				{
					IWeaponInstance->Shoot();
				}*/
			}

			break;
		}
		}
	}
}

bool ULFWeaponMountController::Initialize()
{
	bIsInitialized = false;

	// Getting turret actor
	this->Turret = Cast<ALFWeaponMount>(this->GetOwner());
	if (!IsValid(this->Turret)) return false;

	// TODO: Delete later, For Testing
	if (IsValid(TestTarget)) TurretControlMode = EWeaponMountControlMode::Target;
	
	bIsInitialized = true;
	return bIsInitialized;
}

void ULFWeaponMountController::SwitchControlMode(EWeaponMountControlMode Mode)
{
	TurretControlMode = Mode;
}

FVector ULFWeaponMountController::GetPredictivePositionOfActor(TObjectPtr<AActor> Target)
{
	if (bIsInitialized) {
		float Distance = (Target->GetActorLocation() - Turret->GetWorldLocation()).Length();

		/*
		if (auto WeaponInstance = Cast<IWeaponProjectile>(Turret->GetWeapon()))
		{
			return Target->GetActorLocation() + Target->GetVelocity() * Distance / WeaponInstance->GetProjectileSpeed();
		}
		*/
		return Target->GetActorLocation();
	}

	return FVector();
}


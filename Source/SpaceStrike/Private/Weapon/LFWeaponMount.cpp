// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Weapon/LFWeaponMount.h"

#include "../../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/clsocket/src/ActiveSocket.h"

// Sets default values for this component's properties
ALFWeaponMount::ALFWeaponMount()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponMount"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Root);

	MountMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MountMesh"));
	MountMesh->SetupAttachment(BaseMesh);
	
	bIsActivated = false;
	bIsOnline = false;
	TurnSpeed = 60.0f;
	ElevationSpeed = 40.0f;
	MinAngles = FRotator(-10, -180, 0);
	MaxAngles = FRotator(90, 180, 0);
}


// Called when the game starts
void ALFWeaponMount::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FRotator ALFWeaponMount::RInterpConstantToNoNormalized(const FRotator& Current, const FRotator& Target, float DeltaTime,
	float InterpSpeed)
{
	// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
	if (DeltaTime == 0.f || Current == Target)
	{
		return Current;
	}

	// If no interp speed, jump to target value
	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;

	const FRotator DeltaMove = (Target - Current);
	FRotator Result = Current;
	Result.Pitch += FMath::Clamp(DeltaMove.Pitch, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Yaw += FMath::Clamp(DeltaMove.Yaw, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Roll += FMath::Clamp(DeltaMove.Roll, -DeltaInterpSpeed, DeltaInterpSpeed);
	return Result;
}


// Called every frame
void ALFWeaponMount::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurretState = EWeaponMountState::Disabled;

	if (bIsOnline)
	{
		if (bIsActivated)
		{
			TurretState = EWeaponMountState::Idle;
			
			if (TargetPosition != FVector(0, 0, 0))
			{
				TurretState = EWeaponMountState::Tracking;
				
				FRotator targetRotator = GetLocalRotateToPosition(TargetPosition);

				// Clamping rotation angles
				FRotator BaseMeshTargetRotator = FRotator(0, FMath::Clamp(targetRotator.Yaw, MinAngles.Yaw, MaxAngles.Yaw), 0);
				FRotator MountMeshTargetRotator = FRotator(FMath::Clamp(targetRotator.Pitch, MinAngles.Pitch, MaxAngles.Pitch), 0, 0);

				// Checking if turret have obstacles by clamping rotation angles, use custom function
				if (FMath::Abs(MinAngles.Yaw) + FMath::Abs(MaxAngles.Yaw) >= 360)
				{
					BaseMesh->SetRelativeRotation(FMath::RInterpConstantTo(BaseMesh->GetRelativeRotation(), BaseMeshTargetRotator, DeltaTime, TurnSpeed));
					MountMesh->SetRelativeRotation(FMath::RInterpConstantTo(MountMesh->GetRelativeRotation(), MountMeshTargetRotator, DeltaTime, ElevationSpeed));
				}
				else
				{
					BaseMesh->SetRelativeRotation(RInterpConstantToNoNormalized(BaseMesh->GetRelativeRotation(), BaseMeshTargetRotator, DeltaTime, TurnSpeed));
					MountMesh->SetRelativeRotation(RInterpConstantToNoNormalized(MountMesh->GetRelativeRotation(), MountMeshTargetRotator, DeltaTime, ElevationSpeed));
				}
				
				// Check turret aiming target
				if (round(BaseMesh->GetRelativeRotation().Yaw * 1000) == round(targetRotator.Yaw * 1000) &&
					round(MountMesh->GetRelativeRotation().Pitch * 1000) == round(targetRotator.Pitch * 1000))
				{
					TurretState = EWeaponMountState::ReadyToShoot;
				}
			}
			else 
			{
				BaseMesh->SetRelativeRotation(FMath::RInterpConstantTo(BaseMesh->GetRelativeRotation(), Root->GetRelativeRotation(), DeltaTime, TurnSpeed));
				MountMesh->SetRelativeRotation(FMath::RInterpConstantTo(MountMesh->GetRelativeRotation(), Root->GetRelativeRotation(), DeltaTime, ElevationSpeed));
			}
		}
	}
}

FVector ALFWeaponMount::GetWorldLocation() const
{
	return Root->GetComponentLocation();
}

void ALFWeaponMount::TurnToPos(FVector TargetPos)
{
	this->TargetPosition = TargetPos;
}

void ALFWeaponMount::PullOut()
{
	bIsActivated = true;
}

void ALFWeaponMount::PullIn()
{
	bIsActivated = false;
}

void ALFWeaponMount::TurnOn()
{
	bIsOnline = true;
}

void ALFWeaponMount::TurnOff()
{
	bIsOnline = false;
}

EWeaponMountState ALFWeaponMount::GetState()
{
	return TurretState;
}

bool ALFWeaponMount::IsCanRotateTo(FRotator Rotator)
{
	if (MinAngles.Pitch < Rotator.Pitch && MaxAngles.Pitch > Rotator.Pitch)
	{
		if(MinAngles.Yaw < Rotator.Yaw && MaxAngles.Yaw > Rotator.Yaw)
		{
			return true;
		}
	}
	return false;
}

FRotator ALFWeaponMount::GetLocalRotateToPosition(FVector Position)
{
	FVector targetDirection = FVector(Position - MountMesh->GetComponentLocation());
	FVector targetLocalPosition = Root->GetComponentTransform().InverseTransformVector(targetDirection);
	return targetLocalPosition.Rotation();
}


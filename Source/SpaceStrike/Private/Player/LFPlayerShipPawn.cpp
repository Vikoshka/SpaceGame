// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Player/LFPlayerShipPawn.h"

#include "LFPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/Functions/LFEulerQuaternion.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerShipPawn, All, All);

ALFPlayerShipPawn::ALFPlayerShipPawn()
	: Super()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArmComponent->SocketOffset.X = 0;
	SpringArmComponent->SocketOffset.Y = 0;
	SpringArmComponent->SocketOffset.Z = 300;
	SpringArmComponent->TargetArmLength = 1000;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 5.0f;
	SpringArmComponent->CameraRotationLagSpeed = 5.0f;
	SpringArmComponent->CameraLagMaxDistance = 250.0f;
	SpringArmComponent->bClampToMaxPhysicsDeltaTime = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeRotation(FRotator(0, 0, 0));
	CameraComponent->bUsePawnControlRotation = false;
}

void ALFPlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALFPlayerShipPawn::BeginPlay()
{
	Super::BeginPlay();
		
	OwnerShipController = Cast<ALFPlayerController>(GetController());
}

void ALFPlayerShipPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALFPlayerShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement and Rotation
	MoveForwardShip(DeltaTime);
	ChangeOfControl(DeltaTime);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/**
 * @brief 
 * @param Movement
 */

void ALFPlayerShipPawn::MoveForward(float Scale)
{
	bIsBoostSpeedForward = MoveForwardScaleTarget > 0.0f;

	if (bSwapControl)
	{
		MoveForwardScaleTarget = Scale;
	}
	else
	{
		MoveForwardScaleTarget = FMath::Clamp(MoveForwardScaleTarget + Scale / 100, -1, 1);
	}
	MoveForwardScaleCurrent = FMath::Lerp(MoveForwardScaleCurrent,
	                                      MoveForwardScaleTarget,
	                                      MoveForwardSpeedAcceleration);

	MoveBoostForwardScaleCurrent = FMath::InterpSinOut(MoveBoostForwardScaleCurrent,
	                                                   MoveForwardScaleTarget,
	                                                   MoveBoostForwardSpeedAcceleration);
}

void ALFPlayerShipPawn::MoveForwardShip(float DeltaTime)
{
	if (IsDead) return;

	const auto Location = GetActorLocation();

	float CurrentSpeedForward;

	if (bWantsToSpeedUp && bIsBoostSpeedForward && GetVelocity().IsZero())
	{
		CurrentSpeedForward = MoveBoostForwardScaleCurrent * MoveBoostForwardSpeed;
		if (DebugInfoMovement)
		{
			UE_LOG(LogPlayerShipPawn, Display, TEXT("bIsBoostSpeedForward = true"));
		}
	}
	else
	{
		CurrentSpeedForward = MoveForwardScaleCurrent * MoveForwardSpeed;
		if (DebugInfoMovement)
		{
			UE_LOG(LogPlayerShipPawn, Display, TEXT("bIsBoostSpeedForward = false"));
		}
	}
	SetActorLocation(Location + GetActorForwardVector() * CurrentSpeedForward * DeltaTime, true);

	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, FString::Printf(
															TEXT("MoveForwardScaleCurrent: %f"), MoveForwardScaleCurrent));
	}
}

void ALFPlayerShipPawn::MoveRight(float Scale)
{
	MoveRightScaleTarget = Scale;
	MoveRightScaleCurrent = FMath::Lerp(MoveRightScaleCurrent,
	                                    MoveRightScaleTarget,
	                                    MoveRightSpeedAcceleration);
}

void ALFPlayerShipPawn::MoveRightShip(float DeltaTime)
{
	if (IsDead) return;

	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorRightVector() * MoveRightScaleCurrent * MoveRightSpeed * DeltaTime, true);

	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, FString::Printf(
															TEXT("MoveRightScaleCurrent: %f"), MoveRightScaleCurrent));
	}
}

void ALFPlayerShipPawn::MoveUp(float Scale)
{
	MoveUpScaleTarget = Scale;
	MoveUpScaleCurrent = FMath::Lerp(MoveUpScaleCurrent,
	                                 MoveUpScaleTarget,
	                                 MoveUpSpeedAcceleration);
}

void ALFPlayerShipPawn::MoveUpShip(float DeltaTime)
{
	if (DebugInfoMovement) return;

	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorUpVector() * MoveUpScaleCurrent * MoveUpSpeed * DeltaTime, true);
	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Purple, FString::Printf(
															TEXT("MoveUpScaleCurrent: %f"), MoveUpScaleCurrent));
	}
}

void ALFPlayerShipPawn::ChangeOfControl(float DeltaTime)
{
	const TObjectPtr<ALFPlayerController> PlayerController = Cast<ALFPlayerController>(OwnerShipController);

	if (!IsValid(PlayerController)) return;

	if (bSwapControl)
	{
		UE_LOG(LogPlayerShipPawn, Display, TEXT("bSwapControl: true"));

		PlayerController->bShowMouseCursor = true;

		MoveRightShip(DeltaTime);
		MoveUpShip(DeltaTime);

		ULFEulerQuaternion::AddActorLocalRotationQuaternion(
			this, ULFEulerQuaternion::Euler_To_Quaternion(MovementCurrentRotation));

		ULFEulerQuaternion::SetRelativeRotationQuaternion(
			SpringArmComponent, ULFEulerQuaternion::Euler_To_Quaternion(CameraCurrentRotation));
	}
	else
	{
		UE_LOG(LogPlayerShipPawn, Display, TEXT("bSwapControl: false"));

		PlayerController->bShowMouseCursor = false;
		ULFEulerQuaternion::AddActorLocalRotationQuaternion(
			this, ULFEulerQuaternion::Euler_To_Quaternion(MovementCurrentRotation));

		ULFEulerQuaternion::AddLocalRotationQuaternion(
			SpringArmComponent, ULFEulerQuaternion::Euler_To_Quaternion(CameraCurrentRotation));
	}
}

void ALFPlayerShipPawn::StartBoostSpeedForward()
{
	bWantsToSpeedUp = true;
}

void ALFPlayerShipPawn::StopBoostSpeedForward()
{
	bWantsToSpeedUp = false;
}

/**
 * @brief 
 * @param Rotation
 */

void ALFPlayerShipPawn::TurnRollRight(float Scale)
{
	if (IsDead) return;

	TurnRollRightScaleTarget = Scale;
	TurnRollRightScaleCurrent = FMath::Lerp(TurnRollRightScaleCurrent, TurnRollRightScaleTarget,
	                                        TurnRollRightSpeedAcceleration);

	MovementCurrentRotation.Roll = (TurnRollRightSpeed / 100) * TurnRollRightScaleCurrent;

	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, FString::Printf(
															TEXT("TurnRollRightScaleCurrent: %f"), TurnRollRightScaleCurrent));

		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red,FString::Printf(
															TEXT("TurnRollRightScaleTarget: %f"), TurnRollRightScaleTarget));
	}
}

void ALFPlayerShipPawn::TurnPitchUp(float Scale)
{
	if (IsDead) return;

	TurnPitchUpScaleTarget = Scale;

	TurnPitchUpScaleCurrent = FMath::Lerp(TurnPitchUpScaleCurrent, TurnPitchUpScaleTarget,
	                                      TurnPitchUpSpeedAcceleration);

	MovementCurrentRotation.Pitch = (TurnPitchUpSpeed / 100) * TurnPitchUpScaleCurrent;

	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, FString::Printf(
															TEXT("TurnPitchUpScaleCurrent: %f"), TurnPitchUpScaleCurrent));

		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, FString::Printf(
															TEXT("TurnPitchUpScaleTarget: %f"), TurnPitchUpScaleTarget));
	}
}

void ALFPlayerShipPawn::TurnYawRight(float Scale)
{
	if (IsDead) return;

	TurnYawRightScaleTarget = Scale;

	TurnYawRightScaleCurrent = FMath::Lerp(TurnYawRightScaleCurrent, TurnYawRightScaleTarget,
	                                       TurnYawRightSpeedAcceleration);

	MovementCurrentRotation.Yaw = (TurnYawRightSpeed / 100) * TurnYawRightScaleCurrent;
	if (DebugInfoMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, FString::Printf(
															TEXT("TurnYawRightScaleCurrent: %f"), TurnYawRightScaleCurrent));

		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, FString::Printf(
															TEXT("TurnYawRightScaleTarget: %f"), TurnYawRightScaleTarget));
	}
}

void ALFPlayerShipPawn::TurnPitchUpCamera(float Scale)
{
	if (IsDead) return;

	if (!bSwapControl)
	{
		CameraCurrentRotation.Pitch = Scale;
	}
}

void ALFPlayerShipPawn::TurnYawRightCamera(float Scale)
{
	if (IsDead) return;

	if (!bSwapControl)
	{
		CameraCurrentRotation.Yaw = Scale;
	}
}

void ALFPlayerShipPawn::SwapControl()
{
	bSwapControl = !bSwapControl;
}

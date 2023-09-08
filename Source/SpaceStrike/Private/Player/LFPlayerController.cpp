// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Player/LFPlayerController.h"

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogLFPlayerController, All, All);

void ALFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	PlayerShip = CastChecked<ALFPlayerShipPawn>(GetPawn());
}

void ALFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ALFPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ALFPlayerController::OnMoveRight);
	InputComponent->BindAxis("MoveUp", this, &ALFPlayerController::OnMoveUp);

	InputComponent->BindAction("BoostSpeedForward", IE_Pressed, this, &ALFPlayerController::OnStartBoostSpeedForward);
	InputComponent->BindAction("BoostSpeedForward", IE_Released, this, &ALFPlayerController::OnStopBoostSpeedForward);

	InputComponent->BindAxis("TurnRollRight", this, &ALFPlayerController::OnTurnRollRight);
	InputComponent->BindAxis("TurnPitchUp", this, &ALFPlayerController::OnTurnPitchUp);
	InputComponent->BindAxis("TurnYawRight", this, &ALFPlayerController::OnTurnYawRight);

	InputComponent->BindAxis("TurnPitchUpCamera", this, &ALFPlayerController::OnTurnPitchUpCamera);
	InputComponent->BindAxis("TurnYawRightCamera", this, &ALFPlayerController::OnTurnYawRightCamera);
	
	InputComponent->BindAction("SwapControl", IE_Pressed, this, &ALFPlayerController::OnSwapControl);
}

void ALFPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ScreenPosition();
	TargetInTheDirectionOfTheCameraAndMouse();
}

void ALFPlayerController::OnMoveForward(float Value)
{
	if (PlayerShip)
		PlayerShip->MoveForward(Value);
}

void ALFPlayerController::OnMoveRight(float Value)
{
	if (PlayerShip)
		if (PlayerShip->GetSwapControl())
		{
			PlayerShip->MoveRight(Value);
		}
		else
		{
			PlayerShip->TurnYawRight(Value);
		}
		
}

void ALFPlayerController::OnMoveUp(float Value)
{
	if (PlayerShip)
		if (PlayerShip->GetSwapControl())
		{
			PlayerShip->MoveUp(Value);
		}
		else
		{
			PlayerShip->TurnPitchUp(Value);
		}
}

void ALFPlayerController::OnTurnRollRight(float Value)
{
	if (PlayerShip)
		PlayerShip->TurnRollRight(Value);
}

void ALFPlayerController::OnTurnPitchUp(float Value)
{
	if (PlayerShip)
	{
		if (PlayerShip->GetSwapControl())
		{
			PlayerShip->TurnPitchUp(GetInputMousePosition2D().Y);
		}
	}
}

void ALFPlayerController::OnTurnYawRight(float Value)
{
	if (PlayerShip)
	{
		if (PlayerShip->GetSwapControl())
		{
			PlayerShip->TurnYawRight(GetInputMousePosition2D().X);
		}
	}
}

void ALFPlayerController::OnTurnPitchUpCamera(float Value)
{
	if (PlayerShip)
	{
		if (!PlayerShip->GetSwapControl())
		{
			PlayerShip->TurnPitchUpCamera(Value);
		}
	}
}

void ALFPlayerController::OnTurnYawRightCamera(float Value)
{
	if (PlayerShip)
	{
		if (!PlayerShip->GetSwapControl())
		{
			PlayerShip->TurnYawRightCamera(Value);
		}
	}
}

void ALFPlayerController::OnStartBoostSpeedForward()
{
	if (PlayerShip)
		PlayerShip->StartBoostSpeedForward();
}

void ALFPlayerController::OnStopBoostSpeedForward()
{
	if (PlayerShip)
		PlayerShip->StopBoostSpeedForward();
}

void ALFPlayerController::OnSwapControl()
{
	if (PlayerShip)
		PlayerShip->SwapControl();
}

FVector2f ALFPlayerController::ScreenPosition()
{
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	if (PlayerShip)
	{
		RadiusForMaximumTurningForce = FMath::Min(ViewportSizeX, ViewportSizeY) / PlayerShip->RadiusScaleInputMouse;
	}
	else
	{
		RadiusForMaximumTurningForce = FMath::Min(ViewportSizeX, ViewportSizeY) / 5.0f;
	}

	if (GetMousePosition(MousePositionSizeX, MousePositionSizeY))
	{
		const float CursorDeflectionLength = (FVector2f(MousePositionSizeX, MousePositionSizeY) - FVector2f(
			ViewportSizeX / 2, ViewportSizeY / 2)).Length();

		if (PlayerShip->DebugInfoMouse)
		{
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue,
			                          FString::Printf(
			                                 TEXT("Cursor Deflection Length: %f"), CursorDeflectionLength));
		}

		if (CursorDeflectionLength > RadiusForMaximumTurningForce)
		{
			InputMouse = (FVector2f(MousePositionSizeX, MousePositionSizeY) - FVector2f(
				ViewportSizeX / 2, ViewportSizeY / 2)).GetSafeNormal();
		}
		else
		{
			InputMouse = (FVector2f(MousePositionSizeX, MousePositionSizeY) - FVector2f(
				ViewportSizeX / 2, ViewportSizeY / 2)) / RadiusForMaximumTurningForce;
		}
	}
	else
	{
		InputMouse = FVector2f();
	}

	if (PlayerShip->DebugInfoMouse)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue,
		                         FString::Printf(TEXT("InputMouse.X: %f "), InputMouse.X));
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue,
		                         FString::Printf(TEXT("InputMouse.Y: %f "), InputMouse.Y));
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue,
		                         FString::Printf(TEXT("Radius For Maximum Turning Force: %f"),
		                                                        RadiusForMaximumTurningForce));
	}
	return InputMouse;
}

void ALFPlayerController::TargetInTheDirectionOfTheCameraAndMouse()
{
	if (!bShowMouseCursor) return;

	DeprojectMousePositionToWorld(MousePosition, MouseDirection);
	GetPlayerViewPoint(CameraLocation, CameraRotation);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerShip);

	GetWorld()->LineTraceSingleByChannel(ResultMousePosition, MousePosition, GetWorldPointByMouseDirection(),
	                                     ECollisionChannel::ECC_Visibility, CollisionParams);

	if (ResultMousePosition.bBlockingHit)
	{
		if (PlayerShip->DebugInfoMouse)
		{
			DrawDebugLine(GetWorld(), MousePosition, ResultMousePosition.ImpactPoint, FColor::Green);
			DrawDebugSphere(GetWorld(), ResultMousePosition.ImpactPoint, 3, 16, FColor::Red);
			DrawDebugSphere(GetWorld(), ResultMousePosition.ImpactPoint, 1000, 48, FColor::Red);
			UE_LOG(LogLFPlayerController, Display, TEXT("ResultMousePosition = true"));
		}
	}
	else
	{
		if (PlayerShip->DebugInfoMouse)
		{
			DrawDebugLine(GetWorld(), MousePosition, GetWorldPointByMouseDirection(), FColor::Green);
			DrawDebugSphere(GetWorld(), GetWorldPointByMouseDirection(), 3, 16, FColor::Purple);
			DrawDebugSphere(GetWorld(), GetWorldPointByMouseDirection(), 1000, 48, FColor::Purple);

			UE_LOG(LogLFPlayerController, Display, TEXT("ResultMousePosition = false"));
		}
	}

	GetWorld()->LineTraceSingleByChannel(ResultCameraPosition, CameraLocation, GetWorldPointByCameraDirection(),
	                                     ECollisionChannel::ECC_Visibility, CollisionParams);

	if (ResultCameraPosition.bBlockingHit)
	{
		if (PlayerShip->DebugInfoMouse)
		{
			DrawDebugLine(GetWorld(), CameraLocation, ResultCameraPosition.ImpactPoint, FColor::Green);
			DrawDebugSphere(GetWorld(), ResultCameraPosition.ImpactPoint, 3, 16, FColor::Red);
			DrawDebugSphere(GetWorld(), ResultCameraPosition.ImpactPoint, 1000, 48, FColor::Red);
			UE_LOG(LogLFPlayerController, Display, TEXT("ResultCameraPosition = true"));
		}
	}
	else
	{
		if (PlayerShip->DebugInfoMouse)
		{
			DrawDebugLine(GetWorld(), CameraLocation, GetWorldPointByCameraDirection(), FColor::Green);
			DrawDebugSphere(GetWorld(), GetWorldPointByCameraDirection(), 3, 16, FColor::Purple);
			DrawDebugSphere(GetWorld(), GetWorldPointByCameraDirection(), 1000, 48, FColor::Purple);
			UE_LOG(LogLFPlayerController, Display, TEXT("ResultCameraPosition = false"));
		}
	}
}

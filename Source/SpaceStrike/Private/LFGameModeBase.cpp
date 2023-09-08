// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "LFGameModeBase.h"
#include "Player/LFBaseShipPawn.h"
#include "Player/LFPlayerController.h"
#include "UI/LFGameHUD.h"

ALFGameModeBase::ALFGameModeBase()
{
	DefaultPawnClass = ALFBaseShipPawn::StaticClass();
	PlayerControllerClass = ALFPlayerController::StaticClass();
	HUDClass = ALFGameHUD::StaticClass();
}

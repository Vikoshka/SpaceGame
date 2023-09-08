// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class  ETypeOfShip: uint8
{
	Unknown,
	Fighter,
	Interceptor,
	ReconnaissanceAircraft,
	AttackAircraft,
	Corvette,
	Frigate,
	Cruiser,
	Battleship
};

UENUM(BlueprintType)
enum class EFactionType: uint8
{
	Unknown,
	SpaceResistanceForces,
	DarkSwarm,
	Pirates,
	Kapers,
	Varns UMETA(DisplayName = "VARNs"),
	Taki UMETA(DisplayName = "TAKi")
};

UENUM(BlueprintType)
enum class ETypeOfRelationship: uint8
{
	Neutral,
	Friendly,
	Aggressive
};

UENUM(BlueprintType)
enum class EShipBotBehaviour: uint8
{
	Dormant,
	SystemScanning,
	MiningResources,
	Patrol,
	PursuitTarget,
	GuardThePoint,
	GuardTheTarget,
	FollowsTheTarget,
	MovingToInterestPoint
};

UENUM(BlueprintType)
enum class EPathfindingStatus: uint8
{
	None,
	InProgress,
	Success,
	Failed,
};
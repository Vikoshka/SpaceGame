// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/LFBaseShipPawn.h"
#include "LFBaseShipBotPawn.generated.h"


UENUM()
enum class ECurrentShipBotState: int8
{
	Unknown,
	FindingPathToTarget,
	Patrolling,
	ChasingTarget,
	MovingToPatrolLocation,
	Fights,
	MovingToTarget
};

UCLASS(Blueprintable, BlueprintType)
class SPACESTRIKE_API ALFBaseShipBotPawn : public ALFBaseShipPawn
{
	GENERATED_BODY()

public:
	ALFBaseShipBotPawn();	
protected:
			
	uint32 CachedPlayerShipUniqueId;
			
	//Default Bot Ship behaviour when the spawned.
	UPROPERTY(EditAnywhere, meta=(DisplayName="Default Bot Ship Behaviour"), Category="Behaviour")
	EShipBotBehaviour DefaultBotShipBehaviour= EShipBotBehaviour::Dormant;

	ECurrentShipBotState CurrentShipBotState = ECurrentShipBotState::Unknown;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual  void Tick(float DeltaSeconds) override;
		
	UFUNCTION()
	void OnShipAppeared(int32 ShipUniqueId);

	UFUNCTION()
	void OnShipDisappeared(int32 ShipUniqueId);
	
	FRotator RInterpConstantToNoNormalized(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);

	FRotator GetLocalRotateToPosition(FVector Position);
};



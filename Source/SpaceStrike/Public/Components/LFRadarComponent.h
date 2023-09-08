// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/LFEnums.h"
#include "LFRadarComponent.generated.h"

class ALFGameStateBase;
class ALFBaseShipPawn;

UENUM(BlueprintType)
enum class EDefaultRadarBehaviour: uint8
{
	Disabled,
	AutoActivated,
};

//This class describes the basic implementation of a ship's radar.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESTRIKE_API ULFRadarComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipAppeared, int32, UniqueShipId);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShipDisappeared, int32, UniqueShipId);

public:

	UPROPERTY(BlueprintAssignable)
	FOnShipAppeared OnShipAppeared;

	UPROPERTY(BlueprintAssignable)
	FOnShipDisappeared OnShipDisappeared;
	
	// Sets default values for this component's properties
	explicit ULFRadarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	//Pool of ships that fell into the radar scan radius.
	UPROPERTY()
	TMap<uint32, ALFBaseShipPawn*> RepositoryOfShips;
	
	//	1.In game context - how often the radar will try to detect targets.													
	//	2.How often will ScanShips() be called per tick if we use the component's tick detection function.					
	//	3.How often the timer will be called - if the scan is timed.														 
	///	@Example1:ScanningRate = 1 and we want to use a timer scan - once a second, the timer will update information about the sighted targets.	
	///	@Example2:ScanningRate = 0 and we are using a constant scan, then ScanShips() will be called every tick			

	UPROPERTY(EditAnywhere, Category="Radar Settings", meta=(ClampMin = 0.0f))
	float ScanningRate = 0.0f;

	//The radius within which the radar can detect ships.
	UPROPERTY(EditAnywhere, Category="Radar Settings", meta=(ClampMin = 0.0f))
	float DetectionRange = 0.0f;

	//How long will it take to restart the radar.
	///@Note:At current time this property doesn`t use.
	UPROPERTY(EditAnywhere, Category="Radar Settings", meta=(ClampMin = 0.0f))
	float RadarResetTime = 0.0f;

	EFactionType DetectionByAffiliationFaction;

	//Whether scanning is automatically enabled by default at BeginPlay.
	//Set this parameter to AutoActivated if you want the radar to start scanning automatically
	//If the AutoActivated parameter is selected - the tick will be enabled for the component, then the targets will be updated every tick, where tick updates often = ScanningRate.
	//If the Disabled - the tick of the component will be turned off, scanning will not occur.
	UPROPERTY(EditAnywhere, Category="Radar Settings")
	EDefaultRadarBehaviour DefaultRadarBehaviour = EDefaultRadarBehaviour::Disabled;
	
	TObjectPtr<ALFBaseShipPawn> RadarOwner;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void TryAutoActivateRadar();
	
	void OnRemoveShip(ALFBaseShipPawn* InShip);

	//Updating information about ships.
	virtual void ScanShips();

public:

	//Sets a new ship detection speed and changes the tick update rate.
	UFUNCTION(BlueprintCallable)
	void SetScanningRate(const float NewRate);

	//Sets a new radar detection range.
	UFUNCTION(BlueprintCallable)
	void SetNewDetectionRange(const float NewRange);

	//Returns the radius within which the radar can detect ships.
	UFUNCTION(BlueprintCallable)
	float GetDetectionRange() const;
	
	//Starts the targets discovery process. This method enabled tick component.
	UFUNCTION(BlueprintCallable)
	void ActivateRadar();

	//Stops the targets detection process and disabled tick component. 
	UFUNCTION(BlueprintCallable)
	void DeactivateRadar();

	//Returns all ships spotted by the radar, filtering out the ones needed for the given type of relationship between factions.
	//@Usage example: If you want to get friendly ships, pass the type of relationship - ETypeOfRelationship::Friendly.
	///@param OutShips - the array where the noticed ships will be placed.
	///@param TypeOfRelationship - type of relationship between factions by which the noticed ships will be filtered.
	///@return - count elements added to OutArray.
	UFUNCTION(BlueprintCallable)
	int32 GetShipsByFactionRelationship(TArray<ALFBaseShipPawn*>& OutShips, const ETypeOfRelationship& TypeOfRelationship) const;

	UFUNCTION(BlueprintCallable, Category = "Get Ships")
	int32 GetEnemyShips(TArray<ALFBaseShipPawn*>& OutShips);
	
	//Returns all ships detected by the radar.
	///@param OutShips - the array where the noticed ships will be placed.
	///@return - count elements added to OutArray.
	UFUNCTION(BlueprintCallable, Category = "Get Ships")
	int32 GetAllDetectedShips(TArray<ALFBaseShipPawn*>& OutShips) const;

	//Returns the nearest enemy ship.
	///@Note:When calling the function, take into account the fact that when calculating the distance,
	//line of sight is used without taking into account the presence of obstacles between ships - only the distance between ships is considered.
	UFUNCTION(BlueprintCallable)
	ALFBaseShipPawn* GetNearestEnemyShip();
	
	UFUNCTION(BlueprintCallable)
	float GetDistanceToNearestEnemy();	
};



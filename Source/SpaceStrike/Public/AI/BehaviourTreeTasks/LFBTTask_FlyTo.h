// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/LFEnums.h"
#include "CPathFindPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "LFBTTask_FlyTo.generated.h"

class ALFBaseShipBotPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalLocationChanged, const FVector&, NewGoalLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackedMovingGoalChanged, AActor*, NewTrackedMovingGoal);

USTRUCT(BlueprintType)
struct SPACESTRIKE_API FLFAIFlyRequest
{
	GENERATED_BODY()

	FLFAIFlyRequest();
	explicit FLFAIFlyRequest(const TObjectPtr<AActor> InGoalActor);
	explicit FLFAIFlyRequest(const FVector& InGoalLocation);

	FLFAIFlyRequest& SetUsePathfinding(bool bPathfinding) { bUsePathfinding = bPathfinding; return *this; }
	FLFAIFlyRequest& SetAllowPartialPath(bool bAllowPartial) { bAllowPartialPath = bAllowPartial; return *this; }
	FLFAIFlyRequest& SetProjectGoalLocation(bool bProject) { bProjectGoalOnNavigation = bProject; return *this; }

	FLFAIFlyRequest& SetReachTestIncludesAgentRadius(bool bIncludeRadius) { bReachTestIncludesAgentRadius = bIncludeRadius; return *this; }
	FLFAIFlyRequest& SetReachTestIncludesGoalRadius(bool bIncludeRadius) { bReachTestIncludesGoalRadius = bIncludeRadius; return *this; }
	FLFAIFlyRequest& SetAcceptanceRadius(float Radius) { AcceptanceRadius = Radius; return *this; }
	FLFAIFlyRequest& SetUserData(const FCustomMoveSharedPtr& InUserData) { UserData = InUserData; return *this; }

	/** the request should be either set up to move to a location, of go to a valid actor */
	bool IsValid() const { return bInitialized && (!bFlyToActor || GoalActor); }
	
	bool IsFlyToActorRequest() const { return bFlyToActor; }
	TObjectPtr<AActor> GetGoalActor() const { return bFlyToActor ? GoalActor : nullptr; }
	FVector GetGoalLocation() const { return GoalLocation; }
	/** retrieves request's requested destination location, GoalActor's location 
	 *	or GoalLocation, depending on the request itself */
	FVector GetDestination() const { return bFlyToActor ? (GoalActor ? GoalActor->GetActorLocation() : FAISystem::InvalidLocation) : GoalLocation; }

	bool IsUsingPathfinding() const { return bUsePathfinding; }
	bool IsUsingPartialPaths() const { return bAllowPartialPath; }
	bool IsProjectingGoal() const { return bProjectGoalOnNavigation; }

	bool IsReachTestIncludingAgentRadius() const { return bReachTestIncludesAgentRadius; }
	bool IsReachTestIncludingGoalRadius() const { return bReachTestIncludesGoalRadius; }
	float GetAcceptanceRadius() const { return AcceptanceRadius; }
	const FCustomMoveSharedPtr& GetUserData() const { return UserData; }
	int32 GetUserFlags() const { return UserFlags; }

	void SetGoalActor(const TObjectPtr<AActor> InGoalActor);
	void SetGoalLocation(const FVector& InGoalLocation);
	bool UpdateGoalLocation(const FVector& NewLocation) const;

protected:

	/** move goal: actor */
	UPROPERTY()
	TObjectPtr<AActor> GoalActor;

	/** move goal: location */
	mutable FVector GoalLocation;
	
	/** move goal is an actor */
	uint32 bInitialized : 1;

	/** move goal is an actor */
	uint32 bFlyToActor : 1;

	/** pathfinding: if set - regular pathfinding will be used, if not - direct path between two points */
	uint32 bUsePathfinding : 1;

	/** pathfinding: allow using incomplete path going toward goal but not reaching it */
	uint32 bAllowPartialPath : 1;

	/** pathfinding: goal location will be projected on navigation data before use */
	uint32 bProjectGoalOnNavigation : 1;

	/** pathfollowing: acceptance radius needs to be increased by agent radius (stop on overlap vs exact point) */
	uint32 bReachTestIncludesAgentRadius : 1;

	/** pathfollowing: acceptance radius needs to be increased by goal actor radius */
	uint32 bReachTestIncludesGoalRadius : 1;

	/** pathfollowing: required distance to goal to complete move */
	float AcceptanceRadius;

	/** custom user data: structure */
	FCustomMoveSharedPtr UserData;

	/** custom user data: flags */
	int32 UserFlags;
};

UENUM(BlueprintType)
enum class EFlyToFailReason: uint8
{
	None,
	InvalidAIPawn,
	InvalidGoalActor,
	VolumeNotValid,
	VolumeNotGenerated,
	WrongStartLocation,
	WrongEndLocation,
	EndLocationUnreachable,
	Unknown
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlyTaskCompletedSignature, TEnumAsByte<EPathFollowingResult::Type>, Result);

UCLASS(DisplayName="AIFlyTo")
class SPACESTRIKE_API  ULFAITask_FlyTo final : public UBlueprintAsyncActionBase, public FTickableGameObject 
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FFlyTaskCompletedSignature OnFlyFinished;

	UFUNCTION(BlueprintCallable, Category = AI, meta = (BlueprintInternalUseOnly = "true"))
	static ULFAITask_FlyTo* FlyToActor(ALFBaseShipBotPawn* InShipBotPawn = nullptr,	AActor* InGoalActor = nullptr,	bool bTrackMovingGoal = false, float InAcceptableRadius = 50.f);

	UFUNCTION(BlueprintCallable, Category = AI, meta = (BlueprintInternalUseOnly = "true"))
	static ULFAITask_FlyTo* FlyToLocation(ALFBaseShipBotPawn* InShipBotPawn = nullptr, const FVector& GoalLocation = FVector::ZeroVector,  float InAcceptableRadius = 50.f);

	/** Switch task into continuous tracking mode: keep restarting move toward goal actor. Only pathfinding failure or external cancel will be able to stop this task. */
	void SetContinuousGoalTracking(bool bEnable) { FlayRequest.IsFlyToActorRequest() ? bUseContinuousTracking = bEnable : false; }
	
	virtual void Activate() override;
	
protected:

	virtual void BeginDestroy() override;
	
	virtual ETickableTickType GetTickableTickType() const override;

	virtual bool IsTickable() const override;
	
	virtual TStatId GetStatId() const override;

	virtual  bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;
	
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void PerformFly();
	
	//Using only for find path async.
	UFUNCTION()
	void OnFindPathCompletedCallback(const TArray<FCPathNode>& Path, TEnumAsByte<ECPathfindingFailReason> FailReason);

	/** event from path following */
	//virtual void OnRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	
	void FinishFlyTask(EPathFollowingResult::Type InResult);

	void OnRequestFinished(EPathFollowingResult::Type InResult);
	
	/// @note Call only inside the Tick!!!
	void TickFollowingPath(float DeltaSeconds);

	bool IsGoalLocationReached() const;

	bool IsPathAvailable() const;
	
	bool bEnableTaskTick = false;

	bool bUseContinuousTracking = false;

	FVector CurrentGoalLocation = FAISystem::InvalidLocation;
	
	// Track The last frame number we were ticked.
	uint32 LastTickFrame = INDEX_NONE;
	
	FLFAIFlyRequest FlayRequest;

	TObjectPtr<UCPathAsyncFindPath> FindPathAsyncTaskPtr;
	
	TObjectPtr<ALFBaseShipBotPawn> ShipBotPawn;

	TArray<FCPathNode> PathPoints;
	
	int32 CurrentPathPointIndex = INDEX_NONE;
	
	float MoveBoostForwardScaleCurrent = 0.0f;

	float MoveForwardScaleCurrent = 0.0f;

	EPathFollowingStatus::Type PathFollowingStatus = EPathFollowingStatus::Idle;

	EPathFollowingResult::Type PathFollowingResult = EPathFollowingResult::Type::Aborted;

	EPathfindingStatus PathfindingStatus = EPathfindingStatus::None;

	static TMap<ECPathfindingFailReason, FString> PathfindingFailReasonToErrorMessage;
};


																									



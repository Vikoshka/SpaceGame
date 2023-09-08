// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LFEnums.h"
#include "LFGameStateBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LFBlueprintHelperFunctionLibrary.generated.h"

class ACPathVolume;

UCLASS()
class SPACESTRIKE_API ULFBlueprintHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Returns fractions by the given relationship type.
	//Example: Return hostile factions for the given faction if you need to get the factions that are hostile to it.
	UFUNCTION(BlueprintCallable, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType FactionRelationships"), Category="Factions Relationships")
	static void GetFactionsByRelationshipType(const UObject* WorldContextObject, const EFactionType FactionType, const ETypeOfRelationship RelationshipType, TArray<EFactionType>& OutFactions);

	//Returns the current relationship between two factions.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(Keywords = "BlueprintHelperFunctionLibrary Get GlobalFactionRelationship Faction Relationships FactionRelationships"), Category="Factions Relationships")
	static ETypeOfRelationship GetRelationshipsBetweenTwoFactions(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2);

	//Returns friendly factions to the given faction.
	UFUNCTION(BlueprintCallable, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType FactionRelationships"), Category="Factions Relationships")
	static void GetFriendlyFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions);

	//Returns hostile factions to the given faction.
	UFUNCTION(BlueprintCallable, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType FactionRelationships"), Category="Factions Relationships")
	static void GetHostileFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions);

	//Returns fractions with which neutrality with given fraction.
	UFUNCTION(BlueprintCallable, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType FactionRelationships"), Category="Factions Relationships")
	static void GetNeutralFactions(const UObject* WorldContextObject, const EFactionType FactionType, TArray<EFactionType>& OutFactions);

	UFUNCTION(BlueprintCallable, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType FactionRelationships"), Category="Ships")
	static void GetShipsByRelationshipType(const ALFBaseShipPawn* InShipPawnContext, TArray<ALFBaseShipPawn*>& OutShips, const ETypeOfRelationship RelationshipType);

	//Returns true if factions are in the given relationship.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(Keywords = "BlueprintHelperFunctionLibrary Get GlobalFactionRelationship Faction Relationships FactionRelationships"), Category="Factions Relationships")
	static bool IsInRelationships(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2, const ETypeOfRelationship RelationshipType);

	//Returns true if factions are friends.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(Keywords = "BlueprintHelperFunctionLibrary Get Faction Relationship RelationshipType IsFriends Friends FactionRelationships"), Category="Factions Relationships")
	static bool IsFactionsFriendly(const UObject* WorldContextObject, const EFactionType Faction1, const EFactionType Faction2);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetActorUniqueId(AActor* Actor);

	//Finds the object of the desired ship instance by unique identifier in the global ship pool.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ALFBaseShipPawn* FindShipByUniqueId(const UObject* WorldContextObject, int32 ShipUniqueId);

	//Returns our valid custom navigation mesh type ACPathVolume if is placed on the level.
	UFUNCTION(BlueprintCallable,  BlueprintPure, meta=(Keywords = "BlueprintHelperFunctionLibrary AI AINavigation Navigation NavigationVolume NavigationMesh Volume"), Category="AI Navigation")
	static  ACPathVolume* GetCustomNavigationVolume3D(const UObject* WorldContextObject);

	//Check if there is a direct view between locations (if there are any obstacles on the way between two points).
	/// @param WorldContextObject - WorldContextObject.
	/// @param StartLocation - Start line trace.
	/// @param DestinationLocation - End line trace.
	/// @param ActorsToIgnore - Actors which must be ignored in the  test.
	/// @param  bDrawTraceLine - Draw trace line.
	/// @return - returns tru if there are no obstacles on the way between two points.
	UFUNCTION(BlueprintCallable,  meta=(Keywords = "BlueprintHelperFunctionLibrary"))
	static bool IsHasLineSightBetweenLocations(const UObject* WorldContextObject, const FVector& StartLocation, const FVector& DestinationLocation, const TArray<AActor*>& ActorsToIgnore, bool bDrawTraceLine = false);

	//Returns true if our custom navigation mesh type ACPathVolume is placed on the level and builded.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(Keywords = "BlueprintHelperFunctionLibrary AI AINavigation Navigation NavigationVolume NavigationMesh Volume"), Category="AI Navigation")
	static  bool IsNavigationVolumeGenerated(const UObject* WorldContextObject);
};

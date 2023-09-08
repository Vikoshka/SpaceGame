// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LFEnums.h"
#include "FactionRelationships.generated.h"

/************************************************************************************************************************
 *	Structure describing the relationship of factions.																	*
 *	Each instance of the structure stores the type of faction and the relationship of this faction with other factions.	*
 ************************************************************************************************************************/
USTRUCT(BlueprintType)
struct FFactionRelationship
{
	GENERATED_BODY()

	explicit  FFactionRelationship();
	
	explicit FFactionRelationship(const EFactionType& InFactionType, const TMap<EFactionType, ETypeOfRelationship>& InRelationships);
	
	//Returns true if the factions are at odds.	
	FORCEINLINE bool IsEnemy(const EFactionType& CheckFaction) const
	{
		return Relationships[CheckFaction] == ETypeOfRelationship::Aggressive;
	}

	//Returns true if the factions are friendly.
	FORCEINLINE bool IsFriendly(const EFactionType& CheckFaction) const
	{
		return Relationships[CheckFaction] == ETypeOfRelationship::Friendly;
	}

	//Returns true if the factions are neutral to each other.
	FORCEINLINE bool IsNeutral(const EFactionType& CheckFaction) const
	{
		return Relationships[CheckFaction] == ETypeOfRelationship::Neutral;
	}

	//Type of faction. Faction type should be unique.
	UPROPERTY(EditAnywhere)
	EFactionType FactionType;

	//The relationship of the faction specified in the FactionType variable with other factions. Faction type should be unique.
	UPROPERTY(EditAnywhere)
	TMap<EFactionType, ETypeOfRelationship> Relationships;
	
};

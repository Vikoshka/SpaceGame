// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Core/FactionRelationships.h"

FFactionRelationship::FFactionRelationship()
{
}

FFactionRelationship::FFactionRelationship(const EFactionType& InFactionType, const TMap<EFactionType, ETypeOfRelationship>& InRelationships)
		:FactionType(InFactionType), Relationships(InRelationships)
{	
}


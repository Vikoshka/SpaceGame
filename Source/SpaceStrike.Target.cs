// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceStrikeTarget : TargetRules
{
	public SpaceStrikeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SpaceStrike" } );
		ExtraModuleNames.AddRange(new string[]{"CPathfinding"});
	}
}

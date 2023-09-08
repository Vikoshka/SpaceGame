// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceStrikeEditorTarget : TargetRules
{
	public SpaceStrikeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SpaceStrike" } );
		ExtraModuleNames.AddRange(new string[]{"CPathfinding"});
	}
}

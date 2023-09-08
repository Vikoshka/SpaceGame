// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

using UnrealBuildTool;

public class SpaceStrike : ModuleRules
{
	public SpaceStrike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"CPathfinding",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"SpaceStrike/Public/Core",
			"SpaceStrike/Public/AI",
			"SpaceStrike/Public/AI/BehaviourTreeTasks",
			"SpaceStrike/Public/Components",
			"SpaceStrike/Public/Player",
			"SpaceStrike/Public/Player/Functions",
			"SpaceStrike/Public/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

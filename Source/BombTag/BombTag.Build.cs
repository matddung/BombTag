// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BombTag : ModuleRules
{
	public BombTag(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemEOS",
            "OnlineServicesInterface",
            "OnlineServicesEOS"
        });

		PublicIncludePaths.AddRange(new string[] {
			"BombTag",
			"BombTag/Variant_Platforming",
			"BombTag/Variant_Platforming/Animation",
			"BombTag/Variant_Combat",
			"BombTag/Variant_Combat/AI",
			"BombTag/Variant_Combat/Animation",
			"BombTag/Variant_Combat/Gameplay",
			"BombTag/Variant_Combat/Interfaces",
			"BombTag/Variant_Combat/UI",
			"BombTag/Variant_SideScrolling",
			"BombTag/Variant_SideScrolling/AI",
			"BombTag/Variant_SideScrolling/Gameplay",
			"BombTag/Variant_SideScrolling/Interfaces",
			"BombTag/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

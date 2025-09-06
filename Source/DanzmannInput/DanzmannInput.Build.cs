// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

using UnrealBuildTool;

public class DanzmannInput : ModuleRules
{
	public DanzmannInput(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"Engine",
				"EnhancedInput",
				"GameplayTags"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject"
			}
		);
	}
}

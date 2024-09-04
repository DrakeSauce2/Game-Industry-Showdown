// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FightingTemp : ModuleRules
{
	public FightingTemp(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate" ,"SlateCore", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LostSoul : ModuleRules
{
	public LostSoul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GeometryCollectionEngine", "UMG", "AIModule", "Niagara" });
	}
}

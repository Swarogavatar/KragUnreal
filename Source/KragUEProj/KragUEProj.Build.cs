// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KragUEProj : ModuleRules
{
	public KragUEProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]{"UnrealEd"});
		}
	}
}

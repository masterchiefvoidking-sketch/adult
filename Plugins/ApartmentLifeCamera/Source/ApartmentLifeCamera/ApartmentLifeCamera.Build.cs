// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeCamera : ModuleRules
{
	public ApartmentLifeCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"ApartmentLifeCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CinematicCamera"
		});
	}
}

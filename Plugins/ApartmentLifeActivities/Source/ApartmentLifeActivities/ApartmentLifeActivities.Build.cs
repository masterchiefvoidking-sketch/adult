// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeActivities : ModuleRules
{
	public ApartmentLifeActivities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Json",
			"JsonUtilities",
			"ApartmentLifeCore",
			"ApartmentLifeCamera",
			"ApartmentLifeCharacterPipeline",
			"ApartmentLifeWardrobe"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ApartmentLifeWorldSim"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

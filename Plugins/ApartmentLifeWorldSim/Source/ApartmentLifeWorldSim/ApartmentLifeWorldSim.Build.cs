// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeWorldSim : ModuleRules
{
	public ApartmentLifeWorldSim(ReadOnlyTargetRules Target) : base(Target)
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
			"ApartmentLifeAI",
			"ApartmentLifeActivities",
			"ApartmentLifeApartment",
			"ApartmentLifeCity"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeUI : ModuleRules
{
	public ApartmentLifeUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Slate",
			"SlateCore",
			"UMG",
			"ApartmentLifeCore",
			"ApartmentLifeCamera",
			"ApartmentLifeWorldSim",
			"ApartmentLifeWardrobe"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ApartmentLifeApartment",
			"ApartmentLifeActivities",
			"ApartmentLifeInteraction",
			"ApartmentLifeCharacterPipeline",
			"ApartmentLifeImmersion",
			"ApartmentLifeDevTools"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

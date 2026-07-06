// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeDevTools : ModuleRules
{
	public ApartmentLifeDevTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine",
			"ApartmentLifeCore", "ApartmentLifeApartment", "ApartmentLifeWorldSim",
			"ApartmentLifeSocial", "ApartmentLifeCharacter", "ApartmentLifeCharacterPipeline", "ApartmentLifeActivities",
			"ApartmentLifeWardrobe", "ApartmentLifeCamera", "ApartmentLifeImmersion", "ApartmentLifeInteraction"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

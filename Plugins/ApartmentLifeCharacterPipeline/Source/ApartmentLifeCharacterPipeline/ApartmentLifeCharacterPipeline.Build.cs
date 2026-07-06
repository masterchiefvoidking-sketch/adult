// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeCharacterPipeline : ModuleRules
{
	public ApartmentLifeCharacterPipeline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "Json", "JsonUtilities",
			"ApartmentLifeCore", "ApartmentLifeCharacter", "ApartmentLifeWardrobe", "ApartmentLifeActivities",
			"ApartmentLifeCamera", "ApartmentLifeApartment"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

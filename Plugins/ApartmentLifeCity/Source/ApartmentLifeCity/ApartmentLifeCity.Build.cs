// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeCity : ModuleRules
{
	public ApartmentLifeCity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "Json", "JsonUtilities",
			"ApartmentLifeCore", "ApartmentLifeApartment"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

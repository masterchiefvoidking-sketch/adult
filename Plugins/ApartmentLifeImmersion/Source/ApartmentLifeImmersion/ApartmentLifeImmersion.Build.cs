// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeImmersion : ModuleRules
{
	public ApartmentLifeImmersion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "Json", "JsonUtilities",
			"ApartmentLifeCore", "ApartmentLifeCamera"
		});

		if (Target.bBuildDeveloperTools || Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("AutomationTest");
		}
	}
}

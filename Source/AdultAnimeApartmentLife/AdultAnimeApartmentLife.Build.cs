// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class AdultAnimeApartmentLife : ModuleRules
{
	public AdultAnimeApartmentLife(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ApartmentLifeCore",
			"ApartmentLifeCamera",
			"ApartmentLifeCharacter",
			"ApartmentLifeWardrobe",
			"ApartmentLifeApartment",
			"ApartmentLifeAI",
			"ApartmentLifeActivities",
			"ApartmentLifeWorldSim",
			"ApartmentLifeCharacterPipeline",
			"ApartmentLifeSocial"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG"
		});
	}
}

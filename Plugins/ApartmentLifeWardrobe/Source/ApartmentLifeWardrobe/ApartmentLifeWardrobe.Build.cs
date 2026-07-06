// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;

public class ApartmentLifeWardrobe : ModuleRules
{
	public ApartmentLifeWardrobe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"ApartmentLifeCore",
			"ApartmentLifeCharacter"
		});
	}
}

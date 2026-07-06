// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AdultAnimeApartmentLifeTarget : TargetRules
{
	public AdultAnimeApartmentLifeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("AdultAnimeApartmentLife");
	}
}

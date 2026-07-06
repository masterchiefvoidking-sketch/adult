// Copyright Adult Anime Apartment Life. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AdultAnimeApartmentLifeEditorTarget : TargetRules
{
	public AdultAnimeApartmentLifeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("AdultAnimeApartmentLife");
	}
}

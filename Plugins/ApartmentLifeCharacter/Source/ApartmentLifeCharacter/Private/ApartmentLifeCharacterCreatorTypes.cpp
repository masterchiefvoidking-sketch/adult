// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorTypes.h"

TArray<FApartmentLifeMorphSlider> FApartmentLifeFaceCustomization::GetAllFaceMorphs() const
{
	TArray<FApartmentLifeMorphSlider> All;
	All.Append(FaceShapeMorphs);
	All.Append(EyeMorphs);
	All.Append(EyebrowMorphs);
	All.Append(NoseMorphs);
	All.Append(LipMorphs);
	All.Append(JawMorphs);
	All.Append(CheekMorphs);
	All.Append(EarMorphs);
	return All;
}

TArray<FApartmentLifeMorphSlider> FApartmentLifeBodyCustomization::ToMorphSliders() const
{
	auto MakeMorph = [](FName Name, float Value)
	{
		FApartmentLifeMorphSlider M;
		M.MorphTargetName = Name;
		M.Value = Value;
		return M;
	};

	return {
		MakeMorph(FName(TEXT("ShoulderWidth")), ShoulderWidth),
		MakeMorph(FName(TEXT("Waist")), Waist),
		MakeMorph(FName(TEXT("Hips")), Hips),
		MakeMorph(FName(TEXT("ArmLength")), ArmLength),
		MakeMorph(FName(TEXT("LegLength")), LegLength),
		MakeMorph(FName(TEXT("TorsoLength")), TorsoLength),
		MakeMorph(FName(TEXT("MuscleTone")), MuscleTone)
	};
}

FVector FApartmentLifeBodyCustomization::GetScaleVector() const
{
	const float HeightScale = HeightCm / 165.f;
	return FVector(HeightScale, HeightScale, HeightScale);
}

FApartmentLifeCharacterAppearanceState FApartmentLifeCharacterCreatorState::ToLegacyAppearanceState() const
{
	FApartmentLifeCharacterAppearanceState Legacy;
	Legacy.HeightCm = Body.HeightCm;
	Legacy.BodyProportions = Body.ToMorphSliders();
	Legacy.FaceMorphs = Face.GetAllFaceMorphs();
	Legacy.SkinTone = SkinTone;
	Legacy.EyeColor = EyeColor;
	Legacy.HairstyleId = HairstyleId;
	Legacy.HairColor = HairColor;
	Legacy.MakeupLayers = Makeup.FaceMakeup;
	Legacy.AccessoryIds = AccessoryIds;
	Legacy.TattooIds = TattooIds;
	Legacy.VoiceProfileId = VoicePresetId;
	return Legacy;
}

void FApartmentLifeCharacterCreatorState::FromLegacyAppearanceState(const FApartmentLifeCharacterAppearanceState& Legacy)
{
	Body.HeightCm = Legacy.HeightCm;
	for (const FApartmentLifeMorphSlider& Morph : Legacy.BodyProportions)
	{
		if (Morph.MorphTargetName == FName(TEXT("ShoulderWidth"))) Body.ShoulderWidth = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("Waist"))) Body.Waist = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("Hips"))) Body.Hips = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("ArmLength"))) Body.ArmLength = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("LegLength"))) Body.LegLength = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("TorsoLength"))) Body.TorsoLength = Morph.Value;
		else if (Morph.MorphTargetName == FName(TEXT("MuscleTone"))) Body.MuscleTone = Morph.Value;
	}

	for (const FApartmentLifeMorphSlider& Morph : Legacy.FaceMorphs)
	{
		auto TryAdd = [&Morph](TArray<FApartmentLifeMorphSlider>& Target)
		{
			for (FApartmentLifeMorphSlider& Existing : Target)
			{
				if (Existing.MorphTargetName == Morph.MorphTargetName)
				{
					Existing.Value = Morph.Value;
					return;
				}
			}
		};
		TryAdd(Face.FaceShapeMorphs);
		TryAdd(Face.EyeMorphs);
		TryAdd(Face.EyebrowMorphs);
		TryAdd(Face.NoseMorphs);
		TryAdd(Face.LipMorphs);
		TryAdd(Face.JawMorphs);
		TryAdd(Face.CheekMorphs);
		TryAdd(Face.EarMorphs);
	}

	SkinTone = Legacy.SkinTone;
	EyeColor = Legacy.EyeColor;
	HairstyleId = Legacy.HairstyleId;
	HairColor = Legacy.HairColor;
	Makeup.FaceMakeup = Legacy.MakeupLayers;
	AccessoryIds = Legacy.AccessoryIds;
	TattooIds = Legacy.TattooIds;
	VoicePresetId = Legacy.VoiceProfileId;
}

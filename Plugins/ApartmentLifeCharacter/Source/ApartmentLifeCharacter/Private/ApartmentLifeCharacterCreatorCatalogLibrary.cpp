// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"

namespace
{
	FApartmentLifeMorphSlider MakeMorph(FName Name, float Value = 0.5f)
	{
		FApartmentLifeMorphSlider Morph;
		Morph.MorphTargetName = Name;
		Morph.Value = Value;
		return Morph;
	}

	void SeedFaceRegion(TArray<FApartmentLifeMorphSlider>& OutMorphs, std::initializer_list<FName> Names)
	{
		for (FName Name : Names)
		{
			OutMorphs.Add(MakeMorph(Name));
		}
	}
}

FApartmentLifeCharacterCreatorState UApartmentLifeCharacterCreatorCatalogLibrary::GetDefaultCreatorState()
{
	FApartmentLifeCharacterCreatorState State;
	SeedFaceRegion(State.Face.FaceShapeMorphs, { TEXT("FaceWidth"), TEXT("FaceLength"), TEXT("ChinShape") });
	SeedFaceRegion(State.Face.EyeMorphs, { TEXT("EyeSize"), TEXT("EyeSpacing"), TEXT("EyeTilt") });
	SeedFaceRegion(State.Face.EyebrowMorphs, { TEXT("BrowHeight"), TEXT("BrowThickness") });
	SeedFaceRegion(State.Face.NoseMorphs, { TEXT("NoseWidth"), TEXT("NoseHeight") });
	SeedFaceRegion(State.Face.LipMorphs, { TEXT("LipFullness"), TEXT("MouthWidth") });
	SeedFaceRegion(State.Face.JawMorphs, { TEXT("JawWidth"), TEXT("JawHeight") });
	SeedFaceRegion(State.Face.CheekMorphs, { TEXT("CheekFullness"), TEXT("Cheekbone") });
	SeedFaceRegion(State.Face.EarMorphs, { TEXT("EarSize") });

	State.SkinTone.Color = FLinearColor(0.92f, 0.78f, 0.68f);
	State.SkinTone.MaterialParameterName = FName(TEXT("SkinTone"));
	State.EyeColor.Color = FLinearColor(0.25f, 0.45f, 0.65f);
	State.EyeColor.MaterialParameterName = FName(TEXT("EyeColor"));
	State.HairColor.Color = FLinearColor(0.12f, 0.08f, 0.06f);
	State.HairColor.MaterialParameterName = FName(TEXT("HairColor"));
	State.HairstyleId = FName(TEXT("hair.long.straight"));
	State.VoicePresetId = FName(TEXT("voice.soft"));
	State.PersonalityPresetId = FName(TEXT("personality.kind"));
	State.FavoriteColors = { FLinearColor(0.9f, 0.5f, 0.6f), FLinearColor(0.4f, 0.6f, 0.9f) };
	return State;
}

TArray<FName> UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinHairstyleIds()
{
	return {
		FName(TEXT("hair.long.straight")),
		FName(TEXT("hair.long.wavy")),
		FName(TEXT("hair.bob.soft")),
		FName(TEXT("hair.ponytail.high")),
		FName(TEXT("hair.bun.casual")),
		FName(TEXT("hair.pixie.soft"))
	};
}

TArray<FName> UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinVoicePresetIds()
{
	return {
		FName(TEXT("voice.soft")),
		FName(TEXT("voice.bright")),
		FName(TEXT("voice.calm")),
		FName(TEXT("voice.playful"))
	};
}

TArray<FName> UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinPersonalityPresetIds()
{
	return {
		FName(TEXT("personality.kind")),
		FName(TEXT("personality.confident")),
		FName(TEXT("personality.shy")),
		FName(TEXT("personality.athletic"))
	};
}

TArray<FApartmentLifeCharacterCreatorPreset> UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinPresets()
{
	TArray<FApartmentLifeCharacterCreatorPreset> Presets;

	auto AddPreset = [&](FName Id, const FString& Name, EApartmentLifeCreatorPresetType Type, const FApartmentLifeCharacterCreatorState& State)
	{
		FApartmentLifeCharacterCreatorPreset Preset;
		Preset.PresetId = Id;
		Preset.DisplayName = FText::FromString(Name);
		Preset.PresetType = Type;
		Preset.CompatibilityVersion = CreatorCompatibilityVersion;
		Preset.CreatorState = State;
		Presets.Add(Preset);
	};

	FApartmentLifeCharacterCreatorState Default = GetDefaultCreatorState();
	AddPreset(FName(TEXT("preset.default")), TEXT("Default Girl"), EApartmentLifeCreatorPresetType::FullCharacter, Default);

	FApartmentLifeCharacterCreatorState Athletic = Default;
	Athletic.Body.MuscleTone = 0.65f;
	Athletic.Body.Posture = EApartmentLifePostureStyle::Athletic;
	Athletic.Body.WalkStyle = EApartmentLifeWalkStyle::Energetic;
	Athletic.Body.IdleStyle = EApartmentLifeIdleStyle::Athletic;
	Athletic.HairstyleId = FName(TEXT("hair.ponytail.high"));
	AddPreset(FName(TEXT("preset.athletic")), TEXT("Athletic"), EApartmentLifeCreatorPresetType::FullCharacter, Athletic);

	FApartmentLifeCharacterCreatorState Soft = Default;
	Soft.Face.FaceShapeMorphs[0].Value = 0.35f;
	Soft.HairColor.Color = FLinearColor(0.35f, 0.22f, 0.15f);
	Soft.HairstyleId = FName(TEXT("hair.long.wavy"));
	Soft.VoicePresetId = FName(TEXT("voice.soft"));
	AddPreset(FName(TEXT("preset.soft")), TEXT("Soft & Gentle"), EApartmentLifeCreatorPresetType::FullCharacter, Soft);

	return Presets;
}

TArray<FApartmentLifeMorphSlider> UApartmentLifeCharacterCreatorCatalogLibrary::GetDefaultFaceMorphsForRegion(EApartmentLifeFaceRegion Region)
{
	FApartmentLifeCharacterCreatorState State = GetDefaultCreatorState();
	switch (Region)
	{
	case EApartmentLifeFaceRegion::FaceShape: return State.Face.FaceShapeMorphs;
	case EApartmentLifeFaceRegion::Eyes: return State.Face.EyeMorphs;
	case EApartmentLifeFaceRegion::Eyebrows: return State.Face.EyebrowMorphs;
	case EApartmentLifeFaceRegion::Nose: return State.Face.NoseMorphs;
	case EApartmentLifeFaceRegion::Lips: return State.Face.LipMorphs;
	case EApartmentLifeFaceRegion::Jaw: return State.Face.JawMorphs;
	case EApartmentLifeFaceRegion::Cheeks: return State.Face.CheekMorphs;
	case EApartmentLifeFaceRegion::Ears: return State.Face.EarMorphs;
	default: return {};
	}
}

FLinearColor UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableSkinTone()
{
	const TArray<FLinearColor> Tones = {
		FLinearColor(0.95f, 0.82f, 0.72f),
		FLinearColor(0.92f, 0.78f, 0.68f),
		FLinearColor(0.85f, 0.68f, 0.55f),
		FLinearColor(0.72f, 0.55f, 0.42f)
	};
	return Tones[FMath::RandRange(0, Tones.Num() - 1)];
}

FLinearColor UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableHairColor()
{
	const TArray<FLinearColor> Colors = {
		FLinearColor(0.08f, 0.06f, 0.05f),
		FLinearColor(0.25f, 0.15f, 0.08f),
		FLinearColor(0.45f, 0.28f, 0.15f),
		FLinearColor(0.15f, 0.12f, 0.18f)
	};
	return Colors[FMath::RandRange(0, Colors.Num() - 1)];
}

FLinearColor UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableEyeColor()
{
	const TArray<FLinearColor> Colors = {
		FLinearColor(0.25f, 0.45f, 0.65f),
		FLinearColor(0.35f, 0.55f, 0.35f),
		FLinearColor(0.45f, 0.32f, 0.22f),
		FLinearColor(0.55f, 0.55f, 0.58f)
	};
	return Colors[FMath::RandRange(0, Colors.Num() - 1)];
}

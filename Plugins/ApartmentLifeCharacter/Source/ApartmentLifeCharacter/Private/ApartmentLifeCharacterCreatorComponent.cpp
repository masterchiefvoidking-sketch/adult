// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeBodyCustomizationComponent.h"
#include "ApartmentLifeCharacterCreatorCatalogData.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "ApartmentLifeCharacterCreatorPresetLibrary.h"
#include "JsonObjectConverter.h"

void UApartmentLifeCharacterCreatorComponent::InitializeFromCatalog(UApartmentLifeMorphCatalogData* Catalog)
{
	MorphCatalog = Catalog;
	if (Catalog)
	{
		ApplyCreatorState(Catalog->DefaultCreatorState);
	}
}

void UApartmentLifeCharacterCreatorComponent::InitializeFromBuiltinCatalog()
{
	ApplyCreatorState(GetDefaultState());
}

FApartmentLifeCharacterCreatorState UApartmentLifeCharacterCreatorComponent::GetDefaultState() const
{
	if (MorphCatalog)
	{
		return MorphCatalog->DefaultCreatorState;
	}
	return UApartmentLifeCharacterCreatorCatalogLibrary::GetDefaultCreatorState();
}

void UApartmentLifeCharacterCreatorComponent::ApplyCreatorState(const FApartmentLifeCharacterCreatorState& NewState)
{
	CreatorState = NewState;
	ApplyFullAppearance();
	SyncBodyComponent();
	OnCreatorStateUpdated.Broadcast(CreatorState);
}

void UApartmentLifeCharacterCreatorComponent::BroadcastAndApply()
{
	ApplyFullAppearance();
	SyncBodyComponent();
	OnCreatorStateUpdated.Broadcast(CreatorState);
}

void UApartmentLifeCharacterCreatorComponent::SetFaceMorph(EApartmentLifeFaceRegion Region, FName MorphName, float Value)
{
	auto SetInArray = [MorphName, Value](TArray<FApartmentLifeMorphSlider>& Morphs)
	{
		for (FApartmentLifeMorphSlider& M : Morphs)
		{
			if (M.MorphTargetName == MorphName)
			{
				M.Value = FMath::Clamp(Value, 0.f, 1.f);
				return;
			}
		}
		FApartmentLifeMorphSlider NewMorph;
		NewMorph.MorphTargetName = MorphName;
		NewMorph.Value = FMath::Clamp(Value, 0.f, 1.f);
		Morphs.Add(NewMorph);
	};

	switch (Region)
	{
	case EApartmentLifeFaceRegion::FaceShape: SetInArray(CreatorState.Face.FaceShapeMorphs); break;
	case EApartmentLifeFaceRegion::Eyes: SetInArray(CreatorState.Face.EyeMorphs); break;
	case EApartmentLifeFaceRegion::Eyebrows: SetInArray(CreatorState.Face.EyebrowMorphs); break;
	case EApartmentLifeFaceRegion::Nose: SetInArray(CreatorState.Face.NoseMorphs); break;
	case EApartmentLifeFaceRegion::Lips: SetInArray(CreatorState.Face.LipMorphs); break;
	case EApartmentLifeFaceRegion::Jaw: SetInArray(CreatorState.Face.JawMorphs); break;
	case EApartmentLifeFaceRegion::Cheeks: SetInArray(CreatorState.Face.CheekMorphs); break;
	case EApartmentLifeFaceRegion::Ears: SetInArray(CreatorState.Face.EarMorphs); break;
	}

	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetBodySlider(FName MorphName, float Value)
{
	const float Clamped = FMath::Clamp(Value, 0.f, 1.f);
	if (MorphName == FName(TEXT("HeightCm")))
	{
		CreatorState.Body.HeightCm = FMath::Clamp(Value, 140.f, 200.f);
	}
	else if (MorphName == FName(TEXT("ShoulderWidth"))) CreatorState.Body.ShoulderWidth = Clamped;
	else if (MorphName == FName(TEXT("Waist"))) CreatorState.Body.Waist = Clamped;
	else if (MorphName == FName(TEXT("Hips"))) CreatorState.Body.Hips = Clamped;
	else if (MorphName == FName(TEXT("ArmLength"))) CreatorState.Body.ArmLength = Clamped;
	else if (MorphName == FName(TEXT("LegLength"))) CreatorState.Body.LegLength = Clamped;
	else if (MorphName == FName(TEXT("TorsoLength"))) CreatorState.Body.TorsoLength = Clamped;
	else if (MorphName == FName(TEXT("MuscleTone"))) CreatorState.Body.MuscleTone = Clamped;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetHairstyle(FName HairstyleId)
{
	CreatorState.HairstyleId = HairstyleId;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetVoicePreset(FName VoicePresetId)
{
	CreatorState.VoicePresetId = VoicePresetId;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetPersonalityPreset(FName PersonalityPresetId)
{
	CreatorState.PersonalityPresetId = PersonalityPresetId;
	if (PersonalityPresetId == FName(TEXT("personality.confident")))
	{
		CreatorState.Body.WalkStyle = EApartmentLifeWalkStyle::Confident;
		CreatorState.Body.IdleStyle = EApartmentLifeIdleStyle::Composed;
	}
	else if (PersonalityPresetId == FName(TEXT("personality.shy")))
	{
		CreatorState.Body.WalkStyle = EApartmentLifeWalkStyle::Reserved;
		CreatorState.Body.IdleStyle = EApartmentLifeIdleStyle::Shy;
	}
	else if (PersonalityPresetId == FName(TEXT("personality.athletic")))
	{
		CreatorState.Body.WalkStyle = EApartmentLifeWalkStyle::Energetic;
		CreatorState.Body.IdleStyle = EApartmentLifeIdleStyle::Athletic;
		CreatorState.Body.Posture = EApartmentLifePostureStyle::Athletic;
	}
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetSkinTone(const FLinearColor& Color)
{
	CreatorState.SkinTone.Color = Color;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetHairColor(const FLinearColor& Color)
{
	CreatorState.HairColor.Color = Color;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetEyeColor(const FLinearColor& Color)
{
	CreatorState.EyeColor.Color = Color;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::SetMakeupIntensity(float Intensity)
{
	CreatorState.Makeup.MakeupIntensity = FMath::Clamp(Intensity, 0.f, 1.f);
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeFace()
{
	auto RandomizeArray = [](TArray<FApartmentLifeMorphSlider>& Morphs)
	{
		for (FApartmentLifeMorphSlider& M : Morphs)
		{
			M.Value = FMath::FRandRange(0.25f, 0.75f);
		}
	};

	RandomizeArray(CreatorState.Face.FaceShapeMorphs);
	RandomizeArray(CreatorState.Face.EyeMorphs);
	RandomizeArray(CreatorState.Face.EyebrowMorphs);
	RandomizeArray(CreatorState.Face.NoseMorphs);
	RandomizeArray(CreatorState.Face.LipMorphs);
	RandomizeArray(CreatorState.Face.JawMorphs);
	RandomizeArray(CreatorState.Face.CheekMorphs);
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeBody()
{
	CreatorState.Body.HeightCm = FMath::FRandRange(155.f, 178.f);
	CreatorState.Body.ShoulderWidth = FMath::FRandRange(0.3f, 0.7f);
	CreatorState.Body.Waist = FMath::FRandRange(0.3f, 0.7f);
	CreatorState.Body.Hips = FMath::FRandRange(0.35f, 0.75f);
	CreatorState.Body.ArmLength = FMath::FRandRange(0.35f, 0.65f);
	CreatorState.Body.LegLength = FMath::FRandRange(0.35f, 0.65f);
	CreatorState.Body.TorsoLength = FMath::FRandRange(0.35f, 0.65f);
	CreatorState.Body.MuscleTone = FMath::FRandRange(0.15f, 0.55f);
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeHair()
{
	const TArray<FName> Hairstyles = UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinHairstyleIds();
	if (Hairstyles.Num() > 0)
	{
		CreatorState.HairstyleId = Hairstyles[FMath::RandRange(0, Hairstyles.Num() - 1)];
	}
	CreatorState.HairColor.Color = UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableHairColor();
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeColors()
{
	CreatorState.SkinTone.Color = UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableSkinTone();
	CreatorState.EyeColor.Color = UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableEyeColor();
	CreatorState.HairColor.Color = UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableHairColor();
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeAppearance(EApartmentLifeCreatorRandomizeScope Scope)
{
	switch (Scope)
	{
	case EApartmentLifeCreatorRandomizeScope::FaceOnly: RandomizeFace(); break;
	case EApartmentLifeCreatorRandomizeScope::BodyOnly: RandomizeBody(); break;
	case EApartmentLifeCreatorRandomizeScope::HairOnly: RandomizeHair(); break;
	case EApartmentLifeCreatorRandomizeScope::ColorsOnly: RandomizeColors(); break;
	default:
		RandomizeFace();
		RandomizeBody();
		RandomizeHair();
		RandomizeColors();
		break;
	}
}

void UApartmentLifeCharacterCreatorComponent::ResetFace()
{
	const FApartmentLifeCharacterCreatorState Default = GetDefaultState();
	CreatorState.Face = Default.Face;
	CreatorState.SkinTone = Default.SkinTone;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::ResetBody()
{
	const FApartmentLifeCharacterCreatorState Default = GetDefaultState();
	CreatorState.Body = Default.Body;
	BroadcastAndApply();
}

void UApartmentLifeCharacterCreatorComponent::ResetAppearance()
{
	ApplyCreatorState(GetDefaultState());
}

bool UApartmentLifeCharacterCreatorComponent::ApplyPreset(const FApartmentLifeCharacterCreatorPreset& Preset)
{
	FApartmentLifeCharacterCreatorState NewState = CreatorState;
	if (!UApartmentLifeCharacterCreatorPresetLibrary::ApplyPresetToState(Preset, NewState))
	{
		return false;
	}
	ApplyCreatorState(NewState);
	return true;
}

bool UApartmentLifeCharacterCreatorComponent::SaveCurrentAsPreset(
	FName PresetId,
	const FText& DisplayName,
	EApartmentLifeCreatorPresetType PresetType)
{
	FApartmentLifeCharacterCreatorPreset Preset;
	Preset.PresetId = PresetId;
	Preset.DisplayName = DisplayName;
	Preset.PresetType = PresetType;
	Preset.CompatibilityVersion = UApartmentLifeCharacterCreatorCatalogLibrary::CreatorCompatibilityVersion;
	Preset.CreatorState = CreatorState;
	return UApartmentLifeCharacterCreatorPresetLibrary::SavePresetToDisk(Preset);
}

FString UApartmentLifeCharacterCreatorComponent::ExportCreatorStateJson() const
{
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(CreatorState, Json);
	return Json;
}

void UApartmentLifeCharacterCreatorComponent::SyncBodyComponent()
{
	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeBodyCustomizationComponent* Body = Owner->FindComponentByClass<UApartmentLifeBodyCustomizationComponent>())
		{
			Body->ApplyBodyCustomization(CreatorState.Body);
		}
	}
}

void UApartmentLifeCharacterCreatorComponent::ApplyFullAppearance()
{
	ApplyAppearanceState(CreatorState.ToLegacyAppearanceState());
}

void UApartmentLifeCharacterCreatorComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	Super::CaptureSaveData_Implementation(OutData);
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(CreatorState, Json);
	OutData.Add(TEXT("CreatorState"), Json);
}

void UApartmentLifeCharacterCreatorComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	Super::RestoreSaveData_Implementation(InData);
	if (const FString* Json = InData.Find(TEXT("CreatorState")))
	{
		FApartmentLifeCharacterCreatorState Restored;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &Restored))
		{
			ApplyCreatorState(Restored);
		}
	}
}

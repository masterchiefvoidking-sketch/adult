// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorUiController.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "ApartmentLifeCharacterCreatorPresetLibrary.h"
#include "ApartmentLifeCharacterCreatorLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeCameraTypes.h"

void UApartmentLifeCharacterCreatorUiController::InitializeContext(
	UApartmentLifeCharacterCreatorComponent* InCreator,
	UApartmentLifeWardrobeComponent* InWardrobe,
	UApartmentLifeAnimationComponent* InAnimation,
	AApartmentLifeCameraPawn* InCameraPawn)
{
	Creator = InCreator;
	Wardrobe = InWardrobe;
	Animation = InAnimation;
	CameraPawn = InCameraPawn;
	PreviewAnimationGroups = UApartmentLifeCharacterCreatorLibrary::GetCreatorPreviewAnimationGroups();
	VisiblePresets = UApartmentLifeCharacterCreatorPresetLibrary::GetAllPresets(true);
}

void UApartmentLifeCharacterCreatorUiController::OpenCreator(bool bEditExisting)
{
	if (!Creator.IsValid())
	{
		return;
	}

	if (!bIsOpen)
	{
		SessionBackup = Creator->GetCreatorState();
	}

	bIsOpen = true;
	if (AActor* Owner = Creator->GetOwner())
	{
		if (CameraPawn.IsValid())
		{
			CameraPawn->EnterCharacterCreatorMode(Owner);
			CameraPawn->SetCreatorLightingMode(static_cast<uint8>(LightingMode));
		}
	}

	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::CloseCreator(bool bConfirm)
{
	if (!bIsOpen)
	{
		return;
	}

	if (!bConfirm && Creator.IsValid())
	{
		Creator->ApplyCreatorState(SessionBackup);
	}

	bIsOpen = false;
	if (CameraPawn.IsValid())
	{
		CameraPawn->ExitCharacterCreatorMode();
	}
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::ToggleCreator()
{
	if (bIsOpen)
	{
		CloseCreator(true);
	}
	else
	{
		OpenCreator(true);
	}
}

void UApartmentLifeCharacterCreatorUiController::SetCategoryTab(EApartmentLifeCreatorCategoryTab Tab)
{
	ActiveTab = Tab;
	SelectedEntryIndex = 0;
	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::SelectEntryIndex(int32 Index)
{
	SelectedEntryIndex = VisibleEntryIds.IsValidIndex(Index) ? Index : INDEX_NONE;
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::ActivateSelectedEntry()
{
	if (!Creator.IsValid() || !VisibleEntryIds.IsValidIndex(SelectedEntryIndex))
	{
		return;
	}

	const FName EntryId = VisibleEntryIds[SelectedEntryIndex];
	switch (ActiveTab)
	{
	case EApartmentLifeCreatorCategoryTab::Hair:
		Creator->SetHairstyle(EntryId);
		break;
	case EApartmentLifeCreatorCategoryTab::Presets:
		ApplyPresetByIndex(SelectedEntryIndex);
		return;
	case EApartmentLifeCreatorCategoryTab::Preview:
		PreviewAnimationByIndex(SelectedEntryIndex);
		return;
	case EApartmentLifeCreatorCategoryTab::Voice:
		if (EntryId.ToString().StartsWith(TEXT("voice.")))
		{
			Creator->SetVoicePreset(EntryId);
		}
		else
		{
			Creator->SetPersonalityPreset(EntryId);
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Colors:
		if (EntryId == FName(TEXT("SkinTone")))
		{
			Creator->SetSkinTone(UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableSkinTone());
		}
		else if (EntryId == FName(TEXT("HairColor")))
		{
			Creator->SetHairColor(UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableHairColor());
		}
		else if (EntryId == FName(TEXT("EyeColor")))
		{
			Creator->SetEyeColor(UApartmentLifeCharacterCreatorCatalogLibrary::GetRandomBelievableEyeColor());
		}
		else
		{
			Creator->RandomizeColors();
		}
		break;
	default:
		break;
	}

	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::AdjustSelectedSlider(float Delta)
{
	if (!Creator.IsValid() || !VisibleEntryIds.IsValidIndex(SelectedEntryIndex))
	{
		return;
	}

	const FName EntryId = VisibleEntryIds[SelectedEntryIndex];
	switch (ActiveTab)
	{
	case EApartmentLifeCreatorCategoryTab::Face:
		{
			const FApartmentLifeCharacterCreatorState State = Creator->GetCreatorState();
			for (const FApartmentLifeMorphSlider& Morph : State.Face.GetAllFaceMorphs())
			{
				if (Morph.MorphTargetName == EntryId)
				{
					Creator->SetFaceMorph(EApartmentLifeFaceRegion::FaceShape, EntryId, Morph.Value + Delta);
					break;
				}
			}
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Body:
		{
			const FApartmentLifeCharacterCreatorState State = Creator->GetCreatorState();
			const FApartmentLifeBodyCustomization& Body = State.Body;
			float Current = 0.5f;
			float AdjustDelta = Delta;
			if (EntryId == FName(TEXT("HeightCm")))
			{
				Current = Body.HeightCm;
				AdjustDelta = Delta * 40.f;
			}
			else if (EntryId == FName(TEXT("ShoulderWidth"))) Current = Body.ShoulderWidth;
			else if (EntryId == FName(TEXT("Waist"))) Current = Body.Waist;
			else if (EntryId == FName(TEXT("Hips"))) Current = Body.Hips;
			else if (EntryId == FName(TEXT("ArmLength"))) Current = Body.ArmLength;
			else if (EntryId == FName(TEXT("LegLength"))) Current = Body.LegLength;
			else if (EntryId == FName(TEXT("TorsoLength"))) Current = Body.TorsoLength;
			else if (EntryId == FName(TEXT("MuscleTone"))) Current = Body.MuscleTone;
			Creator->SetBodySlider(EntryId, Current + AdjustDelta);
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Makeup:
		if (EntryId == FName(TEXT("MakeupIntensity")))
		{
			const float Current = Creator->GetCreatorState().Makeup.MakeupIntensity;
			Creator->SetMakeupIntensity(FMath::Clamp(Current + Delta, 0.f, 1.f));
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Hair:
		Creator->SetHairstyle(EntryId);
		break;
	case EApartmentLifeCreatorCategoryTab::Voice:
		if (EntryId.ToString().StartsWith(TEXT("voice.")))
		{
			Creator->SetVoicePreset(EntryId);
		}
		else
		{
			Creator->SetPersonalityPreset(EntryId);
		}
		break;
	default:
		break;
	}

	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::Randomize(EApartmentLifeCreatorRandomizeScope Scope)
{
	if (Creator.IsValid())
	{
		Creator->RandomizeAppearance(Scope);
		RefreshEntryList();
		BroadcastStateChanged();
	}
}

void UApartmentLifeCharacterCreatorUiController::ResetCurrentCategory()
{
	if (!Creator.IsValid())
	{
		return;
	}

	switch (ActiveTab)
	{
	case EApartmentLifeCreatorCategoryTab::Face: Creator->ResetFace(); break;
	case EApartmentLifeCreatorCategoryTab::Body: Creator->ResetBody(); break;
	default: Creator->ResetAppearance(); break;
	}
	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::ApplyPresetByIndex(int32 Index)
{
	if (!Creator.IsValid() || !VisiblePresets.IsValidIndex(Index))
	{
		return;
	}

	Creator->ApplyPreset(VisiblePresets[Index]);
	RefreshEntryList();
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::SavePresetByIndex(int32 Index)
{
	if (!Creator.IsValid() || !VisiblePresets.IsValidIndex(Index))
	{
		return;
	}

	const FApartmentLifeCharacterCreatorPreset& Template = VisiblePresets[Index];
	const FName PresetId = FName(*FString::Printf(TEXT("preset.user.%d"), FMath::RandRange(1000, 9999)));
	Creator->SaveCurrentAsPreset(PresetId, Template.DisplayName, Template.PresetType);
	VisiblePresets = UApartmentLifeCharacterCreatorPresetLibrary::GetAllPresets(true);
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::SetLightingMode(EApartmentLifeCreatorLightingMode Mode)
{
	LightingMode = Mode;
	if (CameraPawn.IsValid())
	{
		CameraPawn->SetCreatorLightingMode(static_cast<uint8>(Mode));
	}
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::SetCameraFocus(FName FocusMode)
{
	if (!CameraPawn.IsValid() || !Creator.IsValid())
	{
		return;
	}

	AActor* Owner = Creator->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (FocusMode == FName(TEXT("Face")))
	{
		CameraPawn->FocusCharacter(Owner, EApartmentLifeCharacterFocusMode::Face);
	}
	else if (FocusMode == FName(TEXT("UpperBody")))
	{
		CameraPawn->FocusCharacter(Owner, EApartmentLifeCharacterFocusMode::UpperBody);
	}
	else if (FocusMode == FName(TEXT("Outfit")))
	{
		CameraPawn->FocusCharacter(Owner, EApartmentLifeCharacterFocusMode::Outfit);
	}
	else if (FocusMode == FName(TEXT("Hair")))
	{
		CameraPawn->FocusCharacter(Owner, EApartmentLifeCharacterFocusMode::Hair);
	}
	else
	{
		CameraPawn->FocusCharacter(Owner, EApartmentLifeCharacterFocusMode::FullBody);
	}
}

void UApartmentLifeCharacterCreatorUiController::PreviewAnimationByIndex(int32 Index)
{
	if (!Creator.IsValid() || !PreviewAnimationGroups.IsValidIndex(Index))
	{
		return;
	}

	UApartmentLifeCharacterCreatorLibrary::PreviewAnimationGroup(Creator->GetOwner(), PreviewAnimationGroups[Index]);
	if (CameraPawn.IsValid())
	{
		CameraPawn->FocusCharacter(Creator->GetOwner(), EApartmentLifeCharacterFocusMode::PosePreview);
	}
	BroadcastStateChanged();
}

void UApartmentLifeCharacterCreatorUiController::ConfirmCharacter()
{
	SessionBackup = Creator.IsValid() ? Creator->GetCreatorState() : FApartmentLifeCharacterCreatorState();
	CloseCreator(true);
}

FApartmentLifeCreatorCompatibilityReport UApartmentLifeCharacterCreatorUiController::GetCompatibilityReport() const
{
	return Creator.IsValid()
		? UApartmentLifeCharacterCreatorLibrary::EvaluateWardrobeCompatibility(Creator->GetOwner())
		: FApartmentLifeCreatorCompatibilityReport();
}

void UApartmentLifeCharacterCreatorUiController::RefreshEntryList()
{
	RebuildListForActiveTab();
}

void UApartmentLifeCharacterCreatorUiController::BroadcastStateChanged()
{
	OnCreatorUiStateChanged.Broadcast();
}

void UApartmentLifeCharacterCreatorUiController::RebuildListForActiveTab()
{
	VisibleEntryLabels.Reset();
	VisibleEntryDetails.Reset();
	VisibleEntryIds.Reset();

	if (!Creator.IsValid())
	{
		return;
	}

	const FApartmentLifeCharacterCreatorState State = Creator->GetCreatorState();
	auto AddMorphEntries = [this](const TArray<FApartmentLifeMorphSlider>& Morphs)
	{
		for (const FApartmentLifeMorphSlider& Morph : Morphs)
		{
			VisibleEntryIds.Add(Morph.MorphTargetName);
			VisibleEntryLabels.Add(FText::FromName(Morph.MorphTargetName));
			VisibleEntryDetails.Add(FText::FromString(FString::Printf(TEXT("%.0f%%"), Morph.Value * 100.f)));
		}
	};

	switch (ActiveTab)
	{
	case EApartmentLifeCreatorCategoryTab::Face:
		AddMorphEntries(State.Face.GetAllFaceMorphs());
		break;
	case EApartmentLifeCreatorCategoryTab::Body:
		VisibleEntryIds = { TEXT("HeightCm"), TEXT("ShoulderWidth"), TEXT("Waist"), TEXT("Hips"), TEXT("ArmLength"), TEXT("LegLength"), TEXT("TorsoLength"), TEXT("MuscleTone") };
		for (const FName& Id : VisibleEntryIds)
		{
			VisibleEntryLabels.Add(FText::FromName(Id));
			float Value = 0.5f;
			if (Id == FName(TEXT("HeightCm"))) Value = State.Body.HeightCm;
			else if (Id == FName(TEXT("ShoulderWidth"))) Value = State.Body.ShoulderWidth;
			else if (Id == FName(TEXT("Waist"))) Value = State.Body.Waist;
			else if (Id == FName(TEXT("Hips"))) Value = State.Body.Hips;
			else if (Id == FName(TEXT("ArmLength"))) Value = State.Body.ArmLength;
			else if (Id == FName(TEXT("LegLength"))) Value = State.Body.LegLength;
			else if (Id == FName(TEXT("TorsoLength"))) Value = State.Body.TorsoLength;
			else if (Id == FName(TEXT("MuscleTone"))) Value = State.Body.MuscleTone;
			VisibleEntryDetails.Add(FText::FromString(FString::Printf(TEXT("%.0f"), Id == FName(TEXT("HeightCm")) ? Value : Value * 100.f)));
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Hair:
		for (const FName& HairId : UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinHairstyleIds())
		{
			VisibleEntryIds.Add(HairId);
			VisibleEntryLabels.Add(FText::FromName(HairId));
			VisibleEntryDetails.Add(FText::FromString(State.HairstyleId == HairId ? TEXT("Equipped") : TEXT("")));
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Colors:
		VisibleEntryIds = { TEXT("SkinTone"), TEXT("HairColor"), TEXT("EyeColor") };
		VisibleEntryLabels = { FText::FromString(TEXT("Skin Tone")), FText::FromString(TEXT("Hair Color")), FText::FromString(TEXT("Eye Color")) };
		VisibleEntryDetails = {
			FText::FromString(State.SkinTone.Color.ToString()),
			FText::FromString(State.HairColor.Color.ToString()),
			FText::FromString(State.EyeColor.Color.ToString())
		};
		break;
	case EApartmentLifeCreatorCategoryTab::Makeup:
		VisibleEntryIds = { TEXT("MakeupIntensity"), TEXT("LipColor"), TEXT("NailColor") };
		VisibleEntryLabels = {
			FText::FromString(TEXT("Makeup Intensity")),
			FText::FromString(TEXT("Lip Color")),
			FText::FromString(TEXT("Nail Color"))
		};
		VisibleEntryDetails = {
			FText::FromString(FString::Printf(TEXT("%.0f%%"), State.Makeup.MakeupIntensity * 100.f)),
			FText::FromString(State.Makeup.LipColor.Color.ToString()),
			FText::FromString(State.Makeup.NailColor.Color.ToString())
		};
		break;
	case EApartmentLifeCreatorCategoryTab::Accessories:
		VisibleEntryIds = { TEXT("Glasses"), TEXT("Jewelry"), TEXT("Accessories") };
		VisibleEntryLabels = {
			FText::FromString(TEXT("Glasses")),
			FText::FromString(TEXT("Jewelry")),
			FText::FromString(TEXT("Accessories"))
		};
		VisibleEntryDetails = {
			FText::FromString(FString::Printf(TEXT("%d equipped"), State.GlassesIds.Num())),
			FText::FromString(FString::Printf(TEXT("%d equipped"), State.JewelryIds.Num())),
			FText::FromString(FString::Printf(TEXT("%d equipped"), State.AccessoryIds.Num()))
		};
		break;
	case EApartmentLifeCreatorCategoryTab::Voice:
		for (const FName& VoiceId : UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinVoicePresetIds())
		{
			VisibleEntryIds.Add(VoiceId);
			VisibleEntryLabels.Add(FText::FromName(VoiceId));
			VisibleEntryDetails.Add(FText::FromString(State.VoicePresetId == VoiceId ? TEXT("Selected") : TEXT("")));
		}
		for (const FName& PersonalityId : UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinPersonalityPresetIds())
		{
			VisibleEntryIds.Add(PersonalityId);
			VisibleEntryLabels.Add(FText::FromName(PersonalityId));
			VisibleEntryDetails.Add(FText::FromString(State.PersonalityPresetId == PersonalityId ? TEXT("Selected") : TEXT("")));
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Presets:
		VisiblePresets = UApartmentLifeCharacterCreatorPresetLibrary::GetAllPresets(true);
		for (const FApartmentLifeCharacterCreatorPreset& Preset : VisiblePresets)
		{
			VisibleEntryIds.Add(Preset.PresetId);
			VisibleEntryLabels.Add(Preset.DisplayName);
			VisibleEntryDetails.Add(FText::FromString(FString::Printf(TEXT("v%d"), Preset.CompatibilityVersion)));
		}
		break;
	case EApartmentLifeCreatorCategoryTab::Preview:
		for (const FName& Group : PreviewAnimationGroups)
		{
			VisibleEntryIds.Add(Group);
			VisibleEntryLabels.Add(FText::FromName(Group));
			VisibleEntryDetails.Add(FText::FromString(TEXT("Preview animation")));
		}
		break;
	default:
		break;
	}

	if (!VisibleEntryIds.IsValidIndex(SelectedEntryIndex))
	{
		SelectedEntryIndex = VisibleEntryIds.Num() > 0 ? 0 : INDEX_NONE;
	}
}

FApartmentLifeCharacterCreatorState UApartmentLifeCharacterCreatorUiController::GetMutableStateCopy() const
{
	return Creator.IsValid() ? Creator->GetCreatorState() : FApartmentLifeCharacterCreatorState();
}

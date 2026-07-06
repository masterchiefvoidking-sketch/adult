// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorPresetLibrary.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

namespace
{
	FString GetPresetDirectory()
	{
		return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("CharacterPresets"));
	}

	FString GetPresetFilePath(FName PresetId)
	{
		return FPaths::Combine(GetPresetDirectory(), PresetId.ToString() + TEXT(".json"));
	}
}

bool UApartmentLifeCharacterCreatorPresetLibrary::SavePresetToDisk(const FApartmentLifeCharacterCreatorPreset& Preset)
{
	if (Preset.PresetId.IsNone())
	{
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString Directory = GetPresetDirectory();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectoryTree(*Directory);
	}

	FString Json;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Preset, Json))
	{
		return false;
	}

	return FFileHelper::SaveStringToFile(Json, *GetPresetFilePath(Preset.PresetId));
}

bool UApartmentLifeCharacterCreatorPresetLibrary::LoadPresetFromDisk(FName PresetId, FApartmentLifeCharacterCreatorPreset& OutPreset)
{
	FString Json;
	if (!FFileHelper::LoadFileToString(Json, *GetPresetFilePath(PresetId)))
	{
		return false;
	}

	return FJsonObjectConverter::JsonObjectStringToUStruct(Json, &OutPreset);
}

bool UApartmentLifeCharacterCreatorPresetLibrary::DeletePresetFromDisk(FName PresetId)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.DeleteFile(*GetPresetFilePath(PresetId));
}

TArray<FApartmentLifeCharacterCreatorPreset> UApartmentLifeCharacterCreatorPresetLibrary::GetAllPresets(bool bIncludeBuiltin)
{
	TArray<FApartmentLifeCharacterCreatorPreset> Presets;
	if (bIncludeBuiltin)
	{
		Presets.Append(UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinPresets());
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString Directory = GetPresetDirectory();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		return Presets;
	}

	TArray<FString> Files;
	PlatformFile.FindFiles(Files, *Directory, TEXT("json"));
	for (const FString& File : Files)
	{
		FString Json;
		if (FFileHelper::LoadFileToString(Json, *File))
		{
			FApartmentLifeCharacterCreatorPreset Preset;
			if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Preset))
			{
				Presets.Add(Preset);
			}
		}
	}

	return Presets;
}

bool UApartmentLifeCharacterCreatorPresetLibrary::ApplyPresetToState(
	const FApartmentLifeCharacterCreatorPreset& Preset,
	FApartmentLifeCharacterCreatorState& InOutState,
	EApartmentLifeCreatorPresetType OverrideType)
{
	const EApartmentLifeCreatorPresetType Type = OverrideType == EApartmentLifeCreatorPresetType::FullCharacter
		? Preset.PresetType : OverrideType;

	switch (Type)
	{
	case EApartmentLifeCreatorPresetType::Face:
		InOutState.Face = Preset.CreatorState.Face;
		InOutState.SkinTone = Preset.CreatorState.SkinTone;
		break;
	case EApartmentLifeCreatorPresetType::Body:
		InOutState.Body = Preset.CreatorState.Body;
		break;
	case EApartmentLifeCreatorPresetType::Hair:
		InOutState.HairstyleId = Preset.CreatorState.HairstyleId;
		InOutState.HairColor = Preset.CreatorState.HairColor;
		break;
	case EApartmentLifeCreatorPresetType::Makeup:
		InOutState.Makeup = Preset.CreatorState.Makeup;
		break;
	case EApartmentLifeCreatorPresetType::Outfit:
		InOutState.AccessoryIds = Preset.CreatorState.AccessoryIds;
		InOutState.GlassesIds = Preset.CreatorState.GlassesIds;
		InOutState.JewelryIds = Preset.CreatorState.JewelryIds;
		break;
	default:
		InOutState = Preset.CreatorState;
		break;
	}

	return true;
}

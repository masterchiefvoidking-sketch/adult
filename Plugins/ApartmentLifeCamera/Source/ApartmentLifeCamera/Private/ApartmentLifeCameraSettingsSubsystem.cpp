// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCameraSettingsSubsystem.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "JsonObjectConverter.h"

void UApartmentLifeCameraSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FApartmentLifeSaveableRegistry::Register(this);
}

void UApartmentLifeCameraSettingsSubsystem::Deinitialize()
{
	FApartmentLifeSaveableRegistry::Unregister(this);
	Super::Deinitialize();
}

void UApartmentLifeCameraSettingsSubsystem::ApplySettingsToPawn(AApartmentLifeCameraPawn* CameraPawn) const
{
	if (CameraPawn)
	{
		CameraPawn->ApplyUserSettings(Settings);
	}
}

void UApartmentLifeCameraSettingsSubsystem::CaptureFromPawn(const AApartmentLifeCameraPawn* CameraPawn)
{
	if (CameraPawn)
	{
		Settings = CameraPawn->BuildUserSettings();
	}
}

FString UApartmentLifeCameraSettingsSubsystem::GetSaveId_Implementation() const
{
	return TEXT("camera_user_settings");
}

void UApartmentLifeCameraSettingsSubsystem::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(Settings, Json);
	OutData.Add(TEXT("Settings"), Json);
}

void UApartmentLifeCameraSettingsSubsystem::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Json = InData.Find(TEXT("Settings")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &Settings);
	}
}

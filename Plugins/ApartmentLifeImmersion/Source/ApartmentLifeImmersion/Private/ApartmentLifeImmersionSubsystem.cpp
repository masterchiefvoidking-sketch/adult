// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeImmersionSubsystem.h"
#include "ApartmentLifeImmersionLibrary.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "JsonObjectConverter.h"
#include "GameFramework/GameUserSettings.h"

void UApartmentLifeImmersionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FApartmentLifeSaveableRegistry::Register(this);
}

void UApartmentLifeImmersionSubsystem::Deinitialize()
{
	FApartmentLifeSaveableRegistry::Unregister(this);
	Super::Deinitialize();
}

void UApartmentLifeImmersionSubsystem::ApplyUserSettings(const FApartmentLifeImmersionUserSettings& Settings)
{
	UserSettings = Settings;
	ApplyGraphicsSettings();
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::SetRoomAmbience(EApartmentLifeRoomType Room)
{
	State.ActiveRoom = Room;
	const FApartmentLifeRoomAmbienceProfile Profile = UApartmentLifeImmersionLibrary::GetRoomAmbienceProfile(Room);
	State.ActiveRoomToneId = Profile.RoomToneId;
	State.ActiveAmbienceLayers = Profile.LayerIds;
	if (Room == EApartmentLifeRoomType::Bedroom)
	{
		SetMusicCategory(EApartmentLifeMusicCategory::Relaxation, true);
	}
	else if (Room == EApartmentLifeRoomType::Office)
	{
		SetMusicCategory(EApartmentLifeMusicCategory::Work, true);
	}
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::SetMusicCategory(EApartmentLifeMusicCategory Category, bool bCrossfade)
{
	if (bCrossfade && State.ActiveMusicCategory != Category)
	{
		PendingMusicCategory = Category;
		MusicCrossfadeTimer = 0.f;
		State.MusicCrossfadeAlpha = 0.f;
	}
	else
	{
		State.ActiveMusicCategory = Category;
		State.MusicCrossfadeAlpha = 1.f;
	}

	State.ActiveMusicId = UApartmentLifeImmersionLibrary::GetMusicIdForCategory(Category);
	OnMusicCategoryChanged.Broadcast(Category, State.ActiveMusicId);
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::UpdateFromGameTime(int32 Hour, EApartmentLifeWeather Weather)
{
	CachedHour = Hour;
	CachedWeather = Weather;
	State.ActiveLightingPreset = UApartmentLifeImmersionLibrary::GetLightingPresetForTimeAndWeather(Hour, Weather);
	State.LightingValues = UApartmentLifeImmersionLibrary::GetLightingValuesForPreset(State.ActiveLightingPreset);
	State.ActiveWeatherLayers = UApartmentLifeImmersionLibrary::GetWeatherAmbienceLayers(Weather, Hour);
	SetMusicCategory(UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(Hour), true);
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::HandleActivityStarted(FName ActivityId, EApartmentLifeRoomType Room)
{
	SetRoomAmbience(Room);
	const EApartmentLifeMusicCategory ActivityMusic = UApartmentLifeImmersionLibrary::GetMusicCategoryForActivity(ActivityId);
	SetMusicCategory(ActivityMusic, true);

	State.LastActivitySfxId = UApartmentLifeImmersionLibrary::GetActivitySfxId(ActivityId);
	OnActivitySfxTriggered.Broadcast(State.LastActivitySfxId);

	if (ActivityId.ToString().Contains(TEXT("yoga")))
	{
		State.ActiveAmbienceLayers = {
			FName(TEXT("audio.ambience.yoga.calm")),
			FName(TEXT("audio.ambience.nature.soft")),
			FName(TEXT("audio.ambience.breathing.guide"))
		};
		SetMusicCategory(EApartmentLifeMusicCategory::Yoga, true);
	}

	State.EnvironmentalMotion = UApartmentLifeImmersionLibrary::BuildEnvironmentalMotion(
		Room, ActivityId, CachedWeather, CachedHour);
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::UpdateCrossfade(float DeltaTime)
{
	if (State.MusicCrossfadeAlpha >= 1.f)
	{
		return;
	}

	MusicCrossfadeTimer += DeltaTime;
	State.MusicCrossfadeAlpha = FMath::Clamp(MusicCrossfadeTimer / 2.f, 0.f, 1.f);
	if (State.MusicCrossfadeAlpha >= 1.f)
	{
		State.ActiveMusicCategory = PendingMusicCategory;
		State.ActiveMusicId = UApartmentLifeImmersionLibrary::GetMusicIdForCategory(PendingMusicCategory);
	}
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::PlayMovementSfx(FName SfxId)
{
	State.LastActivitySfxId = SfxId;
	OnActivitySfxTriggered.Broadcast(SfxId);
}

void UApartmentLifeImmersionSubsystem::UpdateMicroAnimation(float Mood, float Energy, float Comfort, bool bIsSitting)
{
	State.ActiveMicroAnimation = UApartmentLifeImmersionLibrary::PickMicroAnimationCue(Mood, Energy, Comfort, bIsSitting);
	BroadcastState();
}

void UApartmentLifeImmersionSubsystem::SavePhotoBookmark(const FApartmentLifePhotoBookmark& Bookmark)
{
	UserSettings.PhotoBookmarks.Add(Bookmark);
	UserSettings.ActivePhotoBookmarkIndex = UserSettings.PhotoBookmarks.Num() - 1;
	BroadcastState();
}

bool UApartmentLifeImmersionSubsystem::ApplyPhotoBookmark(int32 Index)
{
	if (!UserSettings.PhotoBookmarks.IsValidIndex(Index))
	{
		return false;
	}

	UserSettings.ActivePhotoBookmarkIndex = Index;
	const FApartmentLifePhotoBookmark& Bookmark = UserSettings.PhotoBookmarks[Index];
	State.ActiveLightingPreset = Bookmark.LightingPreset;
	State.LightingValues = UApartmentLifeImmersionLibrary::GetLightingValuesForPreset(Bookmark.LightingPreset);
	BroadcastState();
	return true;
}

void UApartmentLifeImmersionSubsystem::ApplyGraphicsSettings()
{
	if (UGameUserSettings* GameSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		switch (UserSettings.Graphics.QualityPreset)
		{
		case EApartmentLifeGraphicsQuality::Low: GameSettings->SetOverallScalabilityLevel(0); break;
		case EApartmentLifeGraphicsQuality::Medium: GameSettings->SetOverallScalabilityLevel(1); break;
		case EApartmentLifeGraphicsQuality::High: GameSettings->SetOverallScalabilityLevel(2); break;
		case EApartmentLifeGraphicsQuality::Ultra: GameSettings->SetOverallScalabilityLevel(3); break;
		}
		GameSettings->SetVSyncEnabled(UserSettings.Graphics.bVSyncEnabled);
		GameSettings->ApplySettings(false);
	}
}

void UApartmentLifeImmersionSubsystem::BroadcastState()
{
	OnImmersionStateChanged.Broadcast(State);
}

void UApartmentLifeImmersionSubsystem::RefreshLighting()
{
	State.LightingValues = UApartmentLifeImmersionLibrary::GetLightingValuesForPreset(State.ActiveLightingPreset);
}

FString UApartmentLifeImmersionSubsystem::GetSaveId_Implementation() const
{
	return TEXT("immersion_user_settings");
}

void UApartmentLifeImmersionSubsystem::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString SettingsJson;
	FJsonObjectConverter::UStructToJsonObjectString(UserSettings, SettingsJson);
	OutData.Add(TEXT("UserSettings"), SettingsJson);

	FString StateJson;
	FJsonObjectConverter::UStructToJsonObjectString(State, StateJson);
	OutData.Add(TEXT("ImmersionState"), StateJson);
}

void UApartmentLifeImmersionSubsystem::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* SettingsJson = InData.Find(TEXT("UserSettings")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*SettingsJson, &UserSettings);
		ApplyGraphicsSettings();
	}
	if (const FString* StateJson = InData.Find(TEXT("ImmersionState")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*StateJson, &State);
	}
	BroadcastState();
}

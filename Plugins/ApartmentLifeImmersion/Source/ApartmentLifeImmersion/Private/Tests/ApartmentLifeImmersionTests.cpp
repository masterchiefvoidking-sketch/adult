// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeImmersionLibrary.h"
#include "ApartmentLifeImmersionSubsystem.h"
#include "ApartmentLifeImmersionTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeRoomAmbienceTest,
	"ApartmentLife.Immersion.RoomAmbience",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeRoomAmbienceTest::RunTest(const FString& Parameters)
{
	const FApartmentLifeRoomAmbienceProfile Bedroom =
		UApartmentLifeImmersionLibrary::GetRoomAmbienceProfile(EApartmentLifeRoomType::Bedroom);
	TestEqual(TEXT("Bedroom tone id"), Bedroom.RoomToneId, FName(TEXT("audio.ambience.bedroom.soft")));
	TestTrue(TEXT("Bedroom has ambience layers"), Bedroom.LayerIds.Num() >= 2);

	const FApartmentLifeRoomAmbienceProfile Bathroom =
		UApartmentLifeImmersionLibrary::GetRoomAmbienceProfile(EApartmentLifeRoomType::Bathroom);
	TestTrue(TEXT("Bathroom has stronger reverb"), Bathroom.ReverbStrength > Bedroom.ReverbStrength);

	const FApartmentLifeRoomAmbienceProfile Office =
		UApartmentLifeImmersionLibrary::GetRoomAmbienceProfile(EApartmentLifeRoomType::Office);
	TestEqual(TEXT("Office music override"), Office.MusicCategoryOverride, EApartmentLifeMusicCategory::Work);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeMusicByHourTest,
	"ApartmentLife.Immersion.MusicByHour",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeMusicByHourTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Morning music"), UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(8),
		EApartmentLifeMusicCategory::Morning);
	TestEqual(TEXT("Afternoon music"), UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(14),
		EApartmentLifeMusicCategory::Afternoon);
	TestEqual(TEXT("Evening music"), UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(19),
		EApartmentLifeMusicCategory::Evening);
	TestEqual(TEXT("Night music"), UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(23),
		EApartmentLifeMusicCategory::Night);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeLightingPresetTest,
	"ApartmentLife.Immersion.LightingPreset",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeLightingPresetTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Rainy lighting"),
		UApartmentLifeImmersionLibrary::GetLightingPresetForTimeAndWeather(12, EApartmentLifeWeather::Rain),
		EApartmentLifeLightingPreset::RainyDay);
	TestEqual(TEXT("Night lighting"),
		UApartmentLifeImmersionLibrary::GetLightingPresetForTimeAndWeather(22, EApartmentLifeWeather::Clear),
		EApartmentLifeLightingPreset::Night);

	const FApartmentLifeLightingPresetValues GoldenHour =
		UApartmentLifeImmersionLibrary::GetLightingValuesForPreset(EApartmentLifeLightingPreset::GoldenHour);
	TestTrue(TEXT("Golden hour is warm"), GoldenHour.ColorTemperature < 4000.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeImmersionSaveRoundTripTest,
	"ApartmentLife.Immersion.SettingsSaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeImmersionSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UApartmentLifeImmersionSubsystem* Subsystem = NewObject<UApartmentLifeImmersionSubsystem>();
	if (!TestNotNull(TEXT("Subsystem created"), Subsystem))
	{
		return false;
	}

	FApartmentLifeImmersionUserSettings Settings;
	Settings.Audio.MasterVolume = 0.55f;
	Settings.Audio.EffectsVolume = 0.42f;
	Settings.Accessibility.bSubtitlesEnabled = true;
	Settings.Graphics.QualityPreset = EApartmentLifeGraphicsQuality::Ultra;
	Subsystem->ApplyUserSettings(Settings);

	TMap<FString, FString> SavedData;
	Subsystem->CaptureSaveData_Implementation(SavedData);
	TestTrue(TEXT("Save data captured"), SavedData.Contains(TEXT("UserSettings")));

	UApartmentLifeImmersionSubsystem* Restored = NewObject<UApartmentLifeImmersionSubsystem>();
	Restored->RestoreSaveData_Implementation(SavedData);
	const FApartmentLifeImmersionUserSettings& RestoredSettings = Restored->GetUserSettings();
	TestEqual(TEXT("Master volume restored"), RestoredSettings.Audio.MasterVolume, 0.55f);
	TestEqual(TEXT("Effects volume restored"), RestoredSettings.Audio.EffectsVolume, 0.42f);
	TestTrue(TEXT("Subtitles restored"), RestoredSettings.Accessibility.bSubtitlesEnabled);
	TestEqual(TEXT("Graphics quality restored"), RestoredSettings.Graphics.QualityPreset,
		EApartmentLifeGraphicsQuality::Ultra);
	return true;
}

#endif

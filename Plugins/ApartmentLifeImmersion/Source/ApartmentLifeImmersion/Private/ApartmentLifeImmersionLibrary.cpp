// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeImmersionLibrary.h"

FApartmentLifeRoomAmbienceProfile UApartmentLifeImmersionLibrary::GetRoomAmbienceProfile(EApartmentLifeRoomType Room)
{
	FApartmentLifeRoomAmbienceProfile Profile;
	switch (Room)
	{
	case EApartmentLifeRoomType::Bedroom:
		Profile.RoomToneId = FName(TEXT("audio.ambience.bedroom.soft"));
		Profile.LayerIds = { FName(TEXT("audio.ambience.window.quiet")), FName(TEXT("audio.ambience.night.crickets")) };
		Profile.MusicCategoryOverride = EApartmentLifeMusicCategory::Relaxation;
		Profile.ReverbStrength = 0.15f;
		break;
	case EApartmentLifeRoomType::Bathroom:
		Profile.RoomToneId = FName(TEXT("audio.ambience.bathroom.echo"));
		Profile.LayerIds = { FName(TEXT("audio.ambience.fan.exhaust")), FName(TEXT("audio.ambience.sink.water")) };
		Profile.ReverbStrength = 0.45f;
		break;
	case EApartmentLifeRoomType::LivingRoom:
		Profile.RoomToneId = FName(TEXT("audio.ambience.living.roomtone"));
		Profile.LayerIds = {
			FName(TEXT("audio.ambience.tv.soft")),
			FName(TEXT("audio.ambience.ac.hum")),
			FName(TEXT("audio.ambience.clock.tick")),
			FName(TEXT("audio.ambience.traffic.distant"))
		};
		break;
	case EApartmentLifeRoomType::Kitchen:
		Profile.RoomToneId = FName(TEXT("audio.ambience.kitchen.roomtone"));
		Profile.LayerIds = {
			FName(TEXT("audio.ambience.fridge.hum")),
			FName(TEXT("audio.ambience.water.soft")),
			FName(TEXT("audio.ambience.coffee.maker"))
		};
		break;
	case EApartmentLifeRoomType::Office:
		Profile.RoomToneId = FName(TEXT("audio.ambience.office.roomtone"));
		Profile.LayerIds = {
			FName(TEXT("audio.ambience.computer.fan")),
			FName(TEXT("audio.ambience.keyboard.soft")),
			FName(TEXT("audio.ambience.mouse.click"))
		};
		Profile.MusicCategoryOverride = EApartmentLifeMusicCategory::Work;
		Profile.ReverbStrength = 0.2f;
		break;
	default:
		Profile.RoomToneId = FName(TEXT("audio.ambience.apartment.generic"));
		break;
	}
	return Profile;
}

EApartmentLifeMusicCategory UApartmentLifeImmersionLibrary::GetMusicCategoryForHour(int32 Hour)
{
	if (Hour >= 6 && Hour < 11) return EApartmentLifeMusicCategory::Morning;
	if (Hour >= 11 && Hour < 17) return EApartmentLifeMusicCategory::Afternoon;
	if (Hour >= 17 && Hour < 21) return EApartmentLifeMusicCategory::Evening;
	return EApartmentLifeMusicCategory::Night;
}

EApartmentLifeMusicCategory UApartmentLifeImmersionLibrary::GetMusicCategoryForActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("yoga"))) return EApartmentLifeMusicCategory::Yoga;
	if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("computer"))) return EApartmentLifeMusicCategory::Work;
	if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("wardrobe"))) return EApartmentLifeMusicCategory::Wardrobe;
	if (Id.Contains(TEXT("build"))) return EApartmentLifeMusicCategory::BuildMode;
	if (Id.Contains(TEXT("relax")) || Id.Contains(TEXT("read")) || Id.Contains(TEXT("tv"))) return EApartmentLifeMusicCategory::Relaxation;
	return EApartmentLifeMusicCategory::Afternoon;
}

EApartmentLifeLightingPreset UApartmentLifeImmersionLibrary::GetLightingPresetForTimeAndWeather(int32 Hour, EApartmentLifeWeather Weather)
{
	if (Weather == EApartmentLifeWeather::Rain || Weather == EApartmentLifeWeather::Storm) return EApartmentLifeLightingPreset::RainyDay;
	if (Weather == EApartmentLifeWeather::Cloudy) return EApartmentLifeLightingPreset::CloudyDay;
	if (Hour >= 21 || Hour < 6) return EApartmentLifeLightingPreset::Night;
	if (Hour >= 6 && Hour < 9) return EApartmentLifeLightingPreset::Morning;
	if (Hour >= 17 && Hour < 19) return EApartmentLifeLightingPreset::GoldenHour;
	if (Hour >= 19 && Hour < 21) return EApartmentLifeLightingPreset::Evening;
	return EApartmentLifeLightingPreset::Midday;
}

FApartmentLifeLightingPresetValues UApartmentLifeImmersionLibrary::GetLightingValuesForPreset(EApartmentLifeLightingPreset Preset)
{
	FApartmentLifeLightingPresetValues Values;
	switch (Preset)
	{
	case EApartmentLifeLightingPreset::Morning:
		Values.SunIntensity = 2.5f; Values.ColorTemperature = 5200.f; Values.WindowIntensity = 1.4f; break;
	case EApartmentLifeLightingPreset::GoldenHour:
		Values.SunIntensity = 2.2f; Values.ColorTemperature = 3800.f; Values.WindowIntensity = 1.6f; break;
	case EApartmentLifeLightingPreset::Evening:
		Values.SunIntensity = 1.2f; Values.ColorTemperature = 3200.f; Values.InteriorIntensity = 1.3f; break;
	case EApartmentLifeLightingPreset::Night:
		Values.SunIntensity = 0.1f; Values.ColorTemperature = 2800.f; Values.InteriorIntensity = 1.5f; Values.WindowIntensity = 0.4f; break;
	case EApartmentLifeLightingPreset::RainyDay:
		Values.SunIntensity = 0.8f; Values.ColorTemperature = 6000.f; Values.ShadowSoftness = 0.8f; break;
	case EApartmentLifeLightingPreset::CloudyDay:
		Values.SunIntensity = 1.4f; Values.ColorTemperature = 5800.f; Values.ShadowSoftness = 0.7f; break;
	default:
		Values.SunIntensity = 3.f; Values.ColorTemperature = 5500.f; break;
	}
	return Values;
}

TArray<FName> UApartmentLifeImmersionLibrary::GetWeatherAmbienceLayers(EApartmentLifeWeather Weather, int32 Hour)
{
	TArray<FName> Layers;
	switch (Weather)
	{
	case EApartmentLifeWeather::Clear:
		Layers.Add(FName(TEXT("audio.weather.birds")));
		Layers.Add(FName(TEXT("audio.weather.wind.light")));
		break;
	case EApartmentLifeWeather::Rain:
	case EApartmentLifeWeather::Storm:
		Layers.Add(FName(TEXT("audio.weather.rain.windows")));
		Layers.Add(FName(TEXT("audio.weather.traffic.distant")));
		if (Weather == EApartmentLifeWeather::Storm)
		{
			Layers.Add(FName(TEXT("audio.weather.thunder.distant")));
		}
		break;
	case EApartmentLifeWeather::Snow:
		Layers.Add(FName(TEXT("audio.weather.wind.soft")));
		break;
	default:
		break;
	}

	if (Hour >= 21 || Hour < 6)
	{
		Layers.Add(FName(TEXT("audio.weather.night.crickets")));
		Layers.Add(FName(TEXT("audio.weather.city.distant")));
	}
	return Layers;
}

FName UApartmentLifeImmersionLibrary::GetActivitySfxId(FName ActivityId)
{
	FString Id = ActivityId.ToString();
	if (Id.StartsWith(TEXT("activity."))) Id = Id.RightChop(9);
	return FName(*FString::Printf(TEXT("audio.sfx.%s"), *Id));
}

FName UApartmentLifeImmersionLibrary::GetMusicIdForCategory(EApartmentLifeMusicCategory Category)
{
	return FName(*FString::Printf(TEXT("audio.music.%s"), *UEnum::GetValueAsString(Category)));
}

FApartmentLifeEnvironmentalMotionState UApartmentLifeImmersionLibrary::BuildEnvironmentalMotion(
	EApartmentLifeRoomType Room,
	FName ActivityId,
	EApartmentLifeWeather Weather,
	int32 Hour)
{
	FApartmentLifeEnvironmentalMotionState Motion;
	const FString Id = ActivityId.ToString().ToLower();
	Motion.bCurtainsMoving = Weather != EApartmentLifeWeather::Clear || Hour < 20;
	Motion.bPlantsSwaying = true;
	Motion.bCeilingFanSpinning = Room == EApartmentLifeRoomType::LivingRoom || Room == EApartmentLifeRoomType::Bedroom;
	Motion.bClockTicking = Room == EApartmentLifeRoomType::LivingRoom;
	Motion.bRainOnWindows = Weather == EApartmentLifeWeather::Rain || Weather == EApartmentLifeWeather::Storm;
	Motion.bSunlightDust = Hour >= 8 && Hour < 17 && Weather == EApartmentLifeWeather::Clear;
	Motion.bFoodSteam = Id.Contains(TEXT("cook"));
	Motion.bTvGlow = Id.Contains(TEXT("tv")) || Id.Contains(TEXT("watch"));
	Motion.bMonitorGlow = Id.Contains(TEXT("computer")) || Id.Contains(TEXT("work"));
	return Motion;
}

EApartmentLifeMicroAnimationCue UApartmentLifeImmersionLibrary::PickMicroAnimationCue(
	float Mood, float Energy, float Comfort, bool bIsSitting)
{
	if (Energy < 25.f) return EApartmentLifeMicroAnimationCue::Yawn;
	if (Comfort > 70.f && Mood > 60.f) return EApartmentLifeMicroAnimationCue::RelaxedSmile;
	if (bIsSitting && Energy < 50.f) return EApartmentLifeMicroAnimationCue::Stretch;
	if (FMath::FRand() < 0.2f) return EApartmentLifeMicroAnimationCue::Blink;
	if (FMath::FRand() < 0.15f) return EApartmentLifeMicroAnimationCue::LookAround;
	if (FMath::FRand() < 0.1f) return EApartmentLifeMicroAnimationCue::HairAdjust;
	return EApartmentLifeMicroAnimationCue::Breathing;
}

FName UApartmentLifeImmersionLibrary::GetMontageIdForMicroCue(EApartmentLifeMicroAnimationCue Cue)
{
	if (Cue == EApartmentLifeMicroAnimationCue::None)
	{
		return NAME_None;
	}

	const UEnum* CueEnum = StaticEnum<EApartmentLifeMicroAnimationCue>();
	const FString CueName = CueEnum ? CueEnum->GetNameStringByValue(static_cast<int64>(Cue)) : TEXT("None");
	return FName(*FString::Printf(TEXT("montage.micro.%s"), *CueName));
}

FString UApartmentLifeImmersionLibrary::GetWeatherDisplayString(EApartmentLifeWeather Weather)
{
	const UEnum* WeatherEnum = StaticEnum<EApartmentLifeWeather>();
	return WeatherEnum ? WeatherEnum->GetDisplayNameTextByValue(static_cast<int64>(Weather)).ToString() : TEXT("Clear");
}

EApartmentLifeRoomType UApartmentLifeImmersionLibrary::GetRoomForActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("brush")) || Id.Contains(TEXT("hygiene"))) return EApartmentLifeRoomType::Bathroom;
	if (Id.Contains(TEXT("cook")) || Id.Contains(TEXT("eat")) || Id.Contains(TEXT("meal"))) return EApartmentLifeRoomType::Kitchen;
	if (Id.Contains(TEXT("sleep")) || Id.Contains(TEXT("nap")) || Id.Contains(TEXT("read.bed"))) return EApartmentLifeRoomType::Bedroom;
	if (Id.Contains(TEXT("computer")) || Id.Contains(TEXT("work"))) return EApartmentLifeRoomType::Office;
	if (Id.Contains(TEXT("yoga"))) return EApartmentLifeRoomType::LivingRoom;
	return EApartmentLifeRoomType::LivingRoom;
}

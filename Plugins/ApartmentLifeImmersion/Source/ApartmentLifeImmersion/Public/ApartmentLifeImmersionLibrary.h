// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeImmersionTypes.h"
#include "ApartmentLifeImmersionLibrary.generated.h"

UCLASS()
class APARTMENTLIFEIMMERSION_API UApartmentLifeImmersionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FApartmentLifeRoomAmbienceProfile GetRoomAmbienceProfile(EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static EApartmentLifeMusicCategory GetMusicCategoryForHour(int32 Hour);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static EApartmentLifeMusicCategory GetMusicCategoryForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static EApartmentLifeLightingPreset GetLightingPresetForTimeAndWeather(int32 Hour, EApartmentLifeWeather Weather);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FApartmentLifeLightingPresetValues GetLightingValuesForPreset(EApartmentLifeLightingPreset Preset);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static TArray<FName> GetWeatherAmbienceLayers(EApartmentLifeWeather Weather, int32 Hour);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FName GetActivitySfxId(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FName GetMusicIdForCategory(EApartmentLifeMusicCategory Category);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FApartmentLifeEnvironmentalMotionState BuildEnvironmentalMotion(
		EApartmentLifeRoomType Room,
		FName ActivityId,
		EApartmentLifeWeather Weather,
		int32 Hour);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static EApartmentLifeMicroAnimationCue PickMicroAnimationCue(float Mood, float Energy, float Comfort, bool bIsSitting);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FName GetMontageIdForMicroCue(EApartmentLifeMicroAnimationCue Cue);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static FString GetWeatherDisplayString(EApartmentLifeWeather Weather);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	static EApartmentLifeRoomType GetRoomForActivity(FName ActivityId);
};

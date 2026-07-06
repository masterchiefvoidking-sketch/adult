// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeImmersionTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeMusicCategory : uint8
{
	Morning			UMETA(DisplayName = "Morning"),
	Afternoon		UMETA(DisplayName = "Afternoon"),
	Evening			UMETA(DisplayName = "Evening"),
	Night			UMETA(DisplayName = "Night"),
	Relaxation		UMETA(DisplayName = "Relaxation"),
	Work			UMETA(DisplayName = "Work"),
	Yoga			UMETA(DisplayName = "Yoga"),
	CharacterCreator UMETA(DisplayName = "Character Creator"),
	Wardrobe		UMETA(DisplayName = "Wardrobe"),
	BuildMode		UMETA(DisplayName = "Build Mode"),
	MainMenu		UMETA(DisplayName = "Main Menu")
};

UENUM(BlueprintType)
enum class EApartmentLifeLightingPreset : uint8
{
	Morning		UMETA(DisplayName = "Morning"),
	Midday		UMETA(DisplayName = "Midday"),
	GoldenHour	UMETA(DisplayName = "Golden Hour"),
	Evening		UMETA(DisplayName = "Evening"),
	Night		UMETA(DisplayName = "Night"),
	RainyDay	UMETA(DisplayName = "Rainy Day"),
	CloudyDay	UMETA(DisplayName = "Cloudy Day")
};

UENUM(BlueprintType)
enum class EApartmentLifeMicroAnimationCue : uint8
{
	None			UMETA(DisplayName = "None"),
	Blink			UMETA(DisplayName = "Blink"),
	Breathing		UMETA(DisplayName = "Breathing"),
	WeightShift		UMETA(DisplayName = "Weight Shift"),
	LookAround		UMETA(DisplayName = "Look Around"),
	Stretch			UMETA(DisplayName = "Stretch"),
	HairAdjust		UMETA(DisplayName = "Hair Adjust"),
	ClothingAdjust	UMETA(DisplayName = "Clothing Adjust"),
	RelaxedSmile	UMETA(DisplayName = "Relaxed Smile"),
	Yawn			UMETA(DisplayName = "Yawn")
};

UENUM(BlueprintType)
enum class EApartmentLifeGraphicsQuality : uint8
{
	Low			UMETA(DisplayName = "Low"),
	Medium		UMETA(DisplayName = "Medium"),
	High		UMETA(DisplayName = "High"),
	Ultra		UMETA(DisplayName = "Ultra")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeRoomAmbienceProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambience")
	FName RoomToneId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambience")
	TArray<FName> LayerIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambience")
	FName MusicCategoryOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ambience")
	float ReverbStrength = 0.2f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeAudioSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float MasterVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float MusicVolume = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float EffectsVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta = (ClampMin = "0", ClampMax = "1"))
	float UiVolume = 0.9f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeAccessibilitySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bSubtitlesEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	float SubtitleScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	bool bReduceMotion = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility")
	float CameraSensitivityScale = 1.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeGraphicsSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	EApartmentLifeGraphicsQuality QualityPreset = EApartmentLifeGraphicsQuality::High;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool bHighQualityEffects = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool bVSyncEnabled = true;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeLightingPresetValues
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float SunIntensity = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float InteriorIntensity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float WindowIntensity = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float ColorTemperature = 5500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float ShadowSoftness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
	float ReflectionStrength = 0.6f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeEnvironmentalMotionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bCurtainsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bPlantsSwaying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bCeilingFanSpinning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bFoodSteam = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bSunlightDust = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bTvGlow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bMonitorGlow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bRainOnWindows = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	bool bClockTicking = false;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifePhotoBookmark
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	FName BookmarkId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float FieldOfView = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float Aperture = 2.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float TimeOfDayHour = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	EApartmentLifeLightingPreset LightingPreset = EApartmentLifeLightingPreset::Midday;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeImmersionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	EApartmentLifeRoomType ActiveRoom = EApartmentLifeRoomType::LivingRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	EApartmentLifeMusicCategory ActiveMusicCategory = EApartmentLifeMusicCategory::Afternoon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	EApartmentLifeLightingPreset ActiveLightingPreset = EApartmentLifeLightingPreset::Midday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FName ActiveRoomToneId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FName ActiveMusicId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	TArray<FName> ActiveAmbienceLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	TArray<FName> ActiveWeatherLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FName LastActivitySfxId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	EApartmentLifeMicroAnimationCue ActiveMicroAnimation = EApartmentLifeMicroAnimationCue::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FApartmentLifeEnvironmentalMotionState EnvironmentalMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FApartmentLifeLightingPresetValues LightingValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	float MusicCrossfadeAlpha = 1.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEIMMERSION_API FApartmentLifeImmersionUserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FApartmentLifeAudioSettings Audio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FApartmentLifeAccessibilitySettings Accessibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	FApartmentLifeGraphicsSettings Graphics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	TArray<FApartmentLifePhotoBookmark> PhotoBookmarks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersion")
	int32 ActivePhotoBookmarkIndex = INDEX_NONE;
};

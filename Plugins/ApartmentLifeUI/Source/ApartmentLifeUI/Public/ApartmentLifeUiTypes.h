// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeUiTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeUiScreen : uint8
{
	None			UMETA(DisplayName = "None"),
	MainMenu		UMETA(DisplayName = "Main Menu"),
	Gameplay		UMETA(DisplayName = "Gameplay"),
	ContextMenu		UMETA(DisplayName = "Context Menu"),
	Wardrobe		UMETA(DisplayName = "Wardrobe"),
	BuildMode		UMETA(DisplayName = "Build Mode"),
	Shopping		UMETA(DisplayName = "Shopping"),
	Work			UMETA(DisplayName = "Work"),
	WorkResult		UMETA(DisplayName = "Work Result"),
	Finance			UMETA(DisplayName = "Finance"),
	Routines		UMETA(DisplayName = "Routines"),
	Yoga			UMETA(DisplayName = "Yoga"),
	Profile			UMETA(DisplayName = "Profile"),
	SaveLoad		UMETA(DisplayName = "Save / Load"),
	Settings		UMETA(DisplayName = "Settings"),
	CharacterCreator UMETA(DisplayName = "Character Creator"),
	DeveloperHub	UMETA(DisplayName = "Developer Hub")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeUiListEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bOwned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 Index = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeHudViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString TimeLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString WeatherLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Savings = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Mood = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Energy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Hygiene = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Hunger = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Comfort = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Confidence = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Stress = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	float Affection = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString ActivityLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	FString DeveloperOverlayLabel;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeMainMenuSlotViewModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	int32 SlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	bool bExists = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	FString LastPlayedLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	float Savings = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeUiSettingsState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float CameraSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bInvertMouseY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float PhotoFov = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MasterVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MusicVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float UiVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float EffectsVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bSubtitlesEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SubtitleScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bReduceMotion = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 GraphicsQuality = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bHighQualityEffects = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bAutosaveEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bHudVisible = true;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeUiPanelState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Footer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FApartmentLifeUiListEntry> Entries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 SelectedIndex = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeYogaUiState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	FString PoseName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float PoseAccuracy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float Balance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float BreathingRhythm = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	int32 PoseStreak = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float SessionSeconds = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEUI_API FApartmentLifeUiToast
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float RemainingSeconds = 3.f;
};

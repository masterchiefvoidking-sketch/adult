// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeActivityData.h"
#include "ApartmentLifeCameraTypes.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeActivityTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeActivityRoom : uint8
{
	Any				UMETA(DisplayName = "Any"),
	Bedroom			UMETA(DisplayName = "Bedroom"),
	Bathroom		UMETA(DisplayName = "Bathroom"),
	LivingRoom		UMETA(DisplayName = "Living Room"),
	Kitchen			UMETA(DisplayName = "Kitchen"),
	DiningRoom		UMETA(DisplayName = "Dining Room"),
	WalkInCloset	UMETA(DisplayName = "Closet / Wardrobe"),
	Office			UMETA(DisplayName = "Desk / Computer"),
	LaundryRoom		UMETA(DisplayName = "Laundry"),
	YogaArea		UMETA(DisplayName = "Yoga / Exercise")
};

UENUM(BlueprintType)
enum class EApartmentLifeRoutineChainType : uint8
{
	Morning		UMETA(DisplayName = "Morning Routine"),
	Evening		UMETA(DisplayName = "Evening Routine"),
	Custom		UMETA(DisplayName = "Custom Routine")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEACTIVITIES_API FApartmentLifeActivityStatEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MoodDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float EnergyDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HygieneDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HungerDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ComfortDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ConfidenceDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StressDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AffectionDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float FitnessXPDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float FlexibilityXPDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float OrganizationXPDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CreativityXPDelta = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEACTIVITIES_API FApartmentLifeActivityDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	EApartmentLifeActivityCategory Category = EApartmentLifeActivityCategory::Relaxing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	EApartmentLifeActivityRoom RequiredRoom = EApartmentLifeActivityRoom::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FName RequiredFurnitureTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	EApartmentLifeOutfitContext RequiredOutfitContext = EApartmentLifeOutfitContext::Everyday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	bool bRequiresOutfitMatch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	EApartmentLifeAnimationGroup AnimationGroup = EApartmentLifeAnimationGroup::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FName MontageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	int32 DurationMinutes = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	EApartmentLifePrimaryCameraMode CameraMode = EApartmentLifePrimaryCameraMode::Activity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FApartmentLifeActivityCameraFrame CameraFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	bool bPrivacyFraming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FApartmentLifeActivityStatEffect StatEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	float MoneyEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	bool bUsesIncomeFormula = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	int32 CooldownMinutes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activity")
	FName NextActivityInChain;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEACTIVITIES_API FApartmentLifeActivityCompletionResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	bool bSuccess = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FApartmentLifeActivityStatEffect AppliedEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	float MoneyEarned = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
	FName NextActivityId;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEACTIVITIES_API FApartmentLifeRoutineChainDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Routine")
	FName ChainId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Routine")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Routine")
	EApartmentLifeRoutineChainType ChainType = EApartmentLifeRoutineChainType::Custom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Routine")
	TArray<FName> ActivityIds;
};

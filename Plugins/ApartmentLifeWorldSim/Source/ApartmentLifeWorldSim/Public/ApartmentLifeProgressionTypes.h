// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeProgressionTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeFurnitureTier : uint8
{
	Starter		UMETA(DisplayName = "Starter"),
	Budget		UMETA(DisplayName = "Budget"),
	Standard	UMETA(DisplayName = "Standard"),
	Premium		UMETA(DisplayName = "Premium"),
	Luxury		UMETA(DisplayName = "Luxury")
};

UENUM(BlueprintType)
enum class EApartmentLifeShopCategory : uint8
{
	Clothing		UMETA(DisplayName = "Clothing"),
	Furniture		UMETA(DisplayName = "Furniture"),
	Decorations		UMETA(DisplayName = "Decorations"),
	Electronics		UMETA(DisplayName = "Electronics"),
	Kitchen			UMETA(DisplayName = "Kitchen Supplies"),
	Groceries		UMETA(DisplayName = "Groceries"),
	Fitness			UMETA(DisplayName = "Fitness / Yoga"),
	Bathroom		UMETA(DisplayName = "Bathroom / Grooming")
};

UENUM(BlueprintType)
enum class EApartmentLifeUpgradeType : uint8
{
	Bed				UMETA(DisplayName = "Bed"),
	Desk			UMETA(DisplayName = "Desk"),
	Chair			UMETA(DisplayName = "Chair"),
	Computer		UMETA(DisplayName = "Computer"),
	Monitor			UMETA(DisplayName = "Monitor"),
	Lighting		UMETA(DisplayName = "Lighting"),
	Shower			UMETA(DisplayName = "Shower Fixtures"),
	Storage			UMETA(DisplayName = "Storage"),
	KitchenTools	UMETA(DisplayName = "Kitchen Tools"),
	TV				UMETA(DisplayName = "TV"),
	YogaMat			UMETA(DisplayName = "Yoga Mat"),
	Decor			UMETA(DisplayName = "Decor"),
	Wardrobe		UMETA(DisplayName = "Wardrobe / Closet")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeComputerQualityState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Computer")
	int32 ComputerTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Computer")
	int32 MonitorTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Computer")
	int32 ChairTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Computer")
	int32 DeskTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Computer")
	int32 InternetTier = 1;

	APARTMENTLIFEWORLDSIM_API float GetProductivityBonus() const;
	APARTMENTLIFEWORLDSIM_API float GetStressReductionBonus() const;
	APARTMENTLIFEWORLDSIM_API float GetSkillXpBonus() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeWorkTypeDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FName WorkTypeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	EApartmentLifeSkill RequiredSkill = EApartmentLifeSkill::Programming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float MinSkillLevel = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	int32 DurationMinutes = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float BasePayout = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float EnergyCost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float StressGain = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float SkillXpGain = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float MoodImpact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float SuccessChance = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float BonusChance = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float SavingsUnlockThreshold = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeWorkSessionResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FName WorkTypeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	bool bSuccess = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float BaseIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float SkillMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float MoodModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float EnergyModifier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float BonusIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float TotalIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float StressChange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float SkillXpGained = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	int32 MinutesPassed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	TArray<FName> NewUnlocks;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeDailyBudget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float DailyIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float WeeklyIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float RentSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float FoodSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float UtilitiesSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float ClothingSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float FurnitureSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float DecorationSpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	float EmergencySpent = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Budget")
	int32 DayIndex = 0;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeBuiltinShopItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	EApartmentLifeShopCategory Category = EApartmentLifeShopCategory::Furniture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	EApartmentLifeFurnitureTier Tier = EApartmentLifeFurnitureTier::Budget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	float Price = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	float ComfortBonus = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	float MoodBonus = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	float ProductivityBonus = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	EApartmentLifeUpgradeType UpgradeType = EApartmentLifeUpgradeType::Decor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 UpgradeTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	float SavingsUnlockThreshold = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeApartmentUpgradeState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TMap<EApartmentLifeUpgradeType, int32> UpgradeTiers;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeWorldSimDataAssets.generated.h"

UCLASS(BlueprintType)
class APARTMENTLIFEWORLDSIM_API UApartmentLifeCareerData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("Career")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	float BaseHourlyWage = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	float BaseMonthlySalary = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	bool bIsSalaried = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	int32 MaxPromotionTier = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	float ExperiencePerWorkDay = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	float PromotionExperienceThreshold = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	float WageIncreasePerPromotion = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	int32 WorkStartHour = 9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	int32 WorkEndHour = 17;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Career")
	FName WorkActivityId = FName(TEXT("activity.work.office"));
};

USTRUCT(BlueprintType)
struct FApartmentLifeScheduleTemplateSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	EApartmentLifeScheduleBlock BlockType = EApartmentLifeScheduleBlock::Relax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	int32 StartHour = 9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	int32 EndHour = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	bool bWeekdayOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	bool bWeekendOnly = false;
};

UCLASS(BlueprintType)
class APARTMENTLIFEWORLDSIM_API UApartmentLifeScheduleTemplateData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ScheduleTemplate")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Schedule")
	TArray<FApartmentLifeScheduleTemplateSlot> DefaultSlots;
};

UCLASS(BlueprintType)
class APARTMENTLIFEWORLDSIM_API UApartmentLifeShoppingItemData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ShoppingItem")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
	EApartmentLifeShoppingCategory Category = EApartmentLifeShoppingCategory::Furniture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
	float Price = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
	float FloorSpaceRequired = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
	TArray<EApartmentLifeSeason> SeasonalAppeal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shopping")
	TMap<FName, float> PersonalityAppeal;
};

UCLASS(BlueprintType)
class APARTMENTLIFEWORLDSIM_API UApartmentLifeApartmentTierData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ApartmentTier")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apartment")
	int32 TierLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apartment")
	float MinimumSavings = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apartment")
	float MonthlyRent = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apartment")
	float FloorSpace = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Apartment")
	int32 MinimumCreditScore = 600;
};

UCLASS(BlueprintType)
class APARTMENTLIFEWORLDSIM_API UApartmentLifeRandomEventData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("RandomEvent")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float BaseProbability = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	bool bRequiresWeekend = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	TArray<EApartmentLifeSeason> ValidSeasons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float MoodImpact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float FinancialImpact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	FName TriggeredActivityId;
};

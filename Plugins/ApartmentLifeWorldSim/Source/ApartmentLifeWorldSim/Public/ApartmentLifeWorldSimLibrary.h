// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeWorldSimDataAssets.h"
#include "ApartmentLifeWorldSimLibrary.generated.h"

class UApartmentLifeCareerData;

/** Stateless simulation math used by components and automation tests. */
UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeWorldSimLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	static TArray<FApartmentLifeDailyScheduleSlot> GenerateDailySchedule(
		const UApartmentLifeScheduleTemplateData* Template,
		const FApartmentLifePersonalityTraits& Personality,
		const FApartmentLifeMoodState& Mood,
		const FApartmentLifeFinancialLedger& Finance,
		const FApartmentLifeCareerState& Career,
		const FApartmentLifeWeatherState& Weather,
		bool bIsWeekend,
		bool bIsHoliday);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	static FApartmentLifeMoodState RecalculateMood(
		const FApartmentLifeMoodState& Current,
		const FApartmentLifeMoodInfluences& Influences,
		const FApartmentLifePersonalityTraits& Personality);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	static TArray<FApartmentLifeShoppingIntent> EvaluateShoppingNeeds(
		const FApartmentLifePersonalityTraits& Personality,
		const FApartmentLifeMoodState& Mood,
		const FApartmentLifeFinancialLedger& Finance,
		const FApartmentLifeNPCNeeds& Needs,
		const FApartmentLifeApartmentProgressState& Apartment,
		EApartmentLifeSeason Season,
		const TArray<UApartmentLifeShoppingItemData*>& AvailableItems);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	static bool TryPromoteCareer(FApartmentLifeCareerState& Career, const UApartmentLifeCareerData* CareerData);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	static FName GetActivityIdForBlock(EApartmentLifeScheduleBlock Block, const FApartmentLifeCareerState& Career);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	static TArray<FApartmentLifeDailyScheduleSlot> CreateBuiltinWeekdaySchedule();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	static float ScoreScheduleBlockPriority(
		EApartmentLifeScheduleBlock Block,
		const FApartmentLifePersonalityTraits& Personality,
		const FApartmentLifeMoodState& Mood,
		const FApartmentLifeNPCNeeds& Needs);
};

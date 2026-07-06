// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorldSimLibrary.h"

TArray<FApartmentLifeDailyScheduleSlot> UApartmentLifeWorldSimLibrary::GenerateDailySchedule(
	const UApartmentLifeScheduleTemplateData* Template,
	const FApartmentLifePersonalityTraits& Personality,
	const FApartmentLifeMoodState& Mood,
	const FApartmentLifeFinancialLedger& Finance,
	const FApartmentLifeCareerState& Career,
	const FApartmentLifeWeatherState& Weather,
	bool bIsWeekend,
	bool bIsHoliday)
{
	TArray<FApartmentLifeDailyScheduleSlot> Result;

	if (!Template)
	{
		return Result;
	}

	FApartmentLifeNPCNeeds EstimatedNeeds;
	EstimatedNeeds.Hunger = Mood.Energy < 40.f ? 70.f : 30.f;
	EstimatedNeeds.Social = (1.f - Personality.Introversion) * 60.f;
	EstimatedNeeds.Fun = 100.f - Mood.OverallMood;

	for (const FApartmentLifeScheduleTemplateSlot& TemplateSlot : Template->DefaultSlots)
	{
		if (TemplateSlot.bWeekdayOnly && bIsWeekend)
		{
			continue;
		}
		if (TemplateSlot.bWeekendOnly && !bIsWeekend)
		{
			continue;
		}

		FApartmentLifeDailyScheduleSlot Slot;
		Slot.BlockType = TemplateSlot.BlockType;
		Slot.StartHour = TemplateSlot.StartHour;
		Slot.EndHour = TemplateSlot.EndHour;
		Slot.ActivityId = TemplateSlot.ActivityId.IsNone() ? GetActivityIdForBlock(TemplateSlot.BlockType, Career) : TemplateSlot.ActivityId;
		Slot.Priority = ScoreScheduleBlockPriority(TemplateSlot.BlockType, Personality, Mood, EstimatedNeeds);

		switch (TemplateSlot.BlockType)
		{
		case EApartmentLifeScheduleBlock::Work:
			if (bIsWeekend || bIsHoliday || Career.VacationDaysRemaining > 0 && bIsHoliday)
			{
				continue;
			}
			Slot.Occasion = EApartmentLifeOccasion::Work;
			break;
		case EApartmentLifeScheduleBlock::Exercise:
			Slot.Priority += Personality.FitnessInterest * 0.5f;
			if (Weather.Weather == EApartmentLifeWeather::Rain || Weather.Weather == EApartmentLifeWeather::Storm)
			{
				Slot.Priority *= 0.5f;
			}
			Slot.Occasion = EApartmentLifeOccasion::Athletic;
			break;
		case EApartmentLifeScheduleBlock::Shopping:
			if (Finance.GetDisposableIncome(Career.ComputeMonthlyIncome()) < 50.f)
			{
				Slot.Priority *= 0.3f;
			}
			Slot.Priority += Personality.SpendingHabits * 0.3f;
			break;
		case EApartmentLifeScheduleBlock::Social:
			Slot.Priority *= Mood.GetSocialDriveMultiplier();
			Slot.Priority += (1.f - Personality.Introversion) * 0.4f;
			break;
		case EApartmentLifeScheduleBlock::Relax:
			Slot.Priority += Mood.Stress * 0.01f;
			Slot.Occasion = EApartmentLifeOccasion::Relaxing;
			break;
		case EApartmentLifeScheduleBlock::Entertainment:
			Slot.Priority += Personality.GamingInterest * 0.3f + Personality.MusicInterest * 0.2f;
			if (Finance.MonthlyEntertainment < 30.f)
			{
				Slot.Priority *= 0.4f;
			}
			break;
		case EApartmentLifeScheduleBlock::Chores:
			Slot.Priority += Personality.Tidiness * 0.5f;
			break;
		case EApartmentLifeScheduleBlock::Sleep:
			Slot.Occasion = EApartmentLifeOccasion::Sleep;
			break;
		case EApartmentLifeScheduleBlock::GetDressed:
			Slot.Occasion = EApartmentLifeOccasion::Work;
			break;
		default:
			break;
		}

		if (Mood.Fatigue > 70.f && TemplateSlot.BlockType == EApartmentLifeScheduleBlock::Sleep)
		{
			Slot.StartHour = FMath::Max(20, Slot.StartHour - 1);
		}

		Result.Add(Slot);
	}

	Result.Sort([](const FApartmentLifeDailyScheduleSlot& A, const FApartmentLifeDailyScheduleSlot& B)
	{
		return A.StartHour < B.StartHour;
	});

	return Result;
}

FApartmentLifeMoodState UApartmentLifeWorldSimLibrary::RecalculateMood(
	const FApartmentLifeMoodState& Current,
	const FApartmentLifeMoodInfluences& Influences,
	const FApartmentLifePersonalityTraits& Personality)
{
	FApartmentLifeMoodState Updated = Current;

	const float RawMood =
		50.f
		+ Influences.SleepQuality * 0.2f
		+ Influences.WorkSatisfaction * 0.15f
		+ Influences.ApartmentCleanliness * 0.1f
		+ Influences.WeatherComfort * 0.05f
		+ Influences.FinancialSecurity * 0.15f
		+ Influences.MealQuality * 0.1f
		+ Influences.ExerciseBenefit * 0.1f
		+ Influences.SocialFulfillment * 0.15f
		+ Influences.PersonalAchievement * 0.1f;

	Updated.OverallMood = FMath::FInterpTo(Updated.OverallMood, FMath::Clamp(RawMood, 0.f, 100.f), 0.3f, 1.f);
	Updated.Happiness = Updated.OverallMood;
	Updated.Stress = FMath::Clamp(100.f - Influences.FinancialSecurity * 0.5f - Influences.WorkSatisfaction * 0.3f + (1.f - Personality.Patience) * 20.f, 0.f, 100.f);
	Updated.Energy = FMath::Clamp(Influences.SleepQuality + Influences.ExerciseBenefit * 0.5f - Updated.Fatigue, 0.f, 100.f);
	Updated.Fatigue = FMath::Clamp(Updated.Fatigue + (100.f - Influences.SleepQuality) * 0.05f - Influences.ExerciseBenefit * 0.02f, 0.f, 100.f);
	Updated.Confidence = FMath::Clamp(Updated.OverallMood * 0.5f + Influences.PersonalAchievement * 0.3f + Personality.Confidence * 20.f, 0.f, 100.f);
	Updated.Motivation = FMath::Clamp(Updated.OverallMood * 0.4f + Influences.WorkSatisfaction * 0.3f + Personality.Ambition * 30.f, 0.f, 100.f);
	Updated.Loneliness = FMath::Clamp(100.f - Influences.SocialFulfillment - Personality.Extroversion * 20.f, 0.f, 100.f);
	Updated.Comfort = FMath::Clamp(Influences.ApartmentCleanliness * 0.4f + Influences.WeatherComfort * 0.2f + Updated.OverallMood * 0.4f, 0.f, 100.f);
	Updated.SocialBattery = FMath::Clamp(Updated.Energy * 0.5f + Influences.SocialFulfillment * 0.3f - Personality.Introversion * 20.f, 0.f, 100.f);

	return Updated;
}

TArray<FApartmentLifeShoppingIntent> UApartmentLifeWorldSimLibrary::EvaluateShoppingNeeds(
	const FApartmentLifePersonalityTraits& Personality,
	const FApartmentLifeMoodState& Mood,
	const FApartmentLifeFinancialLedger& Finance,
	const FApartmentLifeNPCNeeds& Needs,
	const FApartmentLifeApartmentProgressState& Apartment,
	EApartmentLifeSeason Season,
	const TArray<UApartmentLifeShoppingItemData*>& AvailableItems)
{
	TArray<FApartmentLifeShoppingIntent> Intents;
	const float Disposable = Finance.GetDisposableIncome(Finance.LastMonthIncome);

	for (UApartmentLifeShoppingItemData* Item : AvailableItems)
	{
		if (!Item || Item->Price > Finance.Savings)
		{
			continue;
		}

		FApartmentLifeShoppingIntent Intent;
		Intent.Category = Item->Category;
		Intent.ItemId = Item->AssetId;
		Intent.EstimatedCost = Item->Price;
		Intent.Urgency = 0.f;
		Intent.DesireScore = 0.f;

		switch (Item->Category)
		{
		case EApartmentLifeShoppingCategory::Groceries:
			Intent.Urgency = Needs.Hunger / 100.f;
			Intent.DesireScore = Personality.CookingInterest * 0.3f;
			break;
		case EApartmentLifeShoppingCategory::Furniture:
		case EApartmentLifeShoppingCategory::Decor:
			Intent.DesireScore = Personality.Creativity * 0.3f + Personality.Tidiness * 0.2f;
			Intent.Urgency = Apartment.UpgradeWishlist.Contains(Item->AssetId) ? 0.8f : 0.2f;
			if (Item->FloorSpaceRequired > Apartment.AvailableFloorSpace)
			{
				continue;
			}
			break;
		case EApartmentLifeShoppingCategory::Electronics:
		case EApartmentLifeShoppingCategory::Games:
			Intent.DesireScore = Personality.GamingInterest;
			Intent.Urgency = (100.f - Mood.OverallMood) / 200.f;
			break;
		case EApartmentLifeShoppingCategory::Clothing:
			Intent.DesireScore = Personality.SpendingHabits * 0.5f;
			break;
		case EApartmentLifeShoppingCategory::Books:
			Intent.DesireScore = Personality.ReadingInterest;
			break;
		case EApartmentLifeShoppingCategory::Plants:
			Intent.DesireScore = Personality.Creativity * 0.4f;
			break;
		default:
			Intent.DesireScore = 0.3f;
			break;
		}

		if (Item->SeasonalAppeal.Contains(Season))
		{
			Intent.DesireScore += 0.2f;
		}

		for (const TPair<FName, float>& Appeal : Item->PersonalityAppeal)
		{
			Intent.DesireScore += Appeal.Value * 0.1f;
		}

		const float BudgetFactor = FMath::Clamp(Disposable / FMath::Max(Item->Price, 1.f), 0.f, 1.f);
		const float FinalScore = (Intent.Urgency * 0.6f + Intent.DesireScore * 0.4f) * BudgetFactor;

		if (FinalScore > 0.25f)
		{
			Intents.Add(Intent);
		}
	}

	Intents.Sort([](const FApartmentLifeShoppingIntent& A, const FApartmentLifeShoppingIntent& B)
	{
		return (A.Urgency + A.DesireScore) > (B.Urgency + B.DesireScore);
	});

	return Intents;
}

bool UApartmentLifeWorldSimLibrary::TryPromoteCareer(FApartmentLifeCareerState& Career, const UApartmentLifeCareerData* CareerData)
{
	if (!CareerData || Career.PromotionTier >= CareerData->MaxPromotionTier)
	{
		return false;
	}

	const float Threshold = CareerData->PromotionExperienceThreshold * (Career.PromotionTier + 1);
	if (Career.Experience < Threshold)
	{
		return false;
	}

	++Career.PromotionTier;
	Career.Experience -= Threshold;
	Career.WorkSatisfaction = FMath::Min(Career.WorkSatisfaction + 15.f, 100.f);

	if (Career.bIsSalaried)
	{
		Career.MonthlySalary *= CareerData->WageIncreasePerPromotion;
	}
	else
	{
		Career.HourlyWage *= CareerData->WageIncreasePerPromotion;
	}

	++Career.SkillLevel;
	return true;
}

FName UApartmentLifeWorldSimLibrary::GetActivityIdForBlock(EApartmentLifeScheduleBlock Block, const FApartmentLifeCareerState& Career)
{
	switch (Block)
	{
	case EApartmentLifeScheduleBlock::WakeUp:		return FName(TEXT("activity.sleep.wake"));
	case EApartmentLifeScheduleBlock::Hygiene:		return FName(TEXT("activity.hygiene.shower"));
	case EApartmentLifeScheduleBlock::Breakfast:	return FName(TEXT("activity.cooking.breakfast"));
	case EApartmentLifeScheduleBlock::GetDressed:	return FName(TEXT("activity.grooming.dress"));
	case EApartmentLifeScheduleBlock::Commute:		return FName(TEXT("activity.travel.commute"));
	case EApartmentLifeScheduleBlock::Work:			return Career.CareerId.IsNone() ? FName(TEXT("activity.work.office")) : FName(*(Career.CareerId.ToString() + TEXT(".work")));
	case EApartmentLifeScheduleBlock::Lunch:			return FName(TEXT("activity.cooking.lunch"));
	case EApartmentLifeScheduleBlock::Shopping:		return FName(TEXT("activity.shopping.grocery"));
	case EApartmentLifeScheduleBlock::Exercise:		return FName(TEXT("activity.fitness.yoga"));
	case EApartmentLifeScheduleBlock::Study:			return FName(TEXT("activity.leisure.reading"));
	case EApartmentLifeScheduleBlock::Dinner:			return FName(TEXT("activity.cooking.dinner"));
	case EApartmentLifeScheduleBlock::Relax:			return FName(TEXT("activity.leisure.relax"));
	case EApartmentLifeScheduleBlock::Entertainment:	return FName(TEXT("activity.leisure.gaming"));
	case EApartmentLifeScheduleBlock::Chores:			return FName(TEXT("activity.chores.clean"));
	case EApartmentLifeScheduleBlock::Social:			return FName(TEXT("activity.social.hangout"));
	case EApartmentLifeScheduleBlock::Sleep:			return FName(TEXT("activity.sleep.bed"));
	default:										return FName(TEXT("activity.leisure.relax"));
	}
}

TArray<FApartmentLifeDailyScheduleSlot> UApartmentLifeWorldSimLibrary::CreateBuiltinWeekdaySchedule()
{
	TArray<FApartmentLifeDailyScheduleSlot> Slots;
	FApartmentLifeCareerState DefaultCareer;

	auto AddSlot = [&](EApartmentLifeScheduleBlock Block, int32 Start, int32 End)
	{
		FApartmentLifeDailyScheduleSlot Slot;
		Slot.BlockType = Block;
		Slot.StartHour = Start;
		Slot.EndHour = End;
		Slot.ActivityId = GetActivityIdForBlock(Block, DefaultCareer);
		Slot.Priority = 1.f;
		Slots.Add(Slot);
	};

	AddSlot(EApartmentLifeScheduleBlock::Sleep, 0, 6);
	AddSlot(EApartmentLifeScheduleBlock::WakeUp, 6, 7);
	AddSlot(EApartmentLifeScheduleBlock::Hygiene, 7, 8);
	AddSlot(EApartmentLifeScheduleBlock::Breakfast, 8, 9);
	AddSlot(EApartmentLifeScheduleBlock::GetDressed, 9, 9);
	AddSlot(EApartmentLifeScheduleBlock::Commute, 9, 10);
	AddSlot(EApartmentLifeScheduleBlock::Work, 10, 12);
	AddSlot(EApartmentLifeScheduleBlock::Lunch, 12, 13);
	AddSlot(EApartmentLifeScheduleBlock::Work, 13, 17);
	AddSlot(EApartmentLifeScheduleBlock::Shopping, 17, 18);
	AddSlot(EApartmentLifeScheduleBlock::Exercise, 18, 19);
	AddSlot(EApartmentLifeScheduleBlock::Dinner, 19, 20);
	AddSlot(EApartmentLifeScheduleBlock::Relax, 20, 21);
	AddSlot(EApartmentLifeScheduleBlock::Entertainment, 21, 22);
	AddSlot(EApartmentLifeScheduleBlock::Chores, 22, 23);
	AddSlot(EApartmentLifeScheduleBlock::Sleep, 23, 24);

	return Slots;
}

float UApartmentLifeWorldSimLibrary::ScoreScheduleBlockPriority(
	EApartmentLifeScheduleBlock Block,
	const FApartmentLifePersonalityTraits& Personality,
	const FApartmentLifeMoodState& Mood,
	const FApartmentLifeNPCNeeds& Needs)
{
	switch (Block)
	{
	case EApartmentLifeScheduleBlock::Breakfast:
	case EApartmentLifeScheduleBlock::Lunch:
	case EApartmentLifeScheduleBlock::Dinner:
		return 1.f + Needs.Hunger / 50.f;
	case EApartmentLifeScheduleBlock::Hygiene:
		return 1.f + (100.f - Needs.Hygiene) / 50.f;
	case EApartmentLifeScheduleBlock::Social:
		return 1.f + Needs.Social / 50.f;
	case EApartmentLifeScheduleBlock::Entertainment:
		return 1.f + Needs.Fun / 50.f + Personality.GamingInterest * 0.3f;
	case EApartmentLifeScheduleBlock::Work:
		return 1.f + Personality.Ambition * 0.5f;
	case EApartmentLifeScheduleBlock::Sleep:
		return 1.f + Mood.Fatigue / 40.f;
	default:
		return 1.f;
	}
}

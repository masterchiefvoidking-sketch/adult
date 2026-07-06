// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeGirlLifeLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeWorldSimLibrary.h"
#include "ApartmentLifeActivityFallbackLibrary.h"

static const FName PlayerCharacterId(TEXT("player"));

int32 UApartmentLifeGirlLifeLibrary::GetBuiltinActivityDurationMinutes(FName ActivityId)
{
	return UApartmentLifeActivityFallbackLibrary::GetBuiltinDurationMinutes(ActivityId);
}

void UApartmentLifeGirlLifeLibrary::ApplyActivitySkillGain(FApartmentLifeSkillSet& Skills, FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("cook"))) Skills.GainSkill(EApartmentLifeSkill::Cooking, 0.5f);
	if (Id.Contains(TEXT("exercise")) || Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("gym"))) Skills.GainSkill(EApartmentLifeSkill::Fitness, 0.5f);
	if (Id.Contains(TEXT("clean"))) Skills.GainSkill(EApartmentLifeSkill::HomeMaintenance, 0.3f);
	if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("office")) || Id.Contains(TEXT("computer"))) Skills.GainSkill(EApartmentLifeSkill::Organization, 0.2f);
	if (Id.Contains(TEXT("social")) || Id.Contains(TEXT("talk"))) Skills.GainSkill(EApartmentLifeSkill::Communication, 0.3f);
	if (Id.Contains(TEXT("game"))) Skills.GainSkill(EApartmentLifeSkill::Creativity, 0.2f);
	if (Id.Contains(TEXT("read"))) Skills.GainSkill(EApartmentLifeSkill::Creativity, 0.2f);
}

bool UApartmentLifeGirlLifeLibrary::IsComputerWorkActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	return Id.Contains(TEXT("work.computer"))
		|| Id.Contains(TEXT("work.remote"))
		|| Id.Contains(TEXT("study.read"));
}

float UApartmentLifeGirlLifeLibrary::ComputeComputerWorkPayout(const UApartmentLifeNPCSimulationComponent* Simulation, FName ActivityId)
{
	if (!Simulation || !IsComputerWorkActivity(ActivityId))
	{
		return 0.f;
	}

	const float HourlyRate = Simulation->Career.HourlyWage > 0.f
		? Simulation->Career.HourlyWage
		: 22.f;
	const float DurationHours = GetBuiltinActivityDurationMinutes(ActivityId) / 60.f;
	const float Productivity = Simulation->Mood.GetProductivityMultiplier();
	return FMath::Max(0.f, HourlyRate * DurationHours * Productivity);
}

void UApartmentLifeGirlLifeLibrary::ConfigureRemoteWorkCareer(UApartmentLifeNPCSimulationComponent* Simulation, FName WorkTypeId)
{
	if (!Simulation)
	{
		return;
	}

	Simulation->Career.CareerId = WorkTypeId;
	Simulation->Career.bIsSalaried = false;
	Simulation->Career.WorkplacePOIId = NAME_None;
	Simulation->Career.WorkDistrictId = NAME_None;
	Simulation->Career.HomeDistrictId = NAME_None;

	if (WorkTypeId == FName(TEXT("career.digital_artist")))
	{
		Simulation->Career.HourlyWage = 28.f;
		Simulation->Personality.Creativity = 0.85f;
	}
	else if (WorkTypeId == FName(TEXT("career.online_tutor")))
	{
		Simulation->Career.HourlyWage = 24.f;
	}
	else if (WorkTypeId == FName(TEXT("career.content_editor")))
	{
		Simulation->Career.HourlyWage = 20.f;
	}
	else if (WorkTypeId == FName(TEXT("career.virtual_assistant")))
	{
		Simulation->Career.HourlyWage = 18.f;
	}
	else if (WorkTypeId == FName(TEXT("career.freelancer")))
	{
		Simulation->Career.HourlyWage = 26.f;
	}
	else
	{
		Simulation->Career.CareerId = FName(TEXT("career.remote_programmer"));
		Simulation->Career.HourlyWage = 32.f;
	}
}

void UApartmentLifeGirlLifeLibrary::ApplyActivityCompletion(UApartmentLifeNPCSimulationComponent* Simulation, FName ActivityId)
{
	if (!Simulation || ActivityId.IsNone())
	{
		return;
	}

	const FString Id = ActivityId.ToString().ToLower();

	if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("hygiene")))
	{
		Simulation->Needs.Hygiene = FMath::Clamp(Simulation->Needs.Hygiene + 35.f, 0.f, 100.f);
		Simulation->Mood.OverallMood = FMath::Clamp(Simulation->Mood.OverallMood + 5.f, 0.f, 100.f);
		Simulation->Mood.Comfort = FMath::Clamp(Simulation->Mood.Comfort + 8.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("groom")) || Id.Contains(TEXT("mirror")) || Id.Contains(TEXT("hair")) || Id.Contains(TEXT("makeup")))
	{
		Simulation->Needs.Hygiene = FMath::Clamp(Simulation->Needs.Hygiene + 10.f, 0.f, 100.f);
		Simulation->Mood.Confidence = FMath::Clamp(Simulation->Mood.Confidence + 6.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("sleep")))
	{
		Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy + 60.f, 0.f, 100.f);
		Simulation->Mood.Fatigue = FMath::Clamp(Simulation->Mood.Fatigue - 50.f, 0.f, 100.f);
		Simulation->MoodInfluences.SleepQuality = FMath::Clamp(Simulation->MoodInfluences.SleepQuality + 40.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("nap")))
	{
		Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy + 25.f, 0.f, 100.f);
		Simulation->Mood.Fatigue = FMath::Clamp(Simulation->Mood.Fatigue - 20.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("stretch")) || Id.Contains(TEXT("fitness")))
	{
		Simulation->MoodInfluences.ExerciseBenefit = FMath::Clamp(Simulation->MoodInfluences.ExerciseBenefit + 20.f, 0.f, 100.f);
		Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy + 5.f, 0.f, 100.f);
		Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress - 8.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("relax")) || Id.Contains(TEXT("read")) || Id.Contains(TEXT("tv")))
	{
		Simulation->Needs.Fun = FMath::Clamp(Simulation->Needs.Fun - 20.f, 0.f, 100.f);
		Simulation->Mood.Comfort = FMath::Clamp(Simulation->Mood.Comfort + 10.f, 0.f, 100.f);
		Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress - 5.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("cook")) || Id.Contains(TEXT("eat")))
	{
		Simulation->Needs.Hunger = FMath::Clamp(Simulation->Needs.Hunger - 35.f, 0.f, 100.f);
		Simulation->MoodInfluences.MealQuality = FMath::Clamp(Simulation->MoodInfluences.MealQuality + 15.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("wardrobe")))
	{
		Simulation->Mood.Confidence = FMath::Clamp(Simulation->Mood.Confidence + 4.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("social")) || Id.Contains(TEXT("talk")))
	{
		Simulation->ApplySocialFulfillment(12.f);
		Simulation->AffectionTowardPlayer = FMath::Clamp(Simulation->AffectionTowardPlayer + 2.f, 0.f, 100.f);
	}
	else if (Id.Contains(TEXT("clean")))
	{
		Simulation->Apartment.Cleanliness = FMath::Clamp(Simulation->Apartment.Cleanliness + 8.f, 0.f, 100.f);
		Simulation->MoodInfluences.ApartmentCleanliness = FMath::Clamp(Simulation->MoodInfluences.ApartmentCleanliness + 10.f, 0.f, 100.f);
	}

	if (IsComputerWorkActivity(ActivityId))
	{
		const float Payout = ComputeComputerWorkPayout(Simulation, ActivityId);
		Simulation->Finance.Savings += Payout;
		Simulation->MoodInfluences.WorkSatisfaction = FMath::Clamp(Simulation->MoodInfluences.WorkSatisfaction + 8.f, 0.f, 100.f);
		Simulation->Mood.Motivation = FMath::Clamp(Simulation->Mood.Motivation + 4.f, 0.f, 100.f);
		if (Payout > 0.f)
		{
			Simulation->MoodInfluences.FinancialSecurity = FMath::Clamp(Simulation->MoodInfluences.FinancialSecurity + 3.f, 0.f, 100.f);
		}
	}

	Simulation->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(
		Simulation->Mood, Simulation->MoodInfluences, Simulation->Personality);
}

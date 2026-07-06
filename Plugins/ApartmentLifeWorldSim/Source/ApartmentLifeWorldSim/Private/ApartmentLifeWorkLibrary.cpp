// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeUnlockLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

namespace
{
	FApartmentLifeWorkTypeDefinition MakeWork(
		FName Id,
		FName ActivityId,
		const TCHAR* Name,
		EApartmentLifeSkill Skill,
		float BasePayout,
		float MinSkill = 1.f,
		float Unlock = 0.f)
	{
		FApartmentLifeWorkTypeDefinition W;
		W.WorkTypeId = Id;
		W.ActivityId = ActivityId;
		W.DisplayName = FText::FromString(Name);
		W.RequiredSkill = Skill;
		W.MinSkillLevel = MinSkill;
		W.BasePayout = BasePayout;
		W.SavingsUnlockThreshold = Unlock;
		return W;
	}
}

TArray<FApartmentLifeWorkTypeDefinition> UApartmentLifeWorkLibrary::GetBuiltinWorkTypes()
{
	return {
		MakeWork(TEXT("work.programming"), TEXT("activity.work.programming"), TEXT("Programming Task"), EApartmentLifeSkill::Programming, 55.f),
		MakeWork(TEXT("work.digital_art"), TEXT("activity.work.digital_art"), TEXT("Digital Art Commission"), EApartmentLifeSkill::Art, 48.f, 1.f, 200.f),
		MakeWork(TEXT("work.tutoring"), TEXT("activity.work.tutoring"), TEXT("Online Tutoring"), EApartmentLifeSkill::Communication, 42.f, 1.f, 400.f),
		MakeWork(TEXT("work.content_edit"), TEXT("activity.work.content_edit"), TEXT("Content Editing"), EApartmentLifeSkill::Organization, 35.f, 1.f, 300.f),
		MakeWork(TEXT("work.virtual_assistant"), TEXT("activity.work.virtual_assistant"), TEXT("Virtual Assistant Task"), EApartmentLifeSkill::Organization, 30.f),
		MakeWork(TEXT("work.data_entry"), TEXT("activity.work.data_entry"), TEXT("Data Entry"), EApartmentLifeSkill::Organization, 28.f),
		MakeWork(TEXT("work.design"), TEXT("activity.work.design"), TEXT("Freelance Design"), EApartmentLifeSkill::Art, 50.f, 3.f, 600.f),
		MakeWork(TEXT("work.writing"), TEXT("activity.work.writing"), TEXT("Writing Task"), EApartmentLifeSkill::Creativity, 38.f, 1.f, 250.f),
		MakeWork(TEXT("work.computer"), TEXT("activity.work.computer"), TEXT("Remote Work"), EApartmentLifeSkill::Programming, 45.f),
		MakeWork(TEXT("work.freelance"), TEXT("activity.work.freelance"), TEXT("Freelance Task"), EApartmentLifeSkill::Creativity, 40.f, 2.f, 150.f)
	};
}

bool UApartmentLifeWorkLibrary::TryGetWorkType(FName WorkTypeId, FApartmentLifeWorkTypeDefinition& OutDefinition)
{
	for (const FApartmentLifeWorkTypeDefinition& W : GetBuiltinWorkTypes())
	{
		if (W.WorkTypeId == WorkTypeId)
		{
			OutDefinition = W;
			return true;
		}
	}
	return false;
}

bool UApartmentLifeWorkLibrary::TryGetWorkTypeForActivity(FName ActivityId, FApartmentLifeWorkTypeDefinition& OutDefinition)
{
	for (const FApartmentLifeWorkTypeDefinition& W : GetBuiltinWorkTypes())
	{
		if (W.ActivityId == ActivityId)
		{
			OutDefinition = W;
			return true;
		}
	}
	return false;
}

bool UApartmentLifeWorkLibrary::IsComputerWorkActivity(FName ActivityId)
{
	FApartmentLifeWorkTypeDefinition Temp;
	return TryGetWorkTypeForActivity(ActivityId, Temp);
}

TArray<FApartmentLifeWorkTypeDefinition> UApartmentLifeWorkLibrary::GetAvailableWorkTypes(
	const UApartmentLifeNPCSimulationComponent* Simulation,
	const UApartmentLifeProgressionComponent* Progression)
{
	TArray<FApartmentLifeWorkTypeDefinition> Results;
	const float Savings = Simulation ? Simulation->Finance.Savings : 0.f;

	for (const FApartmentLifeWorkTypeDefinition& W : GetBuiltinWorkTypes())
	{
		if (W.SavingsUnlockThreshold > Savings)
		{
			continue;
		}
		if (Simulation && Simulation->Skills.GetSkill(W.RequiredSkill) < W.MinSkillLevel)
		{
			continue;
		}
		if (Progression && !Progression->IsWorkTypeUnlocked(W.WorkTypeId))
		{
			continue;
		}
		Results.Add(W);
	}
	return Results;
}

FApartmentLifeWorkSessionResult UApartmentLifeWorkLibrary::ComputeWorkSessionResult(
	UApartmentLifeNPCSimulationComponent* Simulation,
	UApartmentLifeProgressionComponent* Progression,
	FName ActivityId)
{
	FApartmentLifeWorkSessionResult Result;
	Result.ActivityId = ActivityId;

	FApartmentLifeWorkTypeDefinition Work;
	if (!TryGetWorkTypeForActivity(ActivityId, Work))
	{
		return Result;
	}

	Result.WorkTypeId = Work.WorkTypeId;
	Result.MinutesPassed = Work.DurationMinutes;
	Result.bSuccess = Simulation != nullptr;

	if (!Simulation)
	{
		return Result;
	}

	const float SkillLevel = Simulation->Skills.GetSkill(Work.RequiredSkill);
	Result.SkillMultiplier = 1.f + (SkillLevel - 1.f) * 0.02f;
	Result.MoodModifier = Simulation->Mood.GetProductivityMultiplier();
	Result.EnergyModifier = FMath::Clamp(Simulation->Mood.Energy / 100.f, 0.3f, 1.f);

	float EquipmentBonus = 1.f;
	if (Progression)
	{
		EquipmentBonus += Progression->GetComputerQuality().GetProductivityBonus();
	}

	Result.BaseIncome = Work.BasePayout * (Work.DurationMinutes / 60.f);
	Result.TotalIncome = Result.BaseIncome * Result.SkillMultiplier * Result.MoodModifier * Result.EnergyModifier * EquipmentBonus;

	if (FMath::FRand() < Work.BonusChance * (1.f + SkillLevel * 0.01f))
	{
		Result.BonusIncome = Result.TotalIncome * 0.25f;
		Result.TotalIncome += Result.BonusIncome;
	}

	if (FMath::FRand() > Work.SuccessChance + SkillLevel * 0.005f)
	{
		Result.bSuccess = false;
		Result.TotalIncome *= 0.5f;
	}

	float StressReduction = Progression ? Progression->GetComputerQuality().GetStressReductionBonus() : 0.f;
	Result.StressChange = FMath::Max(0.f, Work.StressGain - StressReduction * 5.f);
	Result.SkillXpGained = Work.SkillXpGain * (1.f + (Progression ? Progression->GetComputerQuality().GetSkillXpBonus() : 0.f));

	return Result;
}

void UApartmentLifeWorkLibrary::ApplyWorkSessionResult(
	UApartmentLifeNPCSimulationComponent* Simulation,
	UApartmentLifeProgressionComponent* Progression,
	const FApartmentLifeWorkSessionResult& Result)
{
	if (!Simulation)
	{
		return;
	}

	Simulation->Finance.Savings += Result.TotalIncome;
	Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy - 10.f * Result.EnergyModifier, 0.f, 100.f);
	Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress + Result.StressChange, 0.f, 100.f);
	Simulation->Mood.Motivation = FMath::Clamp(Simulation->Mood.Motivation + 4.f, 0.f, 100.f);
	Simulation->MoodInfluences.WorkSatisfaction = FMath::Clamp(Simulation->MoodInfluences.WorkSatisfaction + 8.f, 0.f, 100.f);
	Simulation->MoodInfluences.FinancialSecurity = FMath::Clamp(Simulation->MoodInfluences.FinancialSecurity + 3.f, 0.f, 100.f);

	FApartmentLifeWorkTypeDefinition Work;
	if (TryGetWorkType(Result.WorkTypeId, Work))
	{
		Simulation->GainSkill(Work.RequiredSkill, Result.SkillXpGained);
	}

	if (Progression)
	{
		FApartmentLifeWorkSessionResult FinalResult = Result;
		FinalResult.NewUnlocks = UApartmentLifeUnlockLibrary::EvaluateUnlocks(Simulation, Progression);
		Progression->RecordWorkSession(FinalResult);
	}
}

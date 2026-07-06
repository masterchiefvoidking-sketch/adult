// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorldSimTypes.h"

EApartmentLifeMoodLevel FApartmentLifeMoodState::GetMoodLevel() const
{
	if (OverallMood < 20.f) return EApartmentLifeMoodLevel::Miserable;
	if (OverallMood < 40.f) return EApartmentLifeMoodLevel::Low;
	if (OverallMood < 60.f) return EApartmentLifeMoodLevel::Neutral;
	if (OverallMood < 80.f) return EApartmentLifeMoodLevel::Good;
	return EApartmentLifeMoodLevel::Excellent;
}

float FApartmentLifeMoodState::GetProductivityMultiplier() const
{
	return FMath::Clamp(0.5f + OverallMood / 100.f - Stress / 200.f, 0.25f, 1.25f);
}

float FApartmentLifeMoodState::GetSocialDriveMultiplier() const
{
	return FMath::Clamp(OverallMood / 100.f + Energy / 200.f, 0.1f, 1.5f);
}

float FApartmentLifeCareerState::ComputeMonthlyIncome() const
{
	if (bIsSalaried)
	{
		return MonthlySalary;
	}
	return HourlyWage * 8.f * 22.f;
}

void FApartmentLifeSkillSet::GainSkill(EApartmentLifeSkill Skill, float Amount)
{
	auto Apply = [Amount](float& Level) { Level = FMath::Clamp(Level + Amount, 1.f, 100.f); };
	switch (Skill)
	{
	case EApartmentLifeSkill::Cooking: Apply(Cooking); break;
	case EApartmentLifeSkill::Fitness: Apply(Fitness); break;
	case EApartmentLifeSkill::Creativity: Apply(Creativity); break;
	case EApartmentLifeSkill::Programming: Apply(Programming); break;
	case EApartmentLifeSkill::Art: Apply(Art); break;
	case EApartmentLifeSkill::Music: Apply(Music); break;
	case EApartmentLifeSkill::Organization: Apply(Organization); break;
	case EApartmentLifeSkill::Communication: Apply(Communication); break;
	case EApartmentLifeSkill::Leadership: Apply(Leadership); break;
	case EApartmentLifeSkill::HomeMaintenance: Apply(HomeMaintenance); break;
	}
}

float FApartmentLifeSkillSet::GetSkill(EApartmentLifeSkill Skill) const
{
	switch (Skill)
	{
	case EApartmentLifeSkill::Cooking: return Cooking;
	case EApartmentLifeSkill::Fitness: return Fitness;
	case EApartmentLifeSkill::Creativity: return Creativity;
	case EApartmentLifeSkill::Programming: return Programming;
	case EApartmentLifeSkill::Art: return Art;
	case EApartmentLifeSkill::Music: return Music;
	case EApartmentLifeSkill::Organization: return Organization;
	case EApartmentLifeSkill::Communication: return Communication;
	case EApartmentLifeSkill::Leadership: return Leadership;
	case EApartmentLifeSkill::HomeMaintenance: return HomeMaintenance;
	default: return 1.f;
	}
}

float FApartmentLifeFinancialLedger::GetMonthlyFixedExpenses() const
{
	return MonthlyRent + MonthlyUtilities + MonthlyFood + MonthlyInternet + MonthlyTransportation;
}

float FApartmentLifeFinancialLedger::GetDisposableIncome(float MonthlyIncome) const
{
	return MonthlyIncome - GetMonthlyFixedExpenses() - MonthlyEntertainment;
}

bool FApartmentLifeFinancialLedger::CanAfford(float Amount) const
{
	return Savings >= Amount;
}

float FApartmentLifeRelationshipRecord::GetInvitationScore() const
{
	return (Friendship * 0.3f + Trust * 0.2f + Comfort * 0.2f + Compatibility * 0.2f + SharedInterests * 0.1f) / 100.f;
}

void FApartmentLifeRelationshipRecord::RecordConversation(float Quality)
{
	++ConversationCount;
	const float Delta = FMath::Clamp(Quality, -1.f, 1.f) * 5.f;
	Familiarity = FMath::Clamp(Familiarity + FMath::Abs(Delta) * 0.5f, 0.f, 100.f);
	Friendship = FMath::Clamp(Friendship + Delta, 0.f, 100.f);
	Trust = FMath::Clamp(Trust + Delta * 0.5f, 0.f, 100.f);
	Comfort = FMath::Clamp(Comfort + Delta * 0.3f, 0.f, 100.f);
}

void FApartmentLifeRelationshipRecord::RecordSharedActivity(float Quality)
{
	++SharedActivityCount;
	const float Delta = FMath::Clamp(Quality, -1.f, 1.f) * 6.f;
	Familiarity = FMath::Clamp(Familiarity + FMath::Abs(Delta) * 0.6f, 0.f, 100.f);
	Friendship = FMath::Clamp(Friendship + Delta, 0.f, 100.f);
	Trust = FMath::Clamp(Trust + Delta * 0.4f, 0.f, 100.f);
	Comfort = FMath::Clamp(Comfort + Delta * 0.5f, 0.f, 100.f);
	SharedInterests = FMath::Clamp(SharedInterests + Delta * 0.3f, 0.f, 100.f);
}

float FApartmentLifeMemoryRecord::GetRecencyWeight(const FApartmentLifeGameTime& CurrentTime) const
{
	const int64 MinuteDelta = FMath::Max<int64>(0, CurrentTime.TotalMinutes - Timestamp.TotalMinutes);
	const float Recency = FMath::Clamp(1.f - static_cast<float>(MinuteDelta) / (90.f * 24.f * 60.f), 0.1f, 1.f);
	return Strength * Recency * (1.f + FMath::Abs(EmotionalWeight) * 0.25f);
}

void FApartmentLifeMemoryRecord::ApplyDecay(float DecayRate)
{
	const float ReinforcementProtection = FMath::Min(static_cast<float>(ReinforcementCount) * 0.05f, 0.5f);
	Strength = FMath::Max(0.f, Strength - DecayRate * (1.f - ReinforcementProtection));
}

void FApartmentLifeMemoryRecord::Reinforce(float Amount)
{
	Strength = FMath::Clamp(Strength + Amount, 0.f, 1.f);
	++ReinforcementCount;
}

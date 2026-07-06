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
	Friendship = FMath::Clamp(Friendship + Delta, 0.f, 100.f);
	Trust = FMath::Clamp(Trust + Delta * 0.5f, 0.f, 100.f);
	Comfort = FMath::Clamp(Comfort + Delta * 0.3f, 0.f, 100.f);
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

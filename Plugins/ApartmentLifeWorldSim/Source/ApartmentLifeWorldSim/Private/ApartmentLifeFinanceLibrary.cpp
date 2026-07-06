// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"

void UApartmentLifeFinanceLibrary::RecordIncome(UApartmentLifeProgressionComponent* Progression, float Amount)
{
	if (Progression)
	{
		Progression->AddDailyIncome(Amount);
	}
}

void UApartmentLifeFinanceLibrary::RecordSpending(
	UApartmentLifeProgressionComponent* Progression,
	EApartmentLifeShopCategory Category,
	float Amount)
{
	if (!Progression)
	{
		return;
	}

	FApartmentLifeDailyBudget& Budget = Progression->GetMutableDailyBudget();
	switch (Category)
	{
	case EApartmentLifeShopCategory::Clothing: Budget.ClothingSpent += Amount; break;
	case EApartmentLifeShopCategory::Furniture: Budget.FurnitureSpent += Amount; break;
	case EApartmentLifeShopCategory::Decorations: Budget.DecorationSpent += Amount; break;
	case EApartmentLifeShopCategory::Groceries: Budget.FoodSpent += Amount; break;
	default: Budget.EmergencySpent += Amount; break;
	}
}

FApartmentLifeDailyBudget UApartmentLifeFinanceLibrary::GetCurrentBudget(const UApartmentLifeProgressionComponent* Progression)
{
	return Progression ? Progression->GetDailyBudget() : FApartmentLifeDailyBudget();
}

void UApartmentLifeFinanceLibrary::AdvanceDailyBudget(
	UApartmentLifeProgressionComponent* Progression,
	const UApartmentLifeNPCSimulationComponent* Simulation)
{
	if (!Progression || !Simulation)
	{
		return;
	}

	FApartmentLifeDailyBudget& Budget = Progression->GetMutableDailyBudget();
	Budget.WeeklyIncome += Budget.DailyIncome;
	Budget.RentSpent += Simulation->Finance.MonthlyRent / 30.f;
	Budget.UtilitiesSpent += Simulation->Finance.MonthlyUtilities / 30.f;
	Budget.FoodSpent += Simulation->Finance.MonthlyFood / 30.f;
	Budget.DailyIncome = 0.f;
	++Budget.DayIndex;
}

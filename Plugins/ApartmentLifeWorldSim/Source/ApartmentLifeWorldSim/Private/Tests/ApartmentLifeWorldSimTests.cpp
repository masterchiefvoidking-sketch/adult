// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeCalendarTypes.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeWorldSimLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCalendarWeekdayTest,
	"ApartmentLife.WorldSim.Calendar.WeekdayCalculation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCalendarWeekdayTest::RunTest(const FString& Parameters)
{
	const EApartmentLifeWeekday Weekday = FApartmentLifeCalendarUtils::GetWeekday(2026, 7, 6);
	TestEqual(TEXT("July 6 2026 is Monday"), Weekday, EApartmentLifeWeekday::Monday);
	TestFalse(TEXT("July 6 2026 is not weekend"), FApartmentLifeCalendarUtils::IsWeekend(2026, 7, 6));
	TestTrue(TEXT("July 5 2026 is weekend"), FApartmentLifeCalendarUtils::IsWeekend(2026, 7, 5));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeFinanceLedgerTest,
	"ApartmentLife.WorldSim.Finance.LedgerCalculations",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeFinanceLedgerTest::RunTest(const FString& Parameters)
{
	FApartmentLifeFinancialLedger Ledger;
	Ledger.MonthlyRent = 900.f;
	Ledger.MonthlyUtilities = 100.f;
	Ledger.MonthlyFood = 300.f;
	Ledger.MonthlyInternet = 60.f;
	Ledger.MonthlyTransportation = 80.f;
	Ledger.Savings = 500.f;

	TestEqual(TEXT("Fixed expenses sum"), Ledger.GetMonthlyFixedExpenses(), 1440.f);
	TestTrue(TEXT("Can afford 400"), Ledger.CanAfford(400.f));
	TestFalse(TEXT("Cannot afford 600"), Ledger.CanAfford(600.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeMoodRecalculationTest,
	"ApartmentLife.WorldSim.Mood.Recalculation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeMoodRecalculationTest::RunTest(const FString& Parameters)
{
	FApartmentLifeMoodState Mood;
	FApartmentLifeMoodInfluences Influences;
	Influences.SleepQuality = 80.f;
	Influences.FinancialSecurity = 70.f;
	Influences.SocialFulfillment = 60.f;
	FApartmentLifePersonalityTraits Personality;

	const FApartmentLifeMoodState Updated = UApartmentLifeWorldSimLibrary::RecalculateMood(Mood, Influences, Personality);
	TestTrue(TEXT("Mood improves with positive influences"), Updated.OverallMood > Mood.OverallMood);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeMemoryDecayTest,
	"ApartmentLife.WorldSim.Memory.DecayAndReinforce",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeMemoryDecayTest::RunTest(const FString& Parameters)
{
	FApartmentLifeMemoryRecord Memory;
	Memory.Strength = 1.f;
	Memory.ApplyDecay(0.1f);
	TestTrue(TEXT("Memory decays"), Memory.Strength < 1.f);

	Memory.Reinforce(0.3f);
	TestTrue(TEXT("Memory reinforces"), Memory.Strength > 0.9f);
	TestEqual(TEXT("Reinforcement count increments"), Memory.ReinforcementCount, 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeBuiltinScheduleTest,
	"ApartmentLife.WorldSim.Schedule.BuiltinSchedule",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeBuiltinScheduleTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeDailyScheduleSlot> Schedule = UApartmentLifeWorldSimLibrary::CreateBuiltinWeekdaySchedule();
	TestTrue(TEXT("Builtin schedule has entries"), Schedule.Num() > 0);
	TestEqual(TEXT("First block is sleep"), Schedule[0].BlockType, EApartmentLifeScheduleBlock::Sleep);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeGirlLifeComputerIncomeTest,
	"ApartmentLife.WorldSim.GirlLife.ComputerIncome",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeGirlLifeComputerIncomeTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Computer work activity detected"),
		UApartmentLifeGirlLifeLibrary::IsComputerWorkActivity(FName(TEXT("activity.work.computer"))));
	TestFalse(TEXT("Shower is not computer work"),
		UApartmentLifeGirlLifeLibrary::IsComputerWorkActivity(FName(TEXT("activity.hygiene.shower"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeGirlLifeShowerEffectsTest,
	"ApartmentLife.WorldSim.GirlLife.ShowerCompletion",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeGirlLifeShowerEffectsTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();
	Sim->Needs.Hygiene = 40.f;
	UApartmentLifeGirlLifeLibrary::ApplyActivityCompletion(Sim, FName(TEXT("activity.hygiene.shower")));
	TestTrue(TEXT("Hygiene improves after shower"), Sim->Needs.Hygiene > 40.f);
	return true;
}

#endif

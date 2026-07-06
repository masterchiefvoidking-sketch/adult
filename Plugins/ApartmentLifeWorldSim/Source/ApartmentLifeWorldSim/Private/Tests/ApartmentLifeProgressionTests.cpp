// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeUnlockLibrary.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWorkTypeCatalogTest,
	"ApartmentLife.Progression.WorkTypeCatalog",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWorkTypeCatalogTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeWorkTypeDefinition> WorkTypes = UApartmentLifeWorkLibrary::GetBuiltinWorkTypes();
	TestTrue(TEXT("Builtin work types exist"), WorkTypes.Num() >= 8);

	FApartmentLifeWorkTypeDefinition Definition;
	TestTrue(TEXT("Programming work type resolves"),
		UApartmentLifeWorkLibrary::TryGetWorkType(FName(TEXT("work.programming")), Definition));
	TestEqual(TEXT("Programming maps to activity"),
		Definition.ActivityId, FName(TEXT("activity.work.programming")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWorkSessionIncomeTest,
	"ApartmentLife.Progression.WorkSessionIncome",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWorkSessionIncomeTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Simulation = NewObject<UApartmentLifeNPCSimulationComponent>();
	Simulation->Finance.Savings = 100.f;
	Simulation->Mood.Energy = 80.f;
	Simulation->Skills.GainSkill(EApartmentLifeSkill::Programming, 5.f);

	UApartmentLifeProgressionComponent* Progression = NewObject<UApartmentLifeProgressionComponent>();
	Progression->SeedStarterUnlocks();

	const float SavingsBefore = Simulation->Finance.Savings;
	const FApartmentLifeWorkSessionResult Result = UApartmentLifeWorkLibrary::ComputeWorkSessionResult(
		Simulation, Progression, FName(TEXT("activity.work.computer")));

	TestTrue(TEXT("Work session produces income"), Result.TotalIncome > 0.f);
	UApartmentLifeWorkLibrary::ApplyWorkSessionResult(Simulation, Progression, Result);
	TestTrue(TEXT("Savings increase after work"), Simulation->Finance.Savings > SavingsBefore);
	TestTrue(TEXT("Stress increases from work"), Simulation->Mood.Stress > 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeShopPurchaseTest,
	"ApartmentLife.Progression.ShopPurchase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeShopPurchaseTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Simulation = NewObject<UApartmentLifeNPCSimulationComponent>();
	Simulation->Finance.Savings = 500.f;

	UApartmentLifeProgressionComponent* Progression = NewObject<UApartmentLifeProgressionComponent>();
	Progression->SeedStarterUnlocks();

	const FName ItemId = FName(TEXT("shop.electronics.computer.budget"));
	TestTrue(TEXT("Shop purchase succeeds"), Progression->TryPurchaseShopItem(Simulation, ItemId));
	TestTrue(TEXT("Item is owned"), Progression->OwnsShopItem(ItemId));
	TestFalse(TEXT("Cannot purchase same item twice"), Progression->TryPurchaseShopItem(Simulation, ItemId));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeUnlockThresholdTest,
	"ApartmentLife.Progression.SavingsUnlocks",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeUnlockThresholdTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Simulation = NewObject<UApartmentLifeNPCSimulationComponent>();
	UApartmentLifeProgressionComponent* Progression = NewObject<UApartmentLifeProgressionComponent>();
	Progression->SeedStarterUnlocks();

	Simulation->Finance.Savings = 250.f;
	const TArray<FName> Unlocks = UApartmentLifeUnlockLibrary::EvaluateUnlocks(Simulation, Progression);
	TestTrue(TEXT("Digital art unlocks at savings threshold"),
		Progression->IsWorkTypeUnlocked(FName(TEXT("work.digital_art"))));
	TestTrue(TEXT("Unlock list reports new work type"), Unlocks.Contains(FName(TEXT("work.digital_art"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeDailyBudgetTest,
	"ApartmentLife.Progression.DailyBudget",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeDailyBudgetTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Simulation = NewObject<UApartmentLifeNPCSimulationComponent>();
	Simulation->Finance.MonthlyRent = 900.f;
	Simulation->Finance.MonthlyUtilities = 120.f;
	Simulation->Finance.MonthlyFood = 300.f;

	UApartmentLifeProgressionComponent* Progression = NewObject<UApartmentLifeProgressionComponent>();
	Progression->AddDailyIncome(75.f);
	UApartmentLifeFinanceLibrary::AdvanceDailyBudget(Progression, Simulation);

	const FApartmentLifeDailyBudget Budget = UApartmentLifeFinanceLibrary::GetCurrentBudget(Progression);
	TestEqual(TEXT("Daily income rolls into weekly"), Budget.WeeklyIncome, 75.f);
	TestEqual(TEXT("Daily income resets"), Budget.DailyIncome, 0.f);
	TestTrue(TEXT("Rent tracked per day"), Budget.RentSpent > 0.f);
	return true;
}

#endif

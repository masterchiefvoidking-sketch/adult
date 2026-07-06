// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeQAWardrobeRoomTest,
	"ApartmentLife.QA.WardrobeActivityRoom",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeQAWardrobeRoomTest::RunTest(const FString& Parameters)
{
	FApartmentLifeActivityDefinition Def;
	TestTrue(TEXT("Wardrobe activity exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.dress.wardrobe")), Def));
	TestEqual(TEXT("Wardrobe activity allows any room"), Def.RequiredRoom, EApartmentLifeActivityRoom::Any);
	TestTrue(TEXT("Wardrobe can start from bedroom closet room"),
		UApartmentLifeActivityLibrary::CanStartActivity(
			FName(TEXT("activity.dress.wardrobe")), EApartmentLifeRoomType::Bedroom, nullptr));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeQARoutineRoomTest,
	"ApartmentLife.QA.RoutinePreferredRoom",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeQARoutineRoomTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Shower maps to bathroom"),
		UApartmentLifeActivityLibrary::GetPreferredRoomForActivity(FName(TEXT("activity.hygiene.shower"))),
		EApartmentLifeRoomType::Bathroom);
	TestEqual(TEXT("Work focus maps to office"),
		UApartmentLifeActivityLibrary::GetPreferredRoomForActivity(FName(TEXT("activity.work.focus"))),
		EApartmentLifeRoomType::Office);
	TestTrue(TEXT("Shower can start in bathroom"),
		UApartmentLifeActivityLibrary::CanStartActivity(
			FName(TEXT("activity.hygiene.shower")), EApartmentLifeRoomType::Bathroom, nullptr));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeQAWorkIncomeFlagTest,
	"ApartmentLife.QA.WorkIncomeFormula",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeQAWorkIncomeFlagTest::RunTest(const FString& Parameters)
{
	FApartmentLifeActivityDefinition FocusWork;
	TestTrue(TEXT("Focus work activity exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.work.focus")), FocusWork));
	TestTrue(TEXT("Focus work uses income formula"), FocusWork.bUsesIncomeFormula);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeQASaveRoundTripStressTest,
	"ApartmentLife.QA.SaveRoundTripStress",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeQASaveRoundTripStressTest::RunTest(const FString& Parameters)
{
	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();

	const float InitialSavings = 2500.f;
	Sim->Finance.Savings = InitialSavings;

	for (int32 Cycle = 0; Cycle < 10; ++Cycle)
	{
		TMap<FString, FString> WardrobeData;
		TMap<FString, FString> SimData;
		Wardrobe->CaptureSaveData(WardrobeData);
		Sim->CaptureSaveData(SimData);

		Wardrobe->RestoreSaveData(WardrobeData);
		Sim->RestoreSaveData(SimData);

		Sim->Finance.Savings += 100.f;
	}

	TestEqual(TEXT("Savings survive 10 round-trips"), Sim->Finance.Savings, InitialSavings + 1000.f);
	return true;
}

#endif

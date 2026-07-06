// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeCityLibrary.h"
#include "ApartmentLifeCityTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeDayPhaseTest,
	"ApartmentLife.City.DayPhase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeDayPhaseTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Morning rush"), UApartmentLifeCityLibrary::GetDayPhaseForHour(7), EApartmentLifeDayPhase::MorningRush);
	TestEqual(TEXT("Late night"), UApartmentLifeCityLibrary::GetDayPhaseForHour(23), EApartmentLifeDayPhase::LateNight);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCommuteTest,
	"ApartmentLife.City.Commute",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCommuteTest::RunTest(const FString& Parameters)
{
	const float Minutes = UApartmentLifeCityLibrary::ComputeCommuteMinutes(
		FName(TEXT("district.suburbs")),
		FName(TEXT("district.downtown")),
		EApartmentLifeTransportMode::PublicTransit);
	TestTrue(TEXT("Commute takes time"), Minutes > 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeSimulationLODTest,
	"ApartmentLife.City.SimulationLOD",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeSimulationLODTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Player district is full LOD"),
		UApartmentLifeCityLibrary::ComputeSimulationLOD(8000.f, true),
		EApartmentLifeSimulationLOD::Full);
	TestEqual(TEXT("Far district is dormant"),
		UApartmentLifeCityLibrary::ComputeSimulationLOD(15000.f, false),
		EApartmentLifeSimulationLOD::Dormant);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeBuildingHoursTest,
	"ApartmentLife.City.BuildingHours",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeBuildingHoursTest::RunTest(const FString& Parameters)
{
	FApartmentLifeBuildingHours Hours;
	Hours.OpenHour = 9;
	Hours.CloseHour = 18;
	TestTrue(TEXT("Open at noon"), Hours.IsOpenAtHour(12));
	TestFalse(TEXT("Closed at midnight"), Hours.IsOpenAtHour(0));
	return true;
}

#endif

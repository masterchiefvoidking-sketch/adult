// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeRoutineChainComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityCatalogTest,
	"ApartmentLife.Activities.CatalogCoverage",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityCatalogTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeActivityDefinition> Catalog = UApartmentLifeActivityCatalogLibrary::GetBuiltinCatalog();
	TestTrue(TEXT("Catalog has activities"), Catalog.Num() >= 40);

	TestTrue(TEXT("Sleep activity exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.sleep.bed")), FApartmentLifeActivityDefinition()));
	TestTrue(TEXT("Yoga activity exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.fitness.yoga")), FApartmentLifeActivityDefinition()));
	TestTrue(TEXT("Computer work exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.work.computer")), FApartmentLifeActivityDefinition()));

	const TArray<FApartmentLifeActivityDefinition> Bedroom = UApartmentLifeActivityCatalogLibrary::GetActivitiesForRoom(EApartmentLifeActivityRoom::Bedroom);
	TestTrue(TEXT("Bedroom has activities"), Bedroom.Num() >= 3);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityCompletionTest,
	"ApartmentLife.Activities.CompletionEffects",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityCompletionTest::RunTest(const FString& Parameters)
{
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();
	Sim->Mood.Energy = 40.f;
	Sim->Needs.Hygiene = 30.f;

	const FApartmentLifeActivityCompletionResult Result = UApartmentLifeActivityLibrary::ApplyActivityCompletion(
		Sim, FName(TEXT("activity.hygiene.shower")));

	TestTrue(TEXT("Shower completion succeeds"), Result.bSuccess);
	TestTrue(TEXT("Hygiene improves after shower"), Sim->Needs.Hygiene > 30.f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityComponentTest,
	"ApartmentLife.Activities.ComponentTimer",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityComponentTest::RunTest(const FString& Parameters)
{
	UApartmentLifeActivityComponent* Activity = NewObject<UApartmentLifeActivityComponent>();
	TestTrue(TEXT("Can start catalog activity"), Activity->StartActivity(FName(TEXT("activity.hygiene.brush_teeth"))));
	TestTrue(TEXT("Activity is active"), Activity->IsActivityActive());
	TestEqual(TEXT("Duration from catalog"), Activity->GetRemainingMinutes(), 5);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeRoutineChainTest,
	"ApartmentLife.Activities.RoutineChain",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeRoutineChainTest::RunTest(const FString& Parameters)
{
	FApartmentLifeRoutineChainDefinition Chain;
	TestTrue(TEXT("Morning routine exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(FName(TEXT("routine.morning")), Chain));
	TestTrue(TEXT("Morning routine has steps"), Chain.ActivityIds.Num() >= 4);

	TestTrue(TEXT("Evening routine exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(FName(TEXT("routine.evening")), Chain));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivitySaveTest,
	"ApartmentLife.Activities.SaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivitySaveTest::RunTest(const FString& Parameters)
{
	UApartmentLifeActivityComponent* Activity = NewObject<UApartmentLifeActivityComponent>();
	Activity->StartActivity(FName(TEXT("activity.read.book")));

	TMap<FString, FString> SaveData;
	Activity->CaptureSaveData_Implementation(SaveData);

	UApartmentLifeActivityComponent* Restored = NewObject<UApartmentLifeActivityComponent>();
	Restored->RestoreSaveData_Implementation(SaveData);

	TestTrue(TEXT("Activity active after restore"), Restored->IsActivityActive());
	TestEqual(TEXT("Activity ID restored"), Restored->GetCurrentActivityId(), FName(TEXT("activity.read.book")));
	return true;
}

#endif

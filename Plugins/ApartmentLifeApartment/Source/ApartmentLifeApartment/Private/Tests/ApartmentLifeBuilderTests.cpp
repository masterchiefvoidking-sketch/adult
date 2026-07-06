// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "ApartmentLifeApartmentTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeGridSnapTest,
	"ApartmentLife.Builder.GridSnap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeGridSnapTest::RunTest(const FString& Parameters)
{
	const FTransform Input(FRotator(0.f, 47.f, 0.f), FVector(103.f, 87.f, 5.f));
	const FTransform Snapped = UApartmentLifeBuilderLibrary::SnapTransformToGrid(Input, 25.f);
	TestEqual(TEXT("X snapped"), FMath::Fmod(Snapped.GetLocation().X, 25.f), 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeRoomScoreTest,
	"ApartmentLife.Builder.RoomScore",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeRoomScoreTest::RunTest(const FString& Parameters)
{
	FApartmentLifeRoomScore Score;
	Score.Comfort = 80.f;
	Score.Cleanliness = 70.f;
	Score.Clutter = 10.f;
	TestTrue(TEXT("Overall score positive"), Score.GetOverallScore() > 0.f);
	TestTrue(TEXT("Mood impact positive"), Score.GetMoodImpact() > 0.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeInteractionCategoryTest,
	"ApartmentLife.Builder.InteractionHooks",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeInteractionCategoryTest::RunTest(const FString& Parameters)
{
	const TArray<EApartmentLifeFurnitureInteraction> BedInteractions =
		UApartmentLifeBuilderLibrary::GetInteractionsForCategory(EApartmentLifeFurnitureCategory::Bed);
	TestTrue(TEXT("Bed has sleep"), BedInteractions.Contains(EApartmentLifeFurnitureInteraction::Sleep));
	return true;
}

#endif

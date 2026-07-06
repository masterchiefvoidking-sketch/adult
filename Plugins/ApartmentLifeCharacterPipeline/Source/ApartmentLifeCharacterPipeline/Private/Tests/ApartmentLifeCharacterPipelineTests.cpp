// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeAnimationData.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityAnimationMapTest,
	"ApartmentLife.CharacterPipeline.ActivityAnimationMap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityAnimationMapTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Yoga maps correctly"),
		UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(FName(TEXT("activity.yoga.morning"))),
		EApartmentLifeAnimationGroup::Yoga);
	TestEqual(TEXT("Cooking maps correctly"),
		UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(FName(TEXT("activity.cook.dinner"))),
		EApartmentLifeAnimationGroup::Cooking);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeYogaPoseMatchTest,
	"ApartmentLife.CharacterPipeline.YogaPoseMatch",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeYogaPoseMatchTest::RunTest(const FString& Parameters)
{
	FApartmentLifeYogaSessionState State;
	State.bOnYogaMat = true;
	State.PoseMatchScore = 0.f;

	UApartmentLifeYogaPoseData* Pose = NewObject<UApartmentLifeYogaPoseData>();
	Pose->Difficulty = 0.6f;
	Pose->FlexibilityReward = 5.f;

	const FApartmentLifeYogaSessionState Updated =
		UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(State, Pose, 0.9f, 1.f);

	TestTrue(TEXT("Pose match improves with good input"), Updated.PoseMatchScore > State.PoseMatchScore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeGroomingAdvanceTest,
	"ApartmentLife.CharacterPipeline.GroomingAdvance",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeGroomingAdvanceTest::RunTest(const FString& Parameters)
{
	FApartmentLifeGroomingRoutineState State;
	State.CurrentStep = EApartmentLifeGroomingStep::Shower;
	State.Hygiene = 50.f;

	UApartmentLifeGroomingRoutineData* Routine = NewObject<UApartmentLifeGroomingRoutineData>();
	Routine->Steps = { EApartmentLifeGroomingStep::Shower, EApartmentLifeGroomingStep::BrushTeeth };
	Routine->HygieneGainPerStep = 10.f;

	const FApartmentLifeGroomingRoutineState Updated =
		UApartmentLifeCharacterPipelineLibrary::AdvanceGroomingStep(State, Routine);

	TestEqual(TEXT("Advances to brush teeth"), Updated.CurrentStep, EApartmentLifeGroomingStep::BrushTeeth);
	TestEqual(TEXT("Hygiene increases"), Updated.Hygiene, 60.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeBodyFitProfileTest,
	"ApartmentLife.CharacterPipeline.BodyFitProfile",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeBodyFitProfileTest::RunTest(const FString& Parameters)
{
	FApartmentLifeBodyCustomization Body;
	Body.HeightCm = 180.f;
	Body.ShoulderWidth = 0.8f;

	const FVector Scale = Body.GetScaleVector();
	TestTrue(TEXT("Taller body scales up"), Scale.Z > 1.f);
	return true;
}

#endif

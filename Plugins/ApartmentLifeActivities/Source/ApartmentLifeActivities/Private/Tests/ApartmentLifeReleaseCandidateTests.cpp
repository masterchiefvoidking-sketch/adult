// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeYogaPoseData.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobeAutoRecommendTest,
	"ApartmentLife.ReleaseCandidate.WardrobeAutoRecommend",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobeAutoRecommendTest::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Wardrobe activity skips auto outfit"),
		UApartmentLifeWardrobeLibrary::ShouldAutoRecommendOutfitForActivity(FName(TEXT("activity.dress.wardrobe"))));
	TestFalse(TEXT("Preview activity skips auto outfit"),
		UApartmentLifeWardrobeLibrary::ShouldAutoRecommendOutfitForActivity(FName(TEXT("activity.dress.preview"))));
	TestTrue(TEXT("Yoga still auto-recommends"),
		UApartmentLifeWardrobeLibrary::ShouldAutoRecommendOutfitForActivity(FName(TEXT("activity.yoga.morning"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeYogaBreathingPhaseTest,
	"ApartmentLife.ReleaseCandidate.YogaBreathingPhase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeYogaBreathingPhaseTest::RunTest(const FString& Parameters)
{
	FApartmentLifeYogaSessionState State;
	State.bOnYogaMat = true;

	UApartmentLifeYogaPoseData* Pose = NewObject<UApartmentLifeYogaPoseData>();
	Pose->Difficulty = 0.5f;
	Pose->FlexibilityReward = 4.f;

	const FApartmentLifeYogaSessionState First =
		UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(State, Pose, 0.8f, 0.5f);
	const FApartmentLifeYogaSessionState Second =
		UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(First, Pose, 0.8f, 0.5f);

	TestTrue(TEXT("Breathing phase advances with delta time"), Second.BreathingPhase > First.BreathingPhase);
	TestNotEqual(TEXT("Breathing rhythm responds to phase"), First.BreathingRhythm, Second.BreathingRhythm);
	return true;
}

#endif

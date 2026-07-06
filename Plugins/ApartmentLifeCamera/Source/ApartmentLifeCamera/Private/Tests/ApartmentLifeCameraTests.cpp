// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeActivityCameraLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityCameraFrameTest,
	"ApartmentLife.Camera.ActivityFraming",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityCameraFrameTest::RunTest(const FString& Parameters)
{
	const FApartmentLifeActivityCameraFrame YogaFrame =
		UApartmentLifeActivityCameraLibrary::GetFrameForActivity(FName(TEXT("activity.fitness.yoga")));
	TestTrue(TEXT("Yoga framing uses full body distance"), YogaFrame.ArmLength >= 400.f);
	TestTrue(TEXT("Yoga allows manual orbit"), YogaFrame.bAllowManualOrbit);

	const FApartmentLifeActivityCameraFrame ShowerFrame =
		UApartmentLifeActivityCameraLibrary::GetFrameForActivity(FName(TEXT("activity.hygiene.shower")));
	TestTrue(TEXT("Shower uses privacy framing"), ShowerFrame.bPrivacyFraming);

	TestEqual(TEXT("Wardrobe activity maps to wardrobe mode"),
		UApartmentLifeActivityCameraLibrary::GetPrimaryModeForActivity(FName(TEXT("activity.dress.wardrobe"))),
		EApartmentLifePrimaryCameraMode::Wardrobe);

	return true;
}

#endif

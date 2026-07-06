// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeUiTypes.h"
#include "ApartmentLifeUiOverlayGate.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeUiScreenEnumTest,
	"ApartmentLife.UI.ScreenEnumCoverage",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeUiScreenEnumTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Main menu screen exists"), StaticEnum<EApartmentLifeUiScreen>()->GetIndexByName(TEXT("EApartmentLifeUiScreen::MainMenu")) != INDEX_NONE);
	TestTrue(TEXT("Wardrobe screen exists"), StaticEnum<EApartmentLifeUiScreen>()->GetIndexByName(TEXT("EApartmentLifeUiScreen::Wardrobe")) != INDEX_NONE);
	TestTrue(TEXT("Build mode screen exists"), StaticEnum<EApartmentLifeUiScreen>()->GetIndexByName(TEXT("EApartmentLifeUiScreen::BuildMode")) != INDEX_NONE);
	TestTrue(TEXT("Save/load screen exists"), StaticEnum<EApartmentLifeUiScreen>()->GetIndexByName(TEXT("EApartmentLifeUiScreen::SaveLoad")) != INDEX_NONE);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeUiOverlayGateTest,
	"ApartmentLife.UI.OverlayGate",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeUiOverlayGateTest::RunTest(const FString& Parameters)
{
	const bool bPrevious = FApartmentLifeUiOverlayGate::bSuppressDebugOverlays;
	FApartmentLifeUiOverlayGate::bSuppressDebugOverlays = true;
	TestTrue(TEXT("Overlay gate suppresses debug overlays"), FApartmentLifeUiOverlayGate::bSuppressDebugOverlays);
	FApartmentLifeUiOverlayGate::bSuppressDebugOverlays = bPrevious;
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeUiSubsystemDefaultsTest,
	"ApartmentLife.UI.SubsystemDefaults",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeUiSubsystemDefaultsTest::RunTest(const FString& Parameters)
{
	FApartmentLifeUiSettingsState Settings;
	TestTrue(TEXT("HUD visible by default"), Settings.bHudVisible);
	TestEqual(TEXT("Gameplay screen default"), static_cast<int32>(EApartmentLifeUiScreen::Gameplay), static_cast<int32>(EApartmentLifeUiScreen::Gameplay));
	return true;
}

#endif

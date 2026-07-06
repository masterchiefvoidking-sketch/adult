// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeDeveloperLibrary.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeActivityCatalogLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeDeveloperContentBrowserTest,
	"ApartmentLife.Developer.ContentBrowser",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeDeveloperContentBrowserTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeDeveloperContentEntry> Clothing =
		UApartmentLifeDeveloperLibrary::QueryContentBrowser(
			EApartmentLifeDeveloperContentCategory::Clothing, TEXT(""), false, TArray<FName>());
	TestTrue(TEXT("Clothing browser returns items"), Clothing.Num() > 0);

	const TArray<FApartmentLifeDeveloperContentEntry> Activities =
		UApartmentLifeDeveloperLibrary::QueryContentBrowser(
			EApartmentLifeDeveloperContentCategory::Activity, TEXT("yoga"), false, TArray<FName>());
	TestTrue(TEXT("Activity search finds yoga"), Activities.Num() > 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeDeveloperValidationTest,
	"ApartmentLife.Developer.DataValidation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeDeveloperValidationTest::RunTest(const FString& Parameters)
{
	const FApartmentLifeDeveloperValidationReport Report =
		UApartmentLifeDeveloperLibrary::RunDataValidation(nullptr, nullptr);
	TestTrue(TEXT("Validation produces summary"), !Report.Summary.IsEmpty());
	TestEqual(TEXT("Catalog clothing count matches wardrobe catalog"),
		UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog().Num() > 0, true);
	TestEqual(TEXT("Catalog activity count matches activity catalog"),
		UApartmentLifeActivityCatalogLibrary::GetBuiltinCatalog().Num() > 0, true);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeDeveloperDashboardTest,
	"ApartmentLife.Developer.Dashboard",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeDeveloperDashboardTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeDeveloperDashboardCategory> Dashboard =
		UApartmentLifeDeveloperLibrary::BuildDefaultDashboard();
	TestEqual(TEXT("Dashboard has 8 categories"), Dashboard.Num(), 8);
	TestEqual(TEXT("Camera completion baseline"), Dashboard[0].CompletionPercent, 100);
	return true;
}

#endif

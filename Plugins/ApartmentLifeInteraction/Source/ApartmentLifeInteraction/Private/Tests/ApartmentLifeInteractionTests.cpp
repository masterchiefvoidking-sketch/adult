// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeInteractionLibrary.h"
#include "ApartmentLifeApartmentTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeInteractionActivityMapTest,
	"ApartmentLife.Interaction.ActivityMapping",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeInteractionActivityMapTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Sleep maps to bed activity"),
		UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(EApartmentLifeFurnitureInteraction::Sleep),
		FName(TEXT("activity.sleep.bed")));

	TestEqual(TEXT("Shower maps to hygiene activity"),
		UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(EApartmentLifeFurnitureInteraction::Shower),
		FName(TEXT("activity.hygiene.shower")));

	TestEqual(TEXT("Yoga maps to fitness activity"),
		UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(EApartmentLifeFurnitureInteraction::Yoga),
		FName(TEXT("activity.fitness.yoga")));

	const TArray<EApartmentLifeFurnitureInteraction> Interactions = {
		EApartmentLifeFurnitureInteraction::Read,
		EApartmentLifeFurnitureInteraction::Sleep
	};
	TestEqual(TEXT("Primary interaction is first entry"),
		UApartmentLifeInteractionLibrary::GetPrimaryInteraction(Interactions),
		EApartmentLifeFurnitureInteraction::Read);

	return true;
}

#endif

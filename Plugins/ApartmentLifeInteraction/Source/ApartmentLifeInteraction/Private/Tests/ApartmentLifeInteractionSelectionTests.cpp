// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeInteractionLibrary.h"
#include "ApartmentLifeApartmentTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeInteractionSelectionDescriptorTest,
	"ApartmentLife.Interaction.SelectionDescriptors",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeInteractionSelectionDescriptorTest::RunTest(const FString& Parameters)
{
	const FApartmentLifeInteractionDescriptor YogaDesc =
		UApartmentLifeInteractionLibrary::BuildDescriptor(EApartmentLifeFurnitureInteraction::Yoga);
	TestEqual(TEXT("Descriptor stores interaction type"), YogaDesc.InteractionType, EApartmentLifeFurnitureInteraction::Yoga);
	TestFalse(TEXT("Yoga activity id set"), YogaDesc.ActivityId.IsNone());

	UApartmentLifeInteractionSelectionComponent* Selection = NewObject<UApartmentLifeInteractionSelectionComponent>();
	TestNotNull(TEXT("Selection component created"), Selection);

	return true;
}

#endif

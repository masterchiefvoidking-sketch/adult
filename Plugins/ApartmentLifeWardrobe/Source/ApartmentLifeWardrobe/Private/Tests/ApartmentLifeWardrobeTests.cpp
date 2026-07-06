// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeWardrobeBootstrapLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "Components/ActorComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobeBootstrapTest,
	"ApartmentLife.Wardrobe.VerticalSliceBootstrap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobeBootstrapTest::RunTest(const FString& Parameters)
{
	const TArray<FName> ClothingIds = UApartmentLifeWardrobeBootstrapLibrary::GetVerticalSliceClothingIds();
	TestTrue(TEXT("Vertical slice wardrobe has clothing items"), ClothingIds.Num() >= 12);

	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(Wardrobe);

	TestTrue(TEXT("Equipped slots after bootstrap"), Wardrobe->GetEquippedSlots().Num() > 0);

	return true;
}

#endif

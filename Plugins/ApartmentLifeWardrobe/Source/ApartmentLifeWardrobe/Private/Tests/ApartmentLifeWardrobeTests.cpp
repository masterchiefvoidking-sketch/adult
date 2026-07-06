// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeWardrobeBootstrapLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
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
	TestTrue(TEXT("Morning preset exists"), Wardrobe->ApplyOutfitPresetByType(EApartmentLifeOutfitPresetType::Morning));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobePurchaseTest,
	"ApartmentLife.Wardrobe.PurchaseClothing",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobePurchaseTest::RunTest(const FString& Parameters)
{
	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();
	Sim->Finance.Savings = 500.f;

	const FName ItemId(TEXT("clothing.shop.hoodie.gamer"));
	TestFalse(TEXT("Cannot purchase before owning"), Wardrobe->OwnsClothing(ItemId));
	TestTrue(TEXT("Purchase succeeds with savings"), Wardrobe->PurchaseClothing(Sim, ItemId, 1));
	TestTrue(TEXT("Item owned after purchase"), Wardrobe->OwnsClothing(ItemId));
	TestTrue(TEXT("Shopping history recorded"), Wardrobe->GetShoppingHistory().Num() == 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobeFilterTest,
	"ApartmentLife.Wardrobe.FilterAndSort",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobeFilterTest::RunTest(const FString& Parameters)
{
	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(Wardrobe);

	FApartmentLifeWardrobeFilterQuery Query;
	Query.CategoryTab = EApartmentLifeWardrobeCategoryTab::Athletic;
	const TArray<FName> AthleticItems = UApartmentLifeWardrobeLibrary::FilterAndSortCloset(Wardrobe, Query);
	TestTrue(TEXT("Athletic tab returns items"), AthleticItems.Num() > 0);

	Query.bAllCategories = true;
	Query.SearchText = TEXT("jeans");
	const TArray<FName> SearchResults = UApartmentLifeWardrobeLibrary::FilterAndSortCloset(Wardrobe, Query);
	TestTrue(TEXT("Search finds jeans"), SearchResults.Contains(FName(TEXT("clothing.casual.jeans.blue"))));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobeMoodEffectTest,
	"ApartmentLife.Wardrobe.OutfitMoodEffect",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobeMoodEffectTest::RunTest(const FString& Parameters)
{
	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();
	UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(Wardrobe);

	const float InitialConfidence = Sim->Mood.Confidence;
	const FApartmentLifeOutfitMoodEffect Effect = UApartmentLifeWardrobeLibrary::ComputeOutfitMoodEffect(Wardrobe, FName(TEXT("activity.yoga")));
	UApartmentLifeWardrobeLibrary::ApplyOutfitMoodEffect(Sim, Effect);

	TestTrue(TEXT("Outfit mood effect changes confidence or comfort"),
		Sim->Mood.Confidence != InitialConfidence || Sim->Mood.Comfort > 0.f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeWardrobeSaveRoundTripTest,
	"ApartmentLife.Wardrobe.SaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeWardrobeSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UApartmentLifeWardrobeComponent* Wardrobe = NewObject<UApartmentLifeWardrobeComponent>();
	UApartmentLifeNPCSimulationComponent* Sim = NewObject<UApartmentLifeNPCSimulationComponent>();
	Sim->Finance.Savings = 1000.f;

	UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(Wardrobe);
	Wardrobe->PurchaseClothing(Sim, FName(TEXT("clothing.accessory.glasses.round")), 2);
	Wardrobe->ToggleFavoriteItem(FName(TEXT("clothing.casual.tshirt.pink")));

	TMap<FString, FString> SaveData;
	Wardrobe->CaptureSaveData_Implementation(SaveData);

	UApartmentLifeWardrobeComponent* Restored = NewObject<UApartmentLifeWardrobeComponent>();
	Restored->RestoreSaveData_Implementation(SaveData);

	TestEqual(TEXT("Equipped slots restored"), Restored->GetEquippedSlots().Num(), Wardrobe->GetEquippedSlots().Num());
	TestEqual(TEXT("Closet inventory restored"), Restored->GetClosetInventory().Num(), Wardrobe->GetClosetInventory().Num());
	TestEqual(TEXT("Favorite items restored"), Restored->GetFavoriteItemIds().Num(), Wardrobe->GetFavoriteItemIds().Num());
	TestEqual(TEXT("Shopping history restored"), Restored->GetShoppingHistory().Num(), Wardrobe->GetShoppingHistory().Num());

	return true;
}

#endif

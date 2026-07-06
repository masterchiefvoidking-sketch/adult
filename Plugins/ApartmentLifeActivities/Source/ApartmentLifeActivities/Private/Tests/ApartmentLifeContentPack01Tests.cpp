// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeContentPack01.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeActivityContentPack01.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeShoppingContentPack01.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeYogaPoseCatalogLibrary.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "ApartmentLifeDialogueCatalogLibrary.h"
#include "ApartmentLifeSocialLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeContentPack01WardrobeTest,
	"ApartmentLife.ContentPack01.WardrobeExpansion",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeContentPack01WardrobeTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Pack01 clothing count"), ApartmentLifeWardrobeContentPack01::GetExpansionClothing().Num(), 43);
	const TArray<FApartmentLifeBuiltinClothingItem> Catalog = UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog();
	TestTrue(TEXT("Merged catalog exceeds 75 items"), Catalog.Num() >= 75);

	FApartmentLifeBuiltinClothingItem Item;
	TestTrue(TEXT("Expansion casual hoodie exists"),
		UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(FName(TEXT("clothing.casual.hoodie.teal")), Item));
	TestTrue(TEXT("Expansion accessory exists"),
		UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(FName(TEXT("clothing.accessory.bracelet.silver")), Item));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeContentPack01ActivitiesTest,
	"ApartmentLife.ContentPack01.ActivitiesAndRoutines",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeContentPack01ActivitiesTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Pack01 activity count"), ApartmentLifeActivityContentPack01::GetExpansionActivities().Num(), 11);
	TestEqual(TEXT("Pack01 routine count"), ApartmentLifeActivityContentPack01::GetExpansionRoutineChains().Num(), 17);

	const TArray<FApartmentLifeRoutineChainDefinition> Chains = UApartmentLifeActivityCatalogLibrary::GetBuiltinRoutineChains();
	TestTrue(TEXT("Total routines >= 19"), Chains.Num() >= 19);

	FApartmentLifeRoutineChainDefinition Chain;
	TestTrue(TEXT("Morning quick routine exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(FName(TEXT("routine.morning.quick")), Chain));
	TestTrue(TEXT("Evening TV routine exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(FName(TEXT("routine.evening.tv")), Chain));

	FApartmentLifeActivityDefinition Activity;
	TestTrue(TEXT("Wake activity exists"),
		UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName(TEXT("activity.sleep.wake")), Activity));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeContentPack01ShopTest,
	"ApartmentLife.ContentPack01.ShopExpansion",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeContentPack01ShopTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Pack01 shop count"), ApartmentLifeShoppingContentPack01::GetExpansionShopItems().Num(), 30);
	const TArray<FApartmentLifeBuiltinShopItem> Catalog = UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog();
	TestTrue(TEXT("Merged shop catalog >= 40"), Catalog.Num() >= 40);

	FApartmentLifeBuiltinShopItem Item;
	TestTrue(TEXT("Yoga mat premium exists"),
		UApartmentLifeShoppingCatalogLibrary::TryGetShopItem(FName(TEXT("shop.fitness.yogamat.premium")), Item));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeContentPack01DialogueYogaTest,
	"ApartmentLife.ContentPack01.DialogueAndYoga",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeContentPack01DialogueYogaTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Dialogue line pool exists"), UApartmentLifeDialogueCatalogLibrary::GetBuiltinDialogueLineCount() >= 50);
	TestEqual(TEXT("Yoga pose count"), UApartmentLifeYogaPoseCatalogLibrary::GetBuiltinPoses().Num(), 8);

	FApartmentLifeBuiltinYogaPose Pose;
	TestTrue(TEXT("Warrior pose exists"),
		UApartmentLifeYogaPoseCatalogLibrary::TryGetBuiltinPose(FName(TEXT("pose.warrior.two")), Pose));

	TestEqual(TEXT("Hairstyle count"), UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinHairstyleIds().Num(), 11);
	TestEqual(TEXT("Color palette count"), UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinColorPalettes().Num(), 5);

	FApartmentLifeDialogueContext Context;
	Context.TimeOfDay.Hour = 7;
	Context.SpeakerMood.Happiness = 80.f;
	const FApartmentLifeDialogueLine Line = UApartmentLifeDialogueCatalogLibrary::GenerateContextualPlayerLine(Context);
	TestFalse(TEXT("Morning line generated"), Line.SpeakerText.IsEmpty());
	return true;
}

#endif

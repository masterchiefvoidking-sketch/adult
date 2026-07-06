// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeShoppingContentPack01.h"

namespace
{
	FApartmentLifeBuiltinShopItem MakeShop(
		FName Id, const TCHAR* Name, EApartmentLifeShopCategory Cat, EApartmentLifeFurnitureTier Tier,
		float Price, EApartmentLifeUpgradeType Upgrade, int32 UpgradeTier,
		float Comfort = 0.f, float Mood = 0.f, float Productivity = 0.f, float Unlock = 0.f)
	{
		FApartmentLifeBuiltinShopItem Item;
		Item.ItemId = Id;
		Item.DisplayName = FText::FromString(Name);
		Item.Category = Cat;
		Item.Tier = Tier;
		Item.Price = Price;
		Item.UpgradeType = Upgrade;
		Item.UpgradeTier = UpgradeTier;
		Item.ComfortBonus = Comfort;
		Item.MoodBonus = Mood;
		Item.ProductivityBonus = Productivity;
		Item.SavingsUnlockThreshold = Unlock;
		return Item;
	}
}

TArray<FApartmentLifeBuiltinShopItem> ApartmentLifeShoppingContentPack01::GetExpansionShopItems()
{
	return {
		// Bedroom
		MakeShop(TEXT("shop.furniture.bed.modern"), TEXT("Modern Platform Bed"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 420.f, EApartmentLifeUpgradeType::Bed, 2, 14.f, 9.f, 0.f, 550.f),
		MakeShop(TEXT("shop.furniture.nightstand.oak"), TEXT("Oak Nightstand"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Budget, 95.f, EApartmentLifeUpgradeType::Decor, 2, 4.f, 4.f),
		MakeShop(TEXT("shop.decor.lamp.bedside"), TEXT("Bedside Lamp"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Budget, 48.f, EApartmentLifeUpgradeType::Lighting, 2, 3.f, 5.f),
		MakeShop(TEXT("shop.decor.rug.bedroom"), TEXT("Bedroom Rug"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Standard, 120.f, EApartmentLifeUpgradeType::Decor, 2, 6.f, 5.f, 0.f, 350.f),
		MakeShop(TEXT("shop.furniture.dresser.white"), TEXT("White Dresser"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 260.f, EApartmentLifeUpgradeType::Wardrobe, 2, 5.f, 4.f, 0.f, 450.f),
		MakeShop(TEXT("shop.decor.art.wall"), TEXT("Wall Art Print"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Budget, 55.f, EApartmentLifeUpgradeType::Decor, 1, 2.f, 6.f),

		// Bathroom
		MakeShop(TEXT("shop.bathroom.mirror.round"), TEXT("Round Vanity Mirror"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Standard, 85.f, EApartmentLifeUpgradeType::Decor, 2, 3.f, 4.f),
		MakeShop(TEXT("shop.bathroom.storage.cabinet"), TEXT("Bathroom Storage"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Budget, 75.f, EApartmentLifeUpgradeType::Decor, 2, 3.f, 3.f),
		MakeShop(TEXT("shop.bathroom.towels.plush"), TEXT("Plush Towel Set"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Budget, 42.f, EApartmentLifeUpgradeType::Decor, 1, 4.f, 3.f),
		MakeShop(TEXT("shop.bathroom.plant.fern"), TEXT("Bathroom Fern"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Budget, 32.f, EApartmentLifeUpgradeType::Decor, 1, 2.f, 5.f),
		MakeShop(TEXT("shop.bathroom.light.warm"), TEXT("Warm Vanity Light"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Standard, 68.f, EApartmentLifeUpgradeType::Lighting, 2, 2.f, 4.f, 0.f, 300.f),

		// Living room
		MakeShop(TEXT("shop.furniture.sofa.modern"), TEXT("Modern Grey Sofa"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 520.f, EApartmentLifeUpgradeType::Decor, 2, 12.f, 8.f, 0.f, 700.f),
		MakeShop(TEXT("shop.furniture.chair.accent"), TEXT("Accent Chair"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 180.f, EApartmentLifeUpgradeType::Chair, 2, 6.f, 5.f),
		MakeShop(TEXT("shop.furniture.tvstand.minimal"), TEXT("Minimal TV Stand"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 195.f, EApartmentLifeUpgradeType::Decor, 2, 4.f, 4.f, 0.f, 500.f),
		MakeShop(TEXT("shop.furniture.bookshelf.tall"), TEXT("Tall Bookshelf"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 210.f, EApartmentLifeUpgradeType::Decor, 2, 4.f, 5.f, 0.f, 450.f),
		MakeShop(TEXT("shop.furniture.table.coffee"), TEXT("Coffee Table"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 145.f, EApartmentLifeUpgradeType::Decor, 2, 4.f, 4.f),
		MakeShop(TEXT("shop.decor.plant.monstera"), TEXT("Monstera Plant"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Standard, 58.f, EApartmentLifeUpgradeType::Decor, 2, 3.f, 6.f),

		// Kitchen
		MakeShop(TEXT("shop.kitchen.appliance.toaster"), TEXT("Toaster Oven"), EApartmentLifeShopCategory::Kitchen, EApartmentLifeFurnitureTier::Budget, 65.f, EApartmentLifeUpgradeType::KitchenTools, 2, 2.f, 3.f),
		MakeShop(TEXT("shop.kitchen.table.dining"), TEXT("Dining Table"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 320.f, EApartmentLifeUpgradeType::Decor, 2, 6.f, 5.f, 0.f, 600.f),
		MakeShop(TEXT("shop.kitchen.chair.dining"), TEXT("Dining Chair Set"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 160.f, EApartmentLifeUpgradeType::Chair, 2, 4.f, 3.f),
		MakeShop(TEXT("shop.kitchen.storage.pantry"), TEXT("Pantry Storage"), EApartmentLifeShopCategory::Kitchen, EApartmentLifeFurnitureTier::Budget, 110.f, EApartmentLifeUpgradeType::KitchenTools, 2, 3.f, 2.f),

		// Computer area
		MakeShop(TEXT("shop.furniture.desk.standing"), TEXT("Standing Desk"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Premium, 480.f, EApartmentLifeUpgradeType::Desk, 3, 6.f, 4.f, 10.f, 1800.f),
		MakeShop(TEXT("shop.electronics.monitor.ultrawide"), TEXT("Ultrawide Monitor"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Premium, 380.f, EApartmentLifeUpgradeType::Monitor, 3, 0.f, 2.f, 8.f, 1200.f),
		MakeShop(TEXT("shop.electronics.keyboard.mechanical"), TEXT("Mechanical Keyboard"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Standard, 95.f, EApartmentLifeUpgradeType::Computer, 2, 0.f, 1.f, 3.f),
		MakeShop(TEXT("shop.furniture.shelf.office"), TEXT("Office Shelf"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Budget, 88.f, EApartmentLifeUpgradeType::Decor, 2, 2.f, 3.f, 2.f),
		MakeShop(TEXT("shop.decor.lamp.desk"), TEXT("Desk Lamp"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Budget, 52.f, EApartmentLifeUpgradeType::Lighting, 2, 2.f, 3.f, 2.f),

		// Yoga area
		MakeShop(TEXT("shop.fitness.yogamat.premium"), TEXT("Premium Cork Yoga Mat"), EApartmentLifeShopCategory::Fitness, EApartmentLifeFurnitureTier::Premium, 85.f, EApartmentLifeUpgradeType::YogaMat, 3, 5.f, 7.f, 0.f, 900.f),
		MakeShop(TEXT("shop.fitness.block.yoga"), TEXT("Yoga Blocks (Set)"), EApartmentLifeShopCategory::Fitness, EApartmentLifeFurnitureTier::Budget, 28.f, EApartmentLifeUpgradeType::Decor, 1, 2.f, 4.f),
		MakeShop(TEXT("shop.fitness.plant.calm"), TEXT("Calm Corner Plant"), EApartmentLifeShopCategory::Fitness, EApartmentLifeFurnitureTier::Budget, 38.f, EApartmentLifeUpgradeType::Decor, 1, 3.f, 6.f),
		MakeShop(TEXT("shop.fitness.light.calm"), TEXT("Calm Floor Light"), EApartmentLifeShopCategory::Fitness, EApartmentLifeFurnitureTier::Standard, 72.f, EApartmentLifeUpgradeType::Lighting, 2, 3.f, 7.f),
		MakeShop(TEXT("shop.fitness.speaker.bluetooth"), TEXT("Bluetooth Speaker"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Standard, 68.f, EApartmentLifeUpgradeType::Decor, 2, 2.f, 8.f)
	};
}

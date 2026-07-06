// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeShoppingCatalogLibrary.h"

namespace
{
	FApartmentLifeBuiltinShopItem MakeShop(
		FName Id,
		const TCHAR* Name,
		EApartmentLifeShopCategory Cat,
		EApartmentLifeFurnitureTier Tier,
		float Price,
		EApartmentLifeUpgradeType Upgrade,
		int32 UpgradeTier,
		float Comfort = 0.f,
		float Mood = 0.f,
		float Productivity = 0.f,
		float Unlock = 0.f)
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

TArray<FApartmentLifeBuiltinShopItem> UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog()
{
	return {
		MakeShop(TEXT("shop.furniture.chair.budget"), TEXT("Budget Office Chair"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Budget, 120.f, EApartmentLifeUpgradeType::Chair, 2, 5.f, 2.f, 3.f),
		MakeShop(TEXT("shop.furniture.desk.standard"), TEXT("Standard Desk"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 280.f, EApartmentLifeUpgradeType::Desk, 2, 4.f, 3.f, 5.f, 500.f),
		MakeShop(TEXT("shop.electronics.computer.budget"), TEXT("Budget Laptop"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Budget, 450.f, EApartmentLifeUpgradeType::Computer, 2, 0.f, 2.f, 8.f),
		MakeShop(TEXT("shop.electronics.monitor.standard"), TEXT("HD Monitor"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Standard, 220.f, EApartmentLifeUpgradeType::Monitor, 2, 0.f, 1.f, 5.f, 400.f),
		MakeShop(TEXT("shop.electronics.computer.premium"), TEXT("Premium Workstation"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Premium, 1200.f, EApartmentLifeUpgradeType::Computer, 3, 0.f, 4.f, 15.f, 2000.f),
		MakeShop(TEXT("shop.furniture.bed.standard"), TEXT("Comfortable Bed"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 350.f, EApartmentLifeUpgradeType::Bed, 2, 12.f, 8.f, 0.f, 600.f),
		MakeShop(TEXT("shop.furniture.bed.premium"), TEXT("Premium Bed"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Premium, 800.f, EApartmentLifeUpgradeType::Bed, 3, 18.f, 12.f, 0.f, 2500.f),
		MakeShop(TEXT("shop.decor.lamp.cozy"), TEXT("Cozy Floor Lamp"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Budget, 65.f, EApartmentLifeUpgradeType::Lighting, 2, 3.f, 5.f),
		MakeShop(TEXT("shop.decor.plant.small"), TEXT("Small Plant"), EApartmentLifeShopCategory::Decorations, EApartmentLifeFurnitureTier::Budget, 35.f, EApartmentLifeUpgradeType::Decor, 1, 2.f, 4.f),
		MakeShop(TEXT("shop.kitchen.tools.basic"), TEXT("Kitchen Tool Set"), EApartmentLifeShopCategory::Kitchen, EApartmentLifeFurnitureTier::Budget, 90.f, EApartmentLifeUpgradeType::KitchenTools, 2, 2.f, 3.f),
		MakeShop(TEXT("shop.bathroom.shower.head"), TEXT("Rain Shower Head"), EApartmentLifeShopCategory::Bathroom, EApartmentLifeFurnitureTier::Standard, 110.f, EApartmentLifeUpgradeType::Shower, 2, 5.f, 6.f, 0.f, 700.f),
		MakeShop(TEXT("shop.fitness.yogamat"), TEXT("Premium Yoga Mat"), EApartmentLifeShopCategory::Fitness, EApartmentLifeFurnitureTier::Standard, 55.f, EApartmentLifeUpgradeType::YogaMat, 2, 4.f, 5.f),
		MakeShop(TEXT("shop.furniture.tv.standard"), TEXT("Smart TV"), EApartmentLifeShopCategory::Electronics, EApartmentLifeFurnitureTier::Standard, 420.f, EApartmentLifeUpgradeType::TV, 2, 5.f, 10.f, 0.f, 800.f),
		MakeShop(TEXT("shop.furniture.wardrobe.expand"), TEXT("Expanded Closet"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Standard, 300.f, EApartmentLifeUpgradeType::Wardrobe, 2, 4.f, 5.f, 0.f, 1000.f),
		MakeShop(TEXT("shop.grocery.essentials"), TEXT("Grocery Essentials"), EApartmentLifeShopCategory::Groceries, EApartmentLifeFurnitureTier::Budget, 45.f, EApartmentLifeUpgradeType::Decor, 0, 0.f, 2.f),
		MakeShop(TEXT("shop.furniture.chair.premium"), TEXT("Ergonomic Chair"), EApartmentLifeShopCategory::Furniture, EApartmentLifeFurnitureTier::Premium, 550.f, EApartmentLifeUpgradeType::Chair, 3, 10.f, 5.f, 8.f, 3000.f)
	};
}

bool UApartmentLifeShoppingCatalogLibrary::TryGetShopItem(FName ItemId, FApartmentLifeBuiltinShopItem& OutItem)
{
	for (const FApartmentLifeBuiltinShopItem& Item : GetBuiltinCatalog())
	{
		if (Item.ItemId == ItemId)
		{
			OutItem = Item;
			return true;
		}
	}
	return false;
}

TArray<FApartmentLifeBuiltinShopItem> UApartmentLifeShoppingCatalogLibrary::GetItemsForCategory(EApartmentLifeShopCategory Category)
{
	TArray<FApartmentLifeBuiltinShopItem> Results;
	for (const FApartmentLifeBuiltinShopItem& Item : GetBuiltinCatalog())
	{
		if (Item.Category == Category)
		{
			Results.Add(Item);
		}
	}
	return Results;
}

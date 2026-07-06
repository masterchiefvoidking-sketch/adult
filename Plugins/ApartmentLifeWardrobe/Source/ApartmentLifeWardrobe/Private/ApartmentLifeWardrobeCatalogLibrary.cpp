// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeContentPack01.h"

namespace
{
	FApartmentLifeBuiltinClothingItem MakeItem(
		FName Id,
		const TCHAR* Name,
		EApartmentLifeClothingLayer Layer,
		EApartmentLifeWardrobeCategoryTab Tab,
		float Price,
		EApartmentLifeClothingIncomeTier Tier,
		std::initializer_list<EApartmentLifeClothingStyleTag> Tags,
		std::initializer_list<EApartmentLifeOutfitContext> Contexts,
		float Comfort = 0.7f,
		bool bYoga = false)
	{
		FApartmentLifeBuiltinClothingItem Item;
		Item.ItemId = Id;
		Item.DisplayName = FText::FromString(Name);
		Item.Layer = Layer;
		Item.CategoryTab = Tab;
		Item.Price = Price;
		Item.IncomeTier = Tier;
		Item.ComfortRating = Comfort;
		Item.StyleTags = Tags;
		Item.SuitableContexts = Contexts;
		Item.bYogaCompatible = bYoga;
		Item.ColorOptions = { FName(TEXT("default")) };
		Item.PatternOptions = { FName(TEXT("solid")) };
		return Item;
	}
}

TArray<FApartmentLifeBuiltinClothingItem> UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog()
{
	TArray<FApartmentLifeBuiltinClothingItem> Catalog = {
		MakeItem(TEXT("clothing.casual.tshirt.pink"), TEXT("Pink Casual Tee"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 35.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.jeans.blue"), TEXT("Blue Jeans"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Casual, 55.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.sneakers.white"), TEXT("White Sneakers"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Casual, 45.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Athletic }, 0.8f, true),
		MakeItem(TEXT("clothing.casual.tshirt.white"), TEXT("White Casual Tee"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 30.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Minimalist, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.skirt.denim"), TEXT("Denim Skirt"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Casual, 48.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.sandals.tan"), TEXT("Tan Sandals"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Casual, 40.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.blouse.floral"), TEXT("Floral Blouse"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 52.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Romantic, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.casual.shorts.khaki"), TEXT("Khaki Shorts"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Casual, 42.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.work.blouse.cream"), TEXT("Cream Work Blouse"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 68.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.work.trousers.charcoal"), TEXT("Charcoal Trousers"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 75.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.work.blazer.navy"), TEXT("Navy Blazer"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 120.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Work, EApartmentLifeOutfitContext::Formal }),
		MakeItem(TEXT("clothing.work.pumps.black"), TEXT("Black Pumps"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 85.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Work, EApartmentLifeOutfitContext::Formal }),
		MakeItem(TEXT("clothing.lounge.hoodie.gray"), TEXT("Gray Hoodie"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Lounge, 50.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Lounge }),
		MakeItem(TEXT("clothing.lounge.sweats.soft"), TEXT("Soft Sweatpants"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Lounge, 45.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Lounge }, 0.9f),
		MakeItem(TEXT("clothing.lounge.slippers.pink"), TEXT("Pink Slippers"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Lounge, 25.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Lounge, EApartmentLifeOutfitContext::Sleep }, 0.95f),
		MakeItem(TEXT("clothing.lounge.cardigan.cozy"), TEXT("Cozy Cardigan"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Lounge, 58.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Lounge }),
		MakeItem(TEXT("clothing.lounge.leggings.black"), TEXT("Black Leggings"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Lounge, 38.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Lounge, EApartmentLifeOutfitContext::Athletic }, 0.85f, true),
		MakeItem(TEXT("clothing.sleep.pajama.set"), TEXT("Pajama Set"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Sleepwear, 42.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Sleep }, 0.95f),
		MakeItem(TEXT("clothing.sleep.robe.silk"), TEXT("Silk Robe"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Sleepwear, 95.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Luxury, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Sleep, EApartmentLifeOutfitContext::Lounge }, 0.92f),
		MakeItem(TEXT("clothing.sleep.slippers.fluffy"), TEXT("Fluffy Slippers"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Sleepwear, 28.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Sleep }, 0.95f),
		MakeItem(TEXT("clothing.sleep.nightgown.lace"), TEXT("Lace Nightgown"), EApartmentLifeClothingLayer::Dress, EApartmentLifeWardrobeCategoryTab::Sleepwear, 72.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Romantic, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Sleep }, 0.9f),
		MakeItem(TEXT("clothing.athletic.tank.mint"), TEXT("Mint Athletic Tank"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Athletic, 32.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.85f, true),
		MakeItem(TEXT("clothing.athletic.leggings.teal"), TEXT("Teal Yoga Leggings"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Athletic, 48.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.9f, true),
		MakeItem(TEXT("clothing.athletic.sportsbra.black"), TEXT("Black Sports Bra"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Athletic, 36.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.88f, true),
		MakeItem(TEXT("clothing.athletic.shorts.running"), TEXT("Running Shorts"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Athletic, 34.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.82f, true),
		MakeItem(TEXT("clothing.formal.dress.evening"), TEXT("Evening Dress"), EApartmentLifeClothingLayer::Dress, EApartmentLifeWardrobeCategoryTab::Formal, 180.f, EApartmentLifeClothingIncomeTier::Luxury, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Luxury }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.formal.heels.silver"), TEXT("Silver Heels"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Formal, 110.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Luxury }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.accessory.glasses.round"), TEXT("Round Glasses"), EApartmentLifeClothingLayer::Glasses, EApartmentLifeWardrobeCategoryTab::Accessories, 45.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Artistic }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.accessory.earrings.pearl"), TEXT("Pearl Earrings"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 65.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Romantic }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.accessory.necklace.gold"), TEXT("Gold Necklace"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 90.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Luxury, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal }),
		MakeItem(TEXT("clothing.accessory.hairclip.sakura"), TEXT("Sakura Hair Clip"), EApartmentLifeClothingLayer::Hat, EApartmentLifeWardrobeCategoryTab::Accessories, 22.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Seasonal }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.accessory.watch.minimal"), TEXT("Minimal Watch"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Accessories, 120.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Minimalist, EApartmentLifeClothingStyleTag::Professional }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.accessory.bag.tote"), TEXT("Canvas Tote Bag"), EApartmentLifeClothingLayer::Bag, EApartmentLifeWardrobeCategoryTab::Accessories, 38.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual, EApartmentLifeClothingStyleTag::Artistic }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.seasonal.scarf.wool"), TEXT("Wool Scarf"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Seasonal, 55.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Seasonal }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.seasonal.coat.winter"), TEXT("Winter Coat"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Seasonal, 160.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Seasonal }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.shop.hoodie.gamer"), TEXT("Gamer Hoodie"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 62.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Gamer, EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Lounge, EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.shop.dress.designer"), TEXT("Designer Cocktail Dress"), EApartmentLifeClothingLayer::Dress, EApartmentLifeWardrobeCategoryTab::Formal, 320.f, EApartmentLifeClothingIncomeTier::Luxury, { EApartmentLifeClothingStyleTag::Luxury, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date })
	};
	Catalog.Append(ApartmentLifeWardrobeContentPack01::GetExpansionClothing());
	return Catalog;
}

bool UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(FName ItemId, FApartmentLifeBuiltinClothingItem& OutItem)
{
	for (const FApartmentLifeBuiltinClothingItem& Item : GetBuiltinCatalog())
	{
		if (Item.ItemId == ItemId)
		{
			OutItem = Item;
			return true;
		}
	}
	return false;
}

EApartmentLifeClothingLayer UApartmentLifeWardrobeCatalogLibrary::SlotToLayer(EApartmentLifeClothingSlot Slot)
{
	switch (Slot)
	{
	case EApartmentLifeClothingSlot::HairAccessory: return EApartmentLifeClothingLayer::Hat;
	case EApartmentLifeClothingSlot::Glasses: return EApartmentLifeClothingLayer::Glasses;
	case EApartmentLifeClothingSlot::Earrings:
	case EApartmentLifeClothingSlot::Necklace:
	case EApartmentLifeClothingSlot::BraceletWatch: return EApartmentLifeClothingLayer::Jewelry;
	case EApartmentLifeClothingSlot::Top: return EApartmentLifeClothingLayer::Tops;
	case EApartmentLifeClothingSlot::Jacket: return EApartmentLifeClothingLayer::Jacket;
	case EApartmentLifeClothingSlot::Bottom: return EApartmentLifeClothingLayer::Bottoms;
	case EApartmentLifeClothingSlot::Dress: return EApartmentLifeClothingLayer::Dress;
	case EApartmentLifeClothingSlot::Socks: return EApartmentLifeClothingLayer::Socks;
	case EApartmentLifeClothingSlot::Shoes: return EApartmentLifeClothingLayer::Shoes;
	case EApartmentLifeClothingSlot::Bag: return EApartmentLifeClothingLayer::Bag;
	default: return EApartmentLifeClothingLayer::Tops;
	}
}

EApartmentLifeClothingIncomeTier UApartmentLifeWardrobeCatalogLibrary::GetIncomeTierForSavings(float Savings)
{
	if (Savings >= 5000.f) return EApartmentLifeClothingIncomeTier::Luxury;
	if (Savings >= 2500.f) return EApartmentLifeClothingIncomeTier::Premium;
	if (Savings >= 800.f) return EApartmentLifeClothingIncomeTier::MidRange;
	return EApartmentLifeClothingIncomeTier::Budget;
}

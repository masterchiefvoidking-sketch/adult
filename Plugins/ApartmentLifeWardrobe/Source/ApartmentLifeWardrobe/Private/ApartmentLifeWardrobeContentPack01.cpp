// Copyright Adult Anime Apartment Life. All Rights Reserved.

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

TArray<FApartmentLifeBuiltinClothingItem> ApartmentLifeWardrobeContentPack01::GetExpansionClothing()
{
	return {
		// Casual (10)
		MakeItem(TEXT("clothing.casual.hoodie.teal"), TEXT("Teal Hoodie"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 58.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Lounge }),
		MakeItem(TEXT("clothing.casual.crop.sunset"), TEXT("Sunset Crop Top"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 38.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.joggers.gray"), TEXT("Gray Joggers"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Casual, 46.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual, EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Athletic }, 0.82f, true),
		MakeItem(TEXT("clothing.casual.loafers.brown"), TEXT("Brown Loafers"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Casual, 72.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.casual.cardigan.oat"), TEXT("Oat Cardigan"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Casual, 64.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Minimalist }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.overalls.denim"), TEXT("Denim Overalls"), EApartmentLifeClothingLayer::Dress, EApartmentLifeWardrobeCategoryTab::Casual, 78.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Artistic }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.tee.striped"), TEXT("Striped Tee"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Casual, 34.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.midi.skirt"), TEXT("Midi Skirt"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Casual, 52.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.casual.canvas.shoes"), TEXT("Canvas Shoes"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Casual, 42.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.casual.bucket.hat"), TEXT("Bucket Hat"), EApartmentLifeClothingLayer::Hat, EApartmentLifeWardrobeCategoryTab::Casual, 28.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday }),

		// Work from home (5)
		MakeItem(TEXT("clothing.work.hoodie.zip"), TEXT("Zip Work Hoodie"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 62.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Work, EApartmentLifeOutfitContext::Lounge }),
		MakeItem(TEXT("clothing.work.joggers.black"), TEXT("Black Work Joggers"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 54.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.work.blouse.satin"), TEXT("Satin Work Blouse"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 82.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.work.slacks.navy"), TEXT("Navy Slacks"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 88.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Professional }, { EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.work.loafers.leather"), TEXT("Leather Loafers"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::WorkFromHome, 95.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Work }),

		// Lounge (5)
		MakeItem(TEXT("clothing.lounge.oversized.tee"), TEXT("Oversized Tee"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Lounge, 36.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Lounge }, 0.92f),
		MakeItem(TEXT("clothing.lounge.shorts.soft"), TEXT("Soft Lounge Shorts"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Lounge, 32.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Lounge }, 0.94f),
		MakeItem(TEXT("clothing.lounge.socks.fuzzy"), TEXT("Fuzzy Socks"), EApartmentLifeClothingLayer::Socks, EApartmentLifeWardrobeCategoryTab::Lounge, 18.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Lounge, EApartmentLifeOutfitContext::Sleep }, 0.96f),
		MakeItem(TEXT("clothing.lounge.wrap.soft"), TEXT("Soft Wrap"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Lounge, 48.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Romantic }, { EApartmentLifeOutfitContext::Lounge }, 0.93f),
		MakeItem(TEXT("clothing.lounge.tank.ribbed"), TEXT("Ribbed Tank"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Lounge, 28.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Minimalist }, { EApartmentLifeOutfitContext::Lounge }, 0.88f),

		// Sleepwear (5)
		MakeItem(TEXT("clothing.sleep.camisole.silk"), TEXT("Silk Camisole"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Sleepwear, 68.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Romantic }, { EApartmentLifeOutfitContext::Sleep }, 0.94f),
		MakeItem(TEXT("clothing.sleep.shorts.silk"), TEXT("Silk Sleep Shorts"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Sleepwear, 58.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Sleep }, 0.95f),
		MakeItem(TEXT("clothing.sleep.sock.warm"), TEXT("Warm Sleep Socks"), EApartmentLifeClothingLayer::Socks, EApartmentLifeWardrobeCategoryTab::Sleepwear, 16.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Sleep }, 0.97f),
		MakeItem(TEXT("clothing.sleep.mask.soft"), TEXT("Soft Sleep Mask"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Sleepwear, 22.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy }, { EApartmentLifeOutfitContext::Sleep }, 0.9f),
		MakeItem(TEXT("clothing.sleep.gown.cotton"), TEXT("Cotton Nightgown"), EApartmentLifeClothingLayer::Dress, EApartmentLifeWardrobeCategoryTab::Sleepwear, 54.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cozy, EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Sleep }, 0.93f),

		// Athletic / yoga (5)
		MakeItem(TEXT("clothing.athletic.jacket.zip"), TEXT("Zip Athletic Jacket"), EApartmentLifeClothingLayer::Jacket, EApartmentLifeWardrobeCategoryTab::Athletic, 72.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.86f, true),
		MakeItem(TEXT("clothing.athletic.pants.jogger"), TEXT("Jogger Pants"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Athletic, 52.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.88f, true),
		MakeItem(TEXT("clothing.athletic.top.crop"), TEXT("Crop Athletic Top"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Athletic, 40.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.84f, true),
		MakeItem(TEXT("clothing.athletic.shoes.training"), TEXT("Training Shoes"), EApartmentLifeClothingLayer::Shoes, EApartmentLifeWardrobeCategoryTab::Athletic, 88.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.85f, true),
		MakeItem(TEXT("clothing.athletic.headband"), TEXT("Athletic Headband"), EApartmentLifeClothingLayer::Hat, EApartmentLifeWardrobeCategoryTab::Athletic, 16.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Sporty }, { EApartmentLifeOutfitContext::Athletic }, 0.8f, true),

		// Formal (3)
		MakeItem(TEXT("clothing.formal.blouse.silk"), TEXT("Silk Formal Blouse"), EApartmentLifeClothingLayer::Tops, EApartmentLifeWardrobeCategoryTab::Formal, 125.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Luxury }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.formal.skirt.pencil"), TEXT("Pencil Skirt"), EApartmentLifeClothingLayer::Bottoms, EApartmentLifeWardrobeCategoryTab::Formal, 98.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.formal.clutch.evening"), TEXT("Evening Clutch"), EApartmentLifeClothingLayer::Bag, EApartmentLifeWardrobeCategoryTab::Formal, 140.f, EApartmentLifeClothingIncomeTier::Luxury, { EApartmentLifeClothingStyleTag::Luxury, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),

		// Accessories (10)
		MakeItem(TEXT("clothing.accessory.bracelet.silver"), TEXT("Silver Bracelet"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 55.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Minimalist }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Formal }),
		MakeItem(TEXT("clothing.accessory.ring.delicate"), TEXT("Delicate Ring"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 75.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Romantic, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.accessory.belt.leather"), TEXT("Leather Belt"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Accessories, 48.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Professional, EApartmentLifeClothingStyleTag::Casual }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Work }),
		MakeItem(TEXT("clothing.accessory.scarf.silk"), TEXT("Silk Scarf"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Accessories, 62.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Elegant, EApartmentLifeClothingStyleTag::Seasonal }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Formal }),
		MakeItem(TEXT("clothing.accessory.sunglasses.cat"), TEXT("Cat-Eye Sunglasses"), EApartmentLifeClothingLayer::Glasses, EApartmentLifeWardrobeCategoryTab::Accessories, 58.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.accessory.beret.black"), TEXT("Black Beret"), EApartmentLifeClothingLayer::Hat, EApartmentLifeWardrobeCategoryTab::Accessories, 42.f, EApartmentLifeClothingIncomeTier::MidRange, { EApartmentLifeClothingStyleTag::Artistic, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.accessory.choker.pearl"), TEXT("Pearl Choker"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 68.f, EApartmentLifeClothingIncomeTier::Premium, { EApartmentLifeClothingStyleTag::Romantic, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Formal, EApartmentLifeOutfitContext::Date }),
		MakeItem(TEXT("clothing.accessory.bracelet.charm"), TEXT("Charm Bracelet"), EApartmentLifeClothingLayer::Jewelry, EApartmentLifeWardrobeCategoryTab::Accessories, 38.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.accessory.brooch.floral"), TEXT("Floral Brooch"), EApartmentLifeClothingLayer::Accessory, EApartmentLifeWardrobeCategoryTab::Accessories, 35.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Seasonal }, { EApartmentLifeOutfitContext::Everyday }),
		MakeItem(TEXT("clothing.accessory.headband.satin"), TEXT("Satin Headband"), EApartmentLifeClothingLayer::Hat, EApartmentLifeWardrobeCategoryTab::Accessories, 24.f, EApartmentLifeClothingIncomeTier::Budget, { EApartmentLifeClothingStyleTag::Cute, EApartmentLifeClothingStyleTag::Elegant }, { EApartmentLifeOutfitContext::Everyday, EApartmentLifeOutfitContext::Formal })
	};
}

TArray<FName> ApartmentLifeWardrobeContentPack01::GetExpansionClothingIds()
{
	TArray<FName> Ids;
	for (const FApartmentLifeBuiltinClothingItem& Item : GetExpansionClothing())
	{
		Ids.Add(Item.ItemId);
	}
	return Ids;
}

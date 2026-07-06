// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeBootstrapLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeTypes.h"

namespace
{
	void EquipOutfit(UApartmentLifeWardrobeComponent* Wardrobe, const TArray<TPair<EApartmentLifeClothingLayer, FName>>& Slots)
	{
		if (!Wardrobe)
		{
			return;
		}

		for (const TPair<EApartmentLifeClothingLayer, FName>& Slot : Slots)
		{
			Wardrobe->EquipClothing(Slot.Value, Slot.Key);
		}
	}

	void SaveFavorite(
		UApartmentLifeWardrobeComponent* Wardrobe,
		FName OutfitNameId,
		EApartmentLifeOutfitContext Context,
		const TArray<TPair<EApartmentLifeClothingLayer, FName>>& Slots)
	{
		if (!Wardrobe)
		{
			return;
		}

		EquipOutfit(Wardrobe, Slots);
		Wardrobe->SaveCurrentAsFavorite(OutfitNameId, Context);
	}
}

TArray<FName> UApartmentLifeWardrobeBootstrapLibrary::GetVerticalSliceClothingIds()
{
	return {
		FName(TEXT("clothing.casual.tshirt.pink")),
		FName(TEXT("clothing.casual.jeans.blue")),
		FName(TEXT("clothing.casual.sneakers.white")),
		FName(TEXT("clothing.casual.tshirt.white")),
		FName(TEXT("clothing.casual.skirt.denim")),
		FName(TEXT("clothing.casual.sandals.tan")),
		FName(TEXT("clothing.casual.blouse.floral")),
		FName(TEXT("clothing.casual.shorts.khaki")),
		FName(TEXT("clothing.work.blouse.cream")),
		FName(TEXT("clothing.work.trousers.charcoal")),
		FName(TEXT("clothing.work.blazer.navy")),
		FName(TEXT("clothing.work.pumps.black")),
		FName(TEXT("clothing.lounge.hoodie.gray")),
		FName(TEXT("clothing.lounge.sweats.soft")),
		FName(TEXT("clothing.lounge.slippers.pink")),
		FName(TEXT("clothing.lounge.cardigan.cozy")),
		FName(TEXT("clothing.lounge.leggings.black")),
		FName(TEXT("clothing.sleep.pajama.set")),
		FName(TEXT("clothing.sleep.robe.silk")),
		FName(TEXT("clothing.sleep.slippers.fluffy")),
		FName(TEXT("clothing.sleep.nightgown.lace")),
		FName(TEXT("clothing.athletic.tank.mint")),
		FName(TEXT("clothing.athletic.leggings.teal")),
		FName(TEXT("clothing.athletic.sportsbra.black")),
		FName(TEXT("clothing.athletic.shorts.running")),
		FName(TEXT("clothing.formal.dress.evening")),
		FName(TEXT("clothing.formal.heels.silver"))
	};
}

void UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(UApartmentLifeWardrobeComponent* Wardrobe)
{
	if (!Wardrobe)
	{
		return;
	}

	Wardrobe->SetClosetInventory(GetVerticalSliceClothingIds());

	const auto T = EApartmentLifeClothingLayer::Tops;
	const auto B = EApartmentLifeClothingLayer::Bottoms;
	const auto S = EApartmentLifeClothingLayer::Shoes;
	const auto D = EApartmentLifeClothingLayer::Dress;
	const auto J = EApartmentLifeClothingLayer::Jacket;

	// 3 casual outfits
	SaveFavorite(Wardrobe, FName(TEXT("outfit.casual.day_a")), EApartmentLifeOutfitContext::Everyday, {
		{ T, FName(TEXT("clothing.casual.tshirt.pink")) },
		{ B, FName(TEXT("clothing.casual.jeans.blue")) },
		{ S, FName(TEXT("clothing.casual.sneakers.white")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.casual.day_b")), EApartmentLifeOutfitContext::Everyday, {
		{ T, FName(TEXT("clothing.casual.tshirt.white")) },
		{ B, FName(TEXT("clothing.casual.skirt.denim")) },
		{ S, FName(TEXT("clothing.casual.sandals.tan")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.casual.day_c")), EApartmentLifeOutfitContext::Everyday, {
		{ T, FName(TEXT("clothing.casual.blouse.floral")) },
		{ B, FName(TEXT("clothing.casual.shorts.khaki")) },
		{ S, FName(TEXT("clothing.casual.sandals.tan")) }
	});

	// 2 work outfits
	SaveFavorite(Wardrobe, FName(TEXT("outfit.work.office_a")), EApartmentLifeOutfitContext::Work, {
		{ T, FName(TEXT("clothing.work.blouse.cream")) },
		{ B, FName(TEXT("clothing.work.trousers.charcoal")) },
		{ S, FName(TEXT("clothing.work.pumps.black")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.work.office_b")), EApartmentLifeOutfitContext::Work, {
		{ J, FName(TEXT("clothing.work.blazer.navy")) },
		{ T, FName(TEXT("clothing.work.blouse.cream")) },
		{ B, FName(TEXT("clothing.work.trousers.charcoal")) },
		{ S, FName(TEXT("clothing.work.pumps.black")) }
	});

	// 2 lounge outfits
	SaveFavorite(Wardrobe, FName(TEXT("outfit.lounge.cozy_a")), EApartmentLifeOutfitContext::Lounge, {
		{ T, FName(TEXT("clothing.lounge.hoodie.gray")) },
		{ B, FName(TEXT("clothing.lounge.sweats.soft")) },
		{ S, FName(TEXT("clothing.lounge.slippers.pink")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.lounge.cozy_b")), EApartmentLifeOutfitContext::Lounge, {
		{ T, FName(TEXT("clothing.lounge.cardigan.cozy")) },
		{ B, FName(TEXT("clothing.lounge.leggings.black")) },
		{ S, FName(TEXT("clothing.lounge.slippers.pink")) }
	});

	// 2 sleepwear outfits
	SaveFavorite(Wardrobe, FName(TEXT("outfit.sleep.night_a")), EApartmentLifeOutfitContext::Sleep, {
		{ T, FName(TEXT("clothing.sleep.pajama.set")) },
		{ S, FName(TEXT("clothing.sleep.slippers.fluffy")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.sleep.night_b")), EApartmentLifeOutfitContext::Sleep, {
		{ D, FName(TEXT("clothing.sleep.nightgown.lace")) },
		{ J, FName(TEXT("clothing.sleep.robe.silk")) },
		{ S, FName(TEXT("clothing.sleep.slippers.fluffy")) }
	});

	// 2 athletic outfits
	SaveFavorite(Wardrobe, FName(TEXT("outfit.athletic.yoga_a")), EApartmentLifeOutfitContext::Athletic, {
		{ T, FName(TEXT("clothing.athletic.tank.mint")) },
		{ B, FName(TEXT("clothing.athletic.leggings.teal")) },
		{ S, FName(TEXT("clothing.casual.sneakers.white")) }
	});
	SaveFavorite(Wardrobe, FName(TEXT("outfit.athletic.yoga_b")), EApartmentLifeOutfitContext::Athletic, {
		{ T, FName(TEXT("clothing.athletic.sportsbra.black")) },
		{ B, FName(TEXT("clothing.athletic.shorts.running")) },
		{ S, FName(TEXT("clothing.casual.sneakers.white")) }
	});

	// 1 formal outfit
	SaveFavorite(Wardrobe, FName(TEXT("outfit.formal.evening")), EApartmentLifeOutfitContext::Formal, {
		{ D, FName(TEXT("clothing.formal.dress.evening")) },
		{ S, FName(TEXT("clothing.formal.heels.silver")) }
	});

	Wardrobe->ApplyFavoriteOutfit(FName(TEXT("outfit.casual.day_a")));
}

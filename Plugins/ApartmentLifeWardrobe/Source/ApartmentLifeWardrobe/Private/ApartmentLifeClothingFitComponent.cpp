// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeClothingFitComponent.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"

void UApartmentLifeClothingFitComponent::RefreshClothingFit(const FApartmentLifeBodyFitProfile& BodyFit)
{
	ComputedScaleAdjustment = BodyFit.ScaleAdjustment;
	ComputedSleeveOffset = BodyFit.SleeveLengthOffset + (BodyFit.ScaleAdjustment.Y - 1.f) * 2.f;
	ComputedPantOffset = BodyFit.PantLengthOffset + (BodyFit.ScaleAdjustment.Z - 1.f) * 4.f;

	const float ShoulderNorm = (BodyFit.CollisionRadius - 30.f) / 8.f;
	ComputedScaleAdjustment.X = FMath::Clamp(ComputedScaleAdjustment.X + ShoulderNorm * 0.02f, 0.85f, 1.15f);
}

bool UApartmentLifeClothingFitComponent::DoesClothingFit(const UApartmentLifeClothingItemData* Item, const FApartmentLifeBodyFitProfile& BodyFit) const
{
	if (!Item)
	{
		return false;
	}

	const float ShoulderNorm = (BodyFit.CollisionRadius - 30.f) / 8.f;
	return ShoulderNorm >= Item->FitData.MinShoulderWidth && ShoulderNorm <= Item->FitData.MaxShoulderWidth;
}

bool UApartmentLifeClothingFitComponent::DoesBuiltinClothingFit(FName ItemId, const FApartmentLifeBodyFitProfile& BodyFit) const
{
	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return true;
	}

	const float ShoulderNorm = (BodyFit.CollisionRadius - 30.f) / 8.f;
	const float MinShoulder = Item.Layer == EApartmentLifeClothingLayer::Jacket ? 0.1f : 0.f;
	const float MaxShoulder = Item.Layer == EApartmentLifeClothingLayer::Dress ? 1.2f : 1.4f;
	return ShoulderNorm >= MinShoulder && ShoulderNorm <= MaxShoulder;
}

void UApartmentLifeClothingFitComponent::ValidateEquippedFit(UApartmentLifeWardrobeComponent* Wardrobe, const FApartmentLifeBodyFitProfile& BodyFit)
{
	if (!Wardrobe)
	{
		return;
	}

	for (const FApartmentLifeEquippedClothingSlot& Slot : Wardrobe->GetEquippedSlots())
	{
		if (!DoesBuiltinClothingFit(Slot.ClothingItemId, BodyFit))
		{
			Wardrobe->UnequipLayer(Slot.Layer);
		}
	}
}

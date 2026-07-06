// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeClothingFitComponent.h"
#include "ApartmentLifeClothingItemData.h"

void UApartmentLifeClothingFitComponent::RefreshClothingFit(const FApartmentLifeBodyFitProfile& BodyFit)
{
	// Mesh bone offsets and cloth sim tweaks are applied in Blueprint/AnimBP using BodyFit offsets.
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

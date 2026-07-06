// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeUnlockLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

TArray<FName> UApartmentLifeUnlockLibrary::EvaluateUnlocks(
	UApartmentLifeNPCSimulationComponent* Simulation,
	UApartmentLifeProgressionComponent* Progression)
{
	TArray<FName> NewUnlocks;
	if (!Simulation || !Progression)
	{
		return NewUnlocks;
	}

	const float Savings = Simulation->Finance.Savings;

	for (const FApartmentLifeWorkTypeDefinition& Work : UApartmentLifeWorkLibrary::GetBuiltinWorkTypes())
	{
		if (Work.SavingsUnlockThreshold > 0.f && Savings >= Work.SavingsUnlockThreshold)
		{
			if (Progression->UnlockWorkType(Work.WorkTypeId))
			{
				NewUnlocks.Add(Work.WorkTypeId);
			}
		}
	}

	for (const FApartmentLifeBuiltinShopItem& Item : UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog())
	{
		if (Item.SavingsUnlockThreshold > 0.f && Savings >= Item.SavingsUnlockThreshold)
		{
			if (Progression->UnlockShopItem(Item.ItemId))
			{
				NewUnlocks.Add(Item.ItemId);
			}
		}
	}

	if (Savings >= 2000.f && Progression->UnlockShopTier(EApartmentLifeFurnitureTier::Standard))
	{
		NewUnlocks.Add(FName(TEXT("unlock.tier.standard")));
	}
	if (Savings >= 5000.f && Progression->UnlockShopTier(EApartmentLifeFurnitureTier::Premium))
	{
		NewUnlocks.Add(FName(TEXT("unlock.tier.premium")));
	}
	if (Savings >= 10000.f && Progression->UnlockShopTier(EApartmentLifeFurnitureTier::Luxury))
	{
		NewUnlocks.Add(FName(TEXT("unlock.tier.luxury")));
	}

	return NewUnlocks;
}

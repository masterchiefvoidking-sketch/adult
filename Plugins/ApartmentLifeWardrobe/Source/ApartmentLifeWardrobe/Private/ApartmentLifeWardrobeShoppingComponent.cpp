// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeShoppingComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

void UApartmentLifeWardrobeShoppingComponent::InitializeContext(
	UApartmentLifeWardrobeComponent* InWardrobe,
	UApartmentLifeNPCSimulationComponent* InSimulation)
{
	Wardrobe = InWardrobe;
	Simulation = InSimulation;
}

TArray<FName> UApartmentLifeWardrobeShoppingComponent::GetBrowseResults(const FApartmentLifeWardrobeFilterQuery& Query) const
{
	if (!Wardrobe.IsValid())
	{
		return {};
	}

	TArray<FName> Results = UApartmentLifeWardrobeLibrary::FilterAndSortCloset(Wardrobe.Get(), Query);
	const EApartmentLifeClothingIncomeTier Tier = GetCurrentIncomeTier();

	Results.RemoveAll([Tier](const FName& ItemId)
	{
		FApartmentLifeBuiltinClothingItem Item;
		if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
		{
			return true;
		}
		return static_cast<uint8>(Item.IncomeTier) > static_cast<uint8>(Tier);
	});

	return Results;
}

TArray<FName> UApartmentLifeWardrobeShoppingComponent::GetAffordableRecommendations() const
{
	FApartmentLifeWardrobeFilterQuery Query;
	Query.bAllCategories = true;
	Query.SortMode = EApartmentLifeClothingSortMode::Style;
	Query.bOwnedOnly = false;

	TArray<FName> Results = GetBrowseResults(Query);
	Results.RemoveAll([this](const FName& ItemId)
	{
		return Wardrobe.IsValid() && Wardrobe->OwnsClothing(ItemId);
	});

	Results.SetNum(FMath::Min(Results.Num(), 6));
	return Results;
}

bool UApartmentLifeWardrobeShoppingComponent::TryPurchase(FName ItemId, int32 DayPurchased)
{
	if (!Wardrobe.IsValid() || !Simulation.IsValid())
	{
		return false;
	}

	return Wardrobe->PurchaseClothing(Simulation.Get(), ItemId, DayPurchased);
}

bool UApartmentLifeWardrobeShoppingComponent::CanAfford(FName ItemId) const
{
	return UApartmentLifeWardrobeLibrary::CanAffordItem(Simulation.Get(), ItemId);
}

void UApartmentLifeWardrobeShoppingComponent::PreviewItem(FName ItemId)
{
	if (!Wardrobe.IsValid())
	{
		return;
	}

	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return;
	}

	TArray<FApartmentLifeEquippedClothingSlot> PreviewSlots = Wardrobe->GetEquippedSlots();
	PreviewSlots.RemoveAll([Item](const FApartmentLifeEquippedClothingSlot& Slot) { return Slot.Layer == Item.Layer; });

	FApartmentLifeEquippedClothingSlot NewSlot;
	NewSlot.ClothingItemId = ItemId;
	NewSlot.Layer = Item.Layer;
	PreviewSlots.Add(NewSlot);
	Wardrobe->SetPreviewOutfit(PreviewSlots);
}

bool UApartmentLifeWardrobeShoppingComponent::EquipAfterPurchase(FName ItemId)
{
	if (!Wardrobe.IsValid())
	{
		return false;
	}

	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return false;
	}

	return Wardrobe->EquipClothing(ItemId, Item.Layer);
}

EApartmentLifeClothingIncomeTier UApartmentLifeWardrobeShoppingComponent::GetCurrentIncomeTier() const
{
	if (!Simulation.IsValid())
	{
		return EApartmentLifeClothingIncomeTier::Budget;
	}
	return UApartmentLifeWardrobeCatalogLibrary::GetIncomeTierForSavings(Simulation->Finance.Savings);
}

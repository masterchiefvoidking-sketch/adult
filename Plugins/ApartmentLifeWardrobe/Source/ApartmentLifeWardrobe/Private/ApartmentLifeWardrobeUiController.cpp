// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeUiController.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeWardrobeShoppingComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "Engine/Engine.h"

void UApartmentLifeWardrobeUiController::InitializeContext(
	UApartmentLifeWardrobeComponent* InWardrobe,
	UApartmentLifeNPCSimulationComponent* InSimulation)
{
	Wardrobe = InWardrobe;
	Simulation = InSimulation;
	FilterQuery.bAllCategories = true;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::OpenWardrobe()
{
	bIsOpen = true;
	RefreshItemList();
	RefreshOverlay();
	BroadcastStateChanged();
}

void UApartmentLifeWardrobeUiController::CloseWardrobe()
{
	bIsOpen = false;
	if (Wardrobe.IsValid())
	{
		Wardrobe->ClearPreviewOutfit();
	}
	RefreshOverlay();
	BroadcastStateChanged();
}

void UApartmentLifeWardrobeUiController::ToggleWardrobe()
{
	if (bIsOpen)
	{
		CloseWardrobe();
	}
	else
	{
		OpenWardrobe();
	}
}

void UApartmentLifeWardrobeUiController::SetCategoryTab(EApartmentLifeWardrobeCategoryTab Tab)
{
	FilterQuery.CategoryTab = Tab;
	FilterQuery.bAllCategories = false;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetSlotFilter(EApartmentLifeClothingLayer Layer, bool bEnabled)
{
	FilterQuery.bFilterByLayer = bEnabled;
	FilterQuery.LayerFilter = Layer;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetSearchText(const FString& Text)
{
	FilterQuery.SearchText = Text;
	FilterQuery.bAllCategories = Text.IsEmpty() ? FilterQuery.bAllCategories : true;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetSortMode(EApartmentLifeClothingSortMode SortMode)
{
	FilterQuery.SortMode = SortMode;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetOwnedOnly(bool bEnabled)
{
	FilterQuery.bOwnedOnly = bEnabled;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetFavoritesOnly(bool bEnabled)
{
	FilterQuery.bFavoritesOnly = bEnabled;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SetShoppingMode(bool bEnabled)
{
	bShoppingMode = bEnabled;
	FilterQuery.bOwnedOnly = false;
	RefreshItemList();
}

void UApartmentLifeWardrobeUiController::SelectItemIndex(int32 Index)
{
	SelectedItemIndex = VisibleItemIds.IsValidIndex(Index) ? Index : INDEX_NONE;
	RefreshOverlay();
	BroadcastStateChanged();
}

void UApartmentLifeWardrobeUiController::PreviewSelectedItem()
{
	if (!Wardrobe.IsValid() || !VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		return;
	}

	const FName ItemId = VisibleItemIds[SelectedItemIndex];
	const EApartmentLifeClothingLayer Layer = ResolveLayerForItem(ItemId);

	TArray<FApartmentLifeEquippedClothingSlot> PreviewSlots = Wardrobe->GetEquippedSlots();
	PreviewSlots.RemoveAll([Layer](const FApartmentLifeEquippedClothingSlot& Slot) { return Slot.Layer == Layer; });

	FApartmentLifeEquippedClothingSlot NewSlot;
	NewSlot.ClothingItemId = ItemId;
	NewSlot.Layer = Layer;
	PreviewSlots.Add(NewSlot);
	Wardrobe->SetPreviewOutfit(PreviewSlots);
	RefreshOverlay();
}

void UApartmentLifeWardrobeUiController::EquipSelectedItem()
{
	if (!Wardrobe.IsValid() || !VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		return;
	}

	const FName ItemId = VisibleItemIds[SelectedItemIndex];
	const EApartmentLifeClothingLayer Layer = ResolveLayerForItem(ItemId);

	if (bShoppingMode)
	{
		if (AActor* Owner = GetOwner())
		{
			if (UApartmentLifeWardrobeShoppingComponent* Shopping = Owner->FindComponentByClass<UApartmentLifeWardrobeShoppingComponent>())
			{
				if (Shopping->TryPurchase(ItemId))
				{
					Shopping->EquipAfterPurchase(ItemId);
				}
			}
		}
	}
	else if (Wardrobe->OwnsClothing(ItemId))
	{
		Wardrobe->EquipClothing(ItemId, Layer);
	}

	RefreshItemList();
	RefreshOverlay();
}

void UApartmentLifeWardrobeUiController::UnequipSelectedLayer()
{
	if (!Wardrobe.IsValid() || !VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		return;
	}

	Wardrobe->UnequipLayer(ResolveLayerForItem(VisibleItemIds[SelectedItemIndex]));
	RefreshOverlay();
}

bool UApartmentLifeWardrobeUiController::PurchaseSelectedItem()
{
	if (!VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		return false;
	}

	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeWardrobeShoppingComponent* Shopping = Owner->FindComponentByClass<UApartmentLifeWardrobeShoppingComponent>())
		{
			const bool bPurchased = Shopping->TryPurchase(VisibleItemIds[SelectedItemIndex]);
			RefreshItemList();
			RefreshOverlay();
			return bPurchased;
		}
	}
	return false;
}

void UApartmentLifeWardrobeUiController::ToggleFavoriteSelectedItem()
{
	if (Wardrobe.IsValid() && VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		Wardrobe->ToggleFavoriteItem(VisibleItemIds[SelectedItemIndex]);
		RefreshItemList();
		RefreshOverlay();
	}
}

void UApartmentLifeWardrobeUiController::ApplyPreviewToEquipped()
{
	if (Wardrobe.IsValid())
	{
		Wardrobe->ApplyPreviewOutfit();
		RefreshOverlay();
	}
}

void UApartmentLifeWardrobeUiController::ClearPreview()
{
	if (Wardrobe.IsValid())
	{
		Wardrobe->ClearPreviewOutfit();
		RefreshOverlay();
	}
}

bool UApartmentLifeWardrobeUiController::IsPreviewDifferentFromEquipped() const
{
	return Wardrobe.IsValid() && !Wardrobe->ComparePreviewWithEquipped();
}

void UApartmentLifeWardrobeUiController::SaveCurrentOutfitPreset(
	FName PresetId,
	const FText& DisplayName,
	EApartmentLifeOutfitPresetType PresetType)
{
	if (Wardrobe.IsValid())
	{
		Wardrobe->SaveOutfitPreset(PresetId, DisplayName, PresetTypeToContext(PresetType), PresetType);
		RefreshOverlay();
	}
}

bool UApartmentLifeWardrobeUiController::RenameOutfitPreset(FName PresetId, const FText& NewDisplayName)
{
	return Wardrobe.IsValid() && Wardrobe->RenameOutfitPreset(PresetId, NewDisplayName);
}

bool UApartmentLifeWardrobeUiController::DeleteOutfitPreset(FName PresetId)
{
	return Wardrobe.IsValid() && Wardrobe->DeleteOutfitPreset(PresetId);
}

void UApartmentLifeWardrobeUiController::ApplyOutfitPresetByType(EApartmentLifeOutfitPresetType PresetType)
{
	if (!Wardrobe.IsValid())
	{
		return;
	}

	for (const FApartmentLifeFavoriteOutfit& Favorite : Wardrobe->GetFavoriteOutfits())
	{
		if (Favorite.PresetType == PresetType)
		{
			Wardrobe->ApplyFavoriteOutfit(Favorite.OutfitNameId);
			RefreshOverlay();
			return;
		}
	}
}

void UApartmentLifeWardrobeUiController::RecommendOutfitForActivity(FName ActivityId)
{
	if (Wardrobe.IsValid())
	{
		UApartmentLifeWardrobeLibrary::RecommendOutfitForActivity(Wardrobe.Get(), ActivityId);
		RefreshOverlay();
	}
}

void UApartmentLifeWardrobeUiController::RefreshItemList()
{
	if (!Wardrobe.IsValid())
	{
		VisibleItemIds.Empty();
		SelectedItemIndex = INDEX_NONE;
		return;
	}

	FApartmentLifeWardrobeFilterQuery Query = FilterQuery;
	if (bShoppingMode)
	{
		Query.bOwnedOnly = false;
	}

	VisibleItemIds = UApartmentLifeWardrobeLibrary::FilterAndSortCloset(Wardrobe.Get(), Query);

	if (bShoppingMode && Simulation.IsValid())
	{
		const EApartmentLifeClothingIncomeTier Tier = UApartmentLifeWardrobeCatalogLibrary::GetIncomeTierForSavings(Simulation->Finance.Savings);
		VisibleItemIds.RemoveAll([Tier](const FName& ItemId)
		{
			FApartmentLifeBuiltinClothingItem Item;
			if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
			{
				return true;
			}
			return static_cast<uint8>(Item.IncomeTier) > static_cast<uint8>(Tier);
		});
	}

	if (!VisibleItemIds.IsValidIndex(SelectedItemIndex))
	{
		SelectedItemIndex = VisibleItemIds.Num() > 0 ? 0 : INDEX_NONE;
	}

	BroadcastStateChanged();
}

void UApartmentLifeWardrobeUiController::RefreshOverlay() const
{
	if (!GEngine || !bIsOpen)
	{
		return;
	}

	const UEnum* CategoryEnum = StaticEnum<EApartmentLifeWardrobeCategoryTab>();
	const FString CategoryName = CategoryEnum
		? CategoryEnum->GetDisplayNameTextByValue(static_cast<int64>(FilterQuery.CategoryTab)).ToString()
		: TEXT("All");

	GEngine->AddOnScreenDebugMessage(
		8000,
		0.f,
		FColor::Cyan,
		FString::Printf(TEXT("Wardrobe [%s%s] — %d items | LMB orbit | wheel zoom"),
			*CategoryName,
			bShoppingMode ? TEXT(" | SHOP") : TEXT(""),
			VisibleItemIds.Num()));

	for (int32 Index = 0; Index < FMath::Min(VisibleItemIds.Num(), 8); ++Index)
	{
		FApartmentLifeBuiltinClothingItem Item;
		UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(VisibleItemIds[Index], Item);

		const bool bOwned = Wardrobe.IsValid() && Wardrobe->OwnsClothing(VisibleItemIds[Index]);
		const bool bFavorite = Wardrobe.IsValid() && Wardrobe->IsFavoriteItem(VisibleItemIds[Index]);
		const bool bSelected = Index == SelectedItemIndex;

		GEngine->AddOnScreenDebugMessage(
			8010 + Index,
			0.f,
			bSelected ? FColor::Yellow : FColor::White,
			FString::Printf(TEXT("  [%d] %s $%.0f %s%s%s"),
				Index + 1,
				*Item.DisplayName.ToString(),
				Item.Price,
				bOwned ? TEXT("[owned] ") : TEXT(""),
				bFavorite ? TEXT("[★] ") : TEXT(""),
				bSelected ? TEXT("<") : TEXT("")));
	}

	if (Wardrobe.IsValid())
	{
		const bool bDifferent = !Wardrobe->ComparePreviewWithEquipped();
		GEngine->AddOnScreenDebugMessage(
			8090,
			0.f,
			bDifferent ? FColor::Orange : FColor::Green,
			bDifferent ? TEXT("Preview differs from equipped — apply to confirm") : TEXT("Preview matches equipped outfit"));
	}

	if (Simulation.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(
			8091,
			0.f,
			FColor::Silver,
			FString::Printf(TEXT("Savings: $%.0f"), Simulation->Finance.Savings));
	}
}

FName UApartmentLifeWardrobeUiController::GetSelectedItemId() const
{
	return VisibleItemIds.IsValidIndex(SelectedItemIndex) ? VisibleItemIds[SelectedItemIndex] : NAME_None;
}

void UApartmentLifeWardrobeUiController::BroadcastStateChanged()
{
	OnWardrobeUiStateChanged.Broadcast();
}

EApartmentLifeOutfitContext UApartmentLifeWardrobeUiController::PresetTypeToContext(EApartmentLifeOutfitPresetType PresetType) const
{
	switch (PresetType)
	{
	case EApartmentLifeOutfitPresetType::Work: return EApartmentLifeOutfitContext::Work;
	case EApartmentLifeOutfitPresetType::Lounge: return EApartmentLifeOutfitContext::Lounge;
	case EApartmentLifeOutfitPresetType::Sleep: return EApartmentLifeOutfitContext::Sleep;
	case EApartmentLifeOutfitPresetType::Yoga: return EApartmentLifeOutfitContext::Athletic;
	case EApartmentLifeOutfitPresetType::GoingOut: return EApartmentLifeOutfitContext::Formal;
	default: return EApartmentLifeOutfitContext::Everyday;
	}
}

EApartmentLifeClothingLayer UApartmentLifeWardrobeUiController::ResolveLayerForItem(FName ItemId) const
{
	FApartmentLifeBuiltinClothingItem Item;
	if (UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return Item.Layer;
	}
	return EApartmentLifeClothingLayer::Tops;
}

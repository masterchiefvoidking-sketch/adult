// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "JsonObjectConverter.h"

UApartmentLifeWardrobeComponent::UApartmentLifeWardrobeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UApartmentLifeWardrobeComponent::EquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer)
{
	if (!OwnsClothing(ClothingItemId))
	{
		return false;
	}

	ResolveLayerConflicts(Layer);
	UnequipLayer(Layer);

	FApartmentLifeEquippedClothingSlot NewSlot;
	NewSlot.ClothingItemId = ClothingItemId;
	NewSlot.Layer = Layer;
	NewSlot.LaundryState = EApartmentLifeLaundryState::Clean;
	EquippedSlots.Add(NewSlot);
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
	return true;
}

void UApartmentLifeWardrobeComponent::UnequipLayer(EApartmentLifeClothingLayer Layer)
{
	if (EquippedSlots.RemoveAll([Layer](const FApartmentLifeEquippedClothingSlot& Slot) { return Slot.Layer == Layer; }) > 0)
	{
		RefreshVisuals();
		OnWardrobeUpdated.Broadcast();
	}
}

void UApartmentLifeWardrobeComponent::ResolveLayerConflicts(EApartmentLifeClothingLayer Layer)
{
	if (Layer == EApartmentLifeClothingLayer::Dress)
	{
		UnequipLayer(EApartmentLifeClothingLayer::Tops);
		UnequipLayer(EApartmentLifeClothingLayer::Bottoms);
	}
	else if (Layer == EApartmentLifeClothingLayer::Tops || Layer == EApartmentLifeClothingLayer::Bottoms)
	{
		UnequipLayer(EApartmentLifeClothingLayer::Dress);
	}
}

void UApartmentLifeWardrobeComponent::ApplyOutfitPreset(UApartmentLifeOutfitPresetData* Preset)
{
	if (!Preset) return;
	EquippedSlots = Preset->Slots;
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::ApplyFavoriteOutfit(FName OutfitNameId)
{
	for (const FApartmentLifeFavoriteOutfit& Favorite : FavoriteOutfits)
	{
		if (Favorite.OutfitNameId == OutfitNameId)
		{
			EquippedSlots = Favorite.Slots;
			RefreshVisuals();
			OnWardrobeUpdated.Broadcast();
			return;
		}
	}
}

bool UApartmentLifeWardrobeComponent::ApplyOutfitPresetByType(EApartmentLifeOutfitPresetType PresetType)
{
	for (const FApartmentLifeFavoriteOutfit& Favorite : FavoriteOutfits)
	{
		if (Favorite.PresetType == PresetType)
		{
			ApplyFavoriteOutfit(Favorite.OutfitNameId);
			return true;
		}
	}
	return false;
}

bool UApartmentLifeWardrobeComponent::PreviewEquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer)
{
	PreviewSlots = EquippedSlots;
	PreviewSlots.RemoveAll([Layer](const FApartmentLifeEquippedClothingSlot& Slot) { return Slot.Layer == Layer; });

	if (Layer == EApartmentLifeClothingLayer::Dress)
	{
		PreviewSlots.RemoveAll([](const FApartmentLifeEquippedClothingSlot& Slot)
		{
			return Slot.Layer == EApartmentLifeClothingLayer::Tops || Slot.Layer == EApartmentLifeClothingLayer::Bottoms;
		});
	}
	else if (Layer == EApartmentLifeClothingLayer::Tops || Layer == EApartmentLifeClothingLayer::Bottoms)
	{
		PreviewSlots.RemoveAll([](const FApartmentLifeEquippedClothingSlot& Slot)
		{
			return Slot.Layer == EApartmentLifeClothingLayer::Dress;
		});
	}

	FApartmentLifeEquippedClothingSlot NewSlot;
	NewSlot.ClothingItemId = ClothingItemId;
	NewSlot.Layer = Layer;
	PreviewSlots.Add(NewSlot);
	OnWardrobeUpdated.Broadcast();
	return true;
}

void UApartmentLifeWardrobeComponent::SaveCurrentAsFavorite(FName OutfitNameId, EApartmentLifeOutfitContext Context)
{
	SaveOutfitPreset(OutfitNameId, FText::FromName(OutfitNameId), Context, EApartmentLifeOutfitPresetType::Custom);
}

void UApartmentLifeWardrobeComponent::SaveOutfitPreset(
	FName OutfitNameId,
	const FText& DisplayName,
	EApartmentLifeOutfitContext Context,
	EApartmentLifeOutfitPresetType PresetType)
{
	FavoriteOutfits.RemoveAll([OutfitNameId](const FApartmentLifeFavoriteOutfit& Existing) { return Existing.OutfitNameId == OutfitNameId; });

	FApartmentLifeFavoriteOutfit Favorite;
	Favorite.OutfitNameId = OutfitNameId;
	Favorite.DisplayName = DisplayName;
	Favorite.Slots = EquippedSlots;
	Favorite.Context = Context;
	Favorite.PresetType = PresetType;
	FavoriteOutfits.Add(Favorite);
	OnWardrobeUpdated.Broadcast();
}

bool UApartmentLifeWardrobeComponent::RenameOutfitPreset(FName OutfitNameId, const FText& NewDisplayName)
{
	for (FApartmentLifeFavoriteOutfit& Favorite : FavoriteOutfits)
	{
		if (Favorite.OutfitNameId == OutfitNameId)
		{
			Favorite.DisplayName = NewDisplayName;
			OnWardrobeUpdated.Broadcast();
			return true;
		}
	}
	return false;
}

bool UApartmentLifeWardrobeComponent::DeleteOutfitPreset(FName OutfitNameId)
{
	const int32 Removed = FavoriteOutfits.RemoveAll([OutfitNameId](const FApartmentLifeFavoriteOutfit& Existing)
	{
		return Existing.OutfitNameId == OutfitNameId;
	});
	if (Removed > 0)
	{
		OnWardrobeUpdated.Broadcast();
		return true;
	}
	return false;
}

void UApartmentLifeWardrobeComponent::SelectOutfitForContext(EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather)
{
	TMap<EApartmentLifeClothingLayer, FName> BestPerLayer;
	TMap<EApartmentLifeClothingLayer, float> BestScores;

	for (const FName& ItemId : ClosetInventory)
	{
		FApartmentLifeBuiltinClothingItem Builtin;
		if (UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Builtin))
		{
			const float Score = ScoreBuiltinItem(Builtin, Occasion, Weather);
			if (Score <= 0.f) continue;
			const float* Existing = BestScores.Find(Builtin.Layer);
			if (!Existing || Score > *Existing)
			{
				BestScores.Add(Builtin.Layer, Score);
				BestPerLayer.Add(Builtin.Layer, ItemId);
			}
			continue;
		}

		if (UApartmentLifeClothingItemData* Item = ResolveClothingData(ItemId))
		{
			if (Item->Price > StyleProfile.MonthlyClothingBudget) continue;
			const float Score = ScoreClothingForContext(Item, Occasion, Weather);
			if (Score <= 0.f) continue;
			const float* Existing = BestScores.Find(Item->Layer);
			if (!Existing || Score > *Existing)
			{
				BestScores.Add(Item->Layer, Score);
				BestPerLayer.Add(Item->Layer, ItemId);
			}
		}
	}

	EquippedSlots.Empty();
	for (const TPair<EApartmentLifeClothingLayer, FName>& Pair : BestPerLayer)
	{
		FApartmentLifeEquippedClothingSlot Slot;
		Slot.ClothingItemId = Pair.Value;
		Slot.Layer = Pair.Key;
		EquippedSlots.Add(Slot);
	}
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::SelectOutfitForOutfitContext(EApartmentLifeOutfitContext Context, const FApartmentLifeWeatherState& Weather)
{
	float BestFavoriteScore = -1.f;
	FName BestFavoriteId = NAME_None;

	for (const FApartmentLifeFavoriteOutfit& Favorite : FavoriteOutfits)
	{
		if (Favorite.Context != Context)
		{
			continue;
		}

		float Score = 1.f;
		if (Favorite.PresetType == EApartmentLifeOutfitPresetType::Custom)
		{
			Score += 0.5f;
		}
		if (Score > BestFavoriteScore)
		{
			BestFavoriteScore = Score;
			BestFavoriteId = Favorite.OutfitNameId;
		}
	}

	if (!BestFavoriteId.IsNone())
	{
		ApplyFavoriteOutfit(BestFavoriteId);
		return;
	}

	TMap<EApartmentLifeClothingLayer, FName> BestPerLayer;
	TMap<EApartmentLifeClothingLayer, float> BestScores;

	for (const FName& ItemId : ClosetInventory)
	{
		FApartmentLifeBuiltinClothingItem Builtin;
		if (UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Builtin))
		{
			const float Score = ScoreBuiltinItemForContext(Builtin, Context, Weather);
			if (Score <= 0.f) continue;
			const float* Existing = BestScores.Find(Builtin.Layer);
			if (!Existing || Score > *Existing)
			{
				BestScores.Add(Builtin.Layer, Score);
				BestPerLayer.Add(Builtin.Layer, ItemId);
			}
			continue;
		}

		if (UApartmentLifeClothingItemData* Item = ResolveClothingData(ItemId))
		{
			const float Score = ScoreClothingForOutfitContext(Item, Context, Weather);
			if (Score <= 0.f) continue;
			const float* Existing = BestScores.Find(Item->Layer);
			if (!Existing || Score > *Existing)
			{
				BestScores.Add(Item->Layer, Score);
				BestPerLayer.Add(Item->Layer, ItemId);
			}
		}
	}

	EquippedSlots.Empty();
	for (const TPair<EApartmentLifeClothingLayer, FName>& Pair : BestPerLayer)
	{
		FApartmentLifeEquippedClothingSlot Slot;
		Slot.ClothingItemId = Pair.Value;
		Slot.Layer = Pair.Key;
		EquippedSlots.Add(Slot);
	}
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

bool UApartmentLifeWardrobeComponent::PurchaseClothing(UApartmentLifeNPCSimulationComponent* Simulation, FName ClothingItemId, int32 DayPurchased)
{
	if (!Simulation || OwnsClothing(ClothingItemId))
	{
		return false;
	}

	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ClothingItemId, Item))
	{
		return false;
	}

	if (Simulation->Finance.Savings < Item.Price)
	{
		Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress + 3.f, 0.f, 100.f);
		return false;
	}

	Simulation->Finance.Savings -= Item.Price;
	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeProgressionComponent* Progression = Owner->FindComponentByClass<UApartmentLifeProgressionComponent>())
		{
			UApartmentLifeFinanceLibrary::RecordSpending(Progression, EApartmentLifeShopCategory::Clothing, Item.Price);
		}
	}
	if (Item.Price > StyleProfile.MonthlyClothingBudget)
	{
		Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress + 2.f, 0.f, 100.f);
	}
	else
	{
		Simulation->Mood.Confidence = FMath::Clamp(Simulation->Mood.Confidence + 1.f, 0.f, 100.f);
	}

	AddOwnedClothing(ClothingItemId);

	FApartmentLifeShoppingRecord Record;
	Record.ClothingItemId = ClothingItemId;
	Record.PricePaid = Item.Price;
	Record.DayPurchased = DayPurchased;
	ShoppingHistory.Add(Record);
	OnWardrobeUpdated.Broadcast();
	return true;
}

void UApartmentLifeWardrobeComponent::AddOwnedClothing(FName ClothingItemId)
{
	ClosetInventory.AddUnique(ClothingItemId);
}

bool UApartmentLifeWardrobeComponent::OwnsClothing(FName ClothingItemId) const
{
	return ClosetInventory.Contains(ClothingItemId);
}

void UApartmentLifeWardrobeComponent::ToggleFavoriteItem(FName ClothingItemId)
{
	if (FavoriteItemIds.Contains(ClothingItemId))
	{
		FavoriteItemIds.Remove(ClothingItemId);
	}
	else
	{
		FavoriteItemIds.AddUnique(ClothingItemId);
	}
	OnWardrobeUpdated.Broadcast();
}

bool UApartmentLifeWardrobeComponent::IsFavoriteItem(FName ClothingItemId) const
{
	return FavoriteItemIds.Contains(ClothingItemId);
}

void UApartmentLifeWardrobeComponent::SetPreviewOutfit(const TArray<FApartmentLifeEquippedClothingSlot>& InPreviewSlots)
{
	PreviewSlots = InPreviewSlots;
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::ApplyPreviewOutfit()
{
	EquippedSlots = PreviewSlots;
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::ClearPreviewOutfit()
{
	PreviewSlots.Empty();
	OnWardrobeUpdated.Broadcast();
}

bool UApartmentLifeWardrobeComponent::ComparePreviewWithEquipped() const
{
	if (PreviewSlots.Num() != EquippedSlots.Num())
	{
		return false;
	}

	for (int32 Index = 0; Index < PreviewSlots.Num(); ++Index)
	{
		if (PreviewSlots[Index].ClothingItemId != EquippedSlots[Index].ClothingItemId
			|| PreviewSlots[Index].Layer != EquippedSlots[Index].Layer)
		{
			return false;
		}
	}
	return true;
}

void UApartmentLifeWardrobeComponent::MarkLayerDirty(EApartmentLifeClothingLayer Layer)
{
	for (FApartmentLifeEquippedClothingSlot& Slot : EquippedSlots)
	{
		if (Slot.Layer == Layer)
		{
			Slot.LaundryState = EApartmentLifeLaundryState::Dirty;
		}
	}
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::MarkEquippedWorn()
{
	for (FApartmentLifeEquippedClothingSlot& Slot : EquippedSlots)
	{
		if (Slot.LaundryState == EApartmentLifeLaundryState::Clean)
		{
			Slot.LaundryState = EApartmentLifeLaundryState::Worn;
		}
	}
}

void UApartmentLifeWardrobeComponent::StartLaundryCycle()
{
	for (FApartmentLifeEquippedClothingSlot& Slot : EquippedSlots)
	{
		if (Slot.LaundryState == EApartmentLifeLaundryState::Dirty)
		{
			Slot.LaundryState = EApartmentLifeLaundryState::InWash;
		}
	}
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::AdvanceLaundryCycle()
{
	for (FApartmentLifeEquippedClothingSlot& Slot : EquippedSlots)
	{
		switch (Slot.LaundryState)
		{
		case EApartmentLifeLaundryState::InWash:
			Slot.LaundryState = EApartmentLifeLaundryState::Drying;
			break;
		case EApartmentLifeLaundryState::Drying:
			Slot.LaundryState = EApartmentLifeLaundryState::Clean;
			break;
		case EApartmentLifeLaundryState::Worn:
			Slot.LaundryState = EApartmentLifeLaundryState::Dirty;
			break;
		default:
			break;
		}
	}
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::SetStyleProfile(const FApartmentLifeWardrobeStyleProfile& Profile)
{
	StyleProfile = Profile;
}

void UApartmentLifeWardrobeComponent::SetClosetInventory(const TArray<FName>& ItemIds)
{
	ClosetInventory = ItemIds;
}

void UApartmentLifeWardrobeComponent::RefreshVisuals()
{
	// Mesh compositing is content-driven; broadcast ensures UI/AnimBP refresh.
}

UApartmentLifeClothingItemData* UApartmentLifeWardrobeComponent::ResolveClothingData(FName ClothingItemId) const
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				return Cast<UApartmentLifeClothingItemData>(Registry->FindAsset(ClothingItemId));
			}
		}
	}
	return nullptr;
}

float UApartmentLifeWardrobeComponent::ScoreBuiltinItem(
	const FApartmentLifeBuiltinClothingItem& Item,
	EApartmentLifeOccasion Occasion,
	const FApartmentLifeWeatherState& Weather) const
{
	float Score = Item.ComfortRating;
	for (EApartmentLifeClothingStyleTag Tag : Item.StyleTags)
	{
		const UEnum* Enum = StaticEnum<EApartmentLifeClothingStyleTag>();
		if (Enum && StyleProfile.PreferredStyleTags.Contains(Enum->GetNameByValue(static_cast<int64>(Tag))))
		{
			Score += 0.5f;
		}
	}
	if (Weather.TemperatureCelsius < 10.f) Score += Item.WarmthRating * 2.f;
	else if (Weather.TemperatureCelsius > 28.f) Score += (1.f - Item.WarmthRating);
	if (Item.Price > StyleProfile.MonthlyClothingBudget * StyleProfile.IncomeTier) Score *= 0.3f;
	return Score;
}

float UApartmentLifeWardrobeComponent::ScoreBuiltinItemForContext(
	const FApartmentLifeBuiltinClothingItem& Item,
	EApartmentLifeOutfitContext Context,
	const FApartmentLifeWeatherState& Weather) const
{
	float Score = 1.f;
	if (Item.SuitableContexts.Contains(Context)) Score += 2.f;
	return Score * ScoreBuiltinItem(Item, EApartmentLifeOccasion::Everyday, Weather) * 0.5f;
}

float UApartmentLifeWardrobeComponent::ScoreClothingForContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather) const
{
	if (!Item) return 0.f;
	float Score = 1.f;
	if (Item->SuitableOccasions.Contains(Occasion)) Score += 2.f;
	if (Weather.TemperatureCelsius < 10.f) Score += Item->WarmthRating * 2.f;
	else if (Weather.TemperatureCelsius > 28.f) Score += (1.f - Item->WarmthRating);
	for (const FName& Tag : Item->StyleTags)
	{
		if (StyleProfile.PreferredStyleTags.Contains(Tag)) Score += 0.5f;
	}
	if (Item->Price > StyleProfile.MonthlyClothingBudget * StyleProfile.IncomeTier) Score *= 0.3f;
	return Score;
}

float UApartmentLifeWardrobeComponent::ScoreClothingForOutfitContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOutfitContext Context, const FApartmentLifeWeatherState& Weather) const
{
	if (!Item) return 0.f;
	float Score = 1.f;
	if (Item->SuitableContexts.Contains(Context)) Score += 2.f;
	return Score * ScoreClothingForContext(Item, EApartmentLifeOccasion::Everyday, Weather) * 0.5f;
}

FString UApartmentLifeWardrobeComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("wardrobe_%s"), *Owner->GetName());
	return TEXT("wardrobe_unknown");
}

void UApartmentLifeWardrobeComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString EquippedJson, FavoritesJson, StyleJson, PreviewJson, ShoppingJson;
	FJsonObjectConverter::UStructToJsonObjectString(EquippedSlots, EquippedJson);
	FJsonObjectConverter::UStructToJsonObjectString(FavoriteOutfits, FavoritesJson);
	FJsonObjectConverter::UStructToJsonObjectString(StyleProfile, StyleJson);
	FJsonObjectConverter::UStructToJsonObjectString(PreviewSlots, PreviewJson);
	FJsonObjectConverter::UStructToJsonObjectString(ShoppingHistory, ShoppingJson);
	OutData.Add(TEXT("EquippedSlots"), EquippedJson);
	OutData.Add(TEXT("FavoriteOutfits"), FavoritesJson);
	OutData.Add(TEXT("StyleProfile"), StyleJson);
	OutData.Add(TEXT("PreviewSlots"), PreviewJson);
	OutData.Add(TEXT("ShoppingHistory"), ShoppingJson);

	auto JoinIds = [](const TArray<FName>& Ids)
	{
		TArray<FString> Strings;
		for (const FName& Id : Ids) Strings.Add(Id.ToString());
		return FString::Join(Strings, TEXT(","));
	};

	OutData.Add(TEXT("ClosetInventory"), JoinIds(ClosetInventory));
	OutData.Add(TEXT("FavoriteItemIds"), JoinIds(FavoriteItemIds));
}

void UApartmentLifeWardrobeComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	auto RestoreIds = [](const FString& Joined, TArray<FName>& OutIds)
	{
		OutIds.Empty();
		TArray<FString> Strings;
		Joined.ParseIntoArray(Strings, TEXT(","), true);
		for (const FString& Id : Strings)
		{
			if (!Id.IsEmpty()) OutIds.Add(FName(*Id));
		}
	};

	if (const FString* J = InData.Find(TEXT("EquippedSlots"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &EquippedSlots);
	if (const FString* J = InData.Find(TEXT("FavoriteOutfits"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &FavoriteOutfits);
	if (const FString* J = InData.Find(TEXT("StyleProfile"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &StyleProfile);
	if (const FString* J = InData.Find(TEXT("PreviewSlots"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &PreviewSlots);
	if (const FString* J = InData.Find(TEXT("ShoppingHistory"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &ShoppingHistory);
	if (const FString* Inventory = InData.Find(TEXT("ClosetInventory"))) RestoreIds(*Inventory, ClosetInventory);
	if (const FString* Favorites = InData.Find(TEXT("FavoriteItemIds"))) RestoreIds(*Favorites, FavoriteItemIds);

	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

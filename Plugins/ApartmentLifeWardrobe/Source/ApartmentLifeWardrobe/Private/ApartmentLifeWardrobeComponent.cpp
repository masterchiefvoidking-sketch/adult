// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "JsonObjectConverter.h"

UApartmentLifeWardrobeComponent::UApartmentLifeWardrobeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UApartmentLifeWardrobeComponent::EquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer)
{
	UnequipLayer(Layer);
	FApartmentLifeEquippedClothingSlot NewSlot;
	NewSlot.ClothingItemId = ClothingItemId;
	NewSlot.Layer = Layer;
	EquippedSlots.Add(NewSlot);
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::UnequipLayer(EApartmentLifeClothingLayer Layer)
{
	EquippedSlots.RemoveAll([Layer](const FApartmentLifeEquippedClothingSlot& Slot) { return Slot.Layer == Layer; });
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

void UApartmentLifeWardrobeComponent::SaveCurrentAsFavorite(FName OutfitNameId, EApartmentLifeOutfitContext Context)
{
	FApartmentLifeFavoriteOutfit Favorite;
	Favorite.OutfitNameId = OutfitNameId;
	Favorite.Slots = EquippedSlots;
	Favorite.Context = Context;
	FavoriteOutfits.AddUnique(Favorite);
}

void UApartmentLifeWardrobeComponent::SelectOutfitForContext(EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather)
{
	TMap<EApartmentLifeClothingLayer, FName> BestPerLayer;
	TMap<EApartmentLifeClothingLayer, float> BestScores;

	for (const FName& ItemId : ClosetInventory)
	{
		UApartmentLifeClothingItemData* Item = ResolveClothingData(ItemId);
		if (!Item || Item->Price > StyleProfile.MonthlyClothingBudget) continue;

		const float Score = ScoreClothingForContext(Item, Occasion, Weather);
		if (Score <= 0.f) continue;

		const float* Existing = BestScores.Find(Item->Layer);
		if (!Existing || Score > *Existing)
		{
			BestScores.Add(Item->Layer, Score);
			BestPerLayer.Add(Item->Layer, ItemId);
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
	for (const FApartmentLifeFavoriteOutfit& Favorite : FavoriteOutfits)
	{
		if (Favorite.Context == Context)
		{
			ApplyFavoriteOutfit(Favorite.OutfitNameId);
			return;
		}
	}

	TMap<EApartmentLifeClothingLayer, FName> BestPerLayer;
	TMap<EApartmentLifeClothingLayer, float> BestScores;

	for (const FName& ItemId : ClosetInventory)
	{
		UApartmentLifeClothingItemData* Item = ResolveClothingData(ItemId);
		if (!Item) continue;

		const float Score = ScoreClothingForOutfitContext(Item, Context, Weather);
		if (Score <= 0.f) continue;

		const float* Existing = BestScores.Find(Item->Layer);
		if (!Existing || Score > *Existing)
		{
			BestScores.Add(Item->Layer, Score);
			BestPerLayer.Add(Item->Layer, ItemId);
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

void UApartmentLifeWardrobeComponent::MarkLayerDirty(EApartmentLifeClothingLayer Layer)
{
	for (FApartmentLifeEquippedClothingSlot& Slot : EquippedSlots)
	{
		if (Slot.Layer == Layer) Slot.LaundryState = EApartmentLifeLaundryState::Dirty;
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
}

void UApartmentLifeWardrobeComponent::SetStyleProfile(const FApartmentLifeWardrobeStyleProfile& Profile)
{
	StyleProfile = Profile;
}

void UApartmentLifeWardrobeComponent::RefreshVisuals() {}

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
	FString EquippedJson, FavoritesJson, StyleJson;
	FJsonObjectConverter::UStructToJsonObjectString(EquippedSlots, EquippedJson);
	FJsonObjectConverter::UStructToJsonObjectString(FavoriteOutfits, FavoritesJson);
	FJsonObjectConverter::UStructToJsonObjectString(StyleProfile, StyleJson);
	OutData.Add(TEXT("EquippedSlots"), EquippedJson);
	OutData.Add(TEXT("FavoriteOutfits"), FavoritesJson);
	OutData.Add(TEXT("StyleProfile"), StyleJson);
}

void UApartmentLifeWardrobeComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* J = InData.Find(TEXT("EquippedSlots"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &EquippedSlots);
	if (const FString* J = InData.Find(TEXT("FavoriteOutfits"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &FavoriteOutfits);
	if (const FString* J = InData.Find(TEXT("StyleProfile"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &StyleProfile);
	RefreshVisuals();
}

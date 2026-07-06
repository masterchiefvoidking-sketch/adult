// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeContentSettings.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "Components/SkeletalMeshComponent.h"

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
	EquippedSlots.RemoveAll([Layer](const FApartmentLifeEquippedClothingSlot& Slot)
	{
		return Slot.Layer == Layer;
	});
}

void UApartmentLifeWardrobeComponent::ApplyOutfitPreset(UApartmentLifeOutfitPresetData* Preset)
{
	if (!Preset)
	{
		return;
	}

	EquippedSlots = Preset->Slots;
	RefreshVisuals();
	OnWardrobeUpdated.Broadcast();
}

void UApartmentLifeWardrobeComponent::SelectOutfitForContext(EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather)
{
	TMap<EApartmentLifeClothingLayer, FName> BestPerLayer;
	TMap<EApartmentLifeClothingLayer, float> BestScores;

	for (const FName& ItemId : ClosetInventory)
	{
		UApartmentLifeClothingItemData* Item = ResolveClothingData(ItemId);
		if (!Item)
		{
			continue;
		}

		if (Item->bRequiresMatureContent && !UApartmentLifeContentSettings::IsNudityAllowed())
		{
			continue;
		}

		const float Score = ScoreClothingForContext(Item, Occasion, Weather);
		if (Score <= 0.f)
		{
			continue;
		}

		const float* ExistingScore = BestScores.Find(Item->Layer);
		if (!ExistingScore || Score > *ExistingScore)
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
		if (Slot.Layer == Layer)
		{
			Slot.LaundryState = EApartmentLifeLaundryState::Dirty;
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
}

void UApartmentLifeWardrobeComponent::RefreshVisuals()
{
	// Visual attachment is implemented in Blueprint/child components using equipped slot data.
	// C++ owns state; meshes and cloth physics are data-driven per clothing asset.
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

float UApartmentLifeWardrobeComponent::ScoreClothingForContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather) const
{
	if (!Item)
	{
		return 0.f;
	}

	float Score = 1.f;

	if (Item->SuitableOccasions.Contains(Occasion))
	{
		Score += 2.f;
	}

	if (Weather.TemperatureCelsius < 10.f)
	{
		Score += Item->WarmthRating * 2.f;
	}
	else if (Weather.TemperatureCelsius > 28.f)
	{
		Score += (1.f - Item->WarmthRating);
	}

	return Score;
}

FString UApartmentLifeWardrobeComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("wardrobe_%s"), *Owner->GetName());
	}
	return TEXT("wardrobe_unknown");
}

void UApartmentLifeWardrobeComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("EquippedCount"), FString::FromInt(EquippedSlots.Num()));
}

void UApartmentLifeWardrobeComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	RefreshVisuals();
}

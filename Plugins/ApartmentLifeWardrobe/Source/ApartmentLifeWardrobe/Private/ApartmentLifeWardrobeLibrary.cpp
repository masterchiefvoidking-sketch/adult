// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

TArray<FName> UApartmentLifeWardrobeLibrary::FilterAndSortCloset(
	const UApartmentLifeWardrobeComponent* Wardrobe,
	const FApartmentLifeWardrobeFilterQuery& Query)
{
	TArray<FName> Results;

	for (const FApartmentLifeBuiltinClothingItem& Item : UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog())
	{
		const bool bOwned = Wardrobe && Wardrobe->OwnsClothing(Item.ItemId);
		if (Query.bOwnedOnly && !bOwned)
		{
			continue;
		}
		if (Query.bFavoritesOnly && Wardrobe && !Wardrobe->IsFavoriteItem(Item.ItemId))
		{
			continue;
		}
		if (Query.bFilterByLayer && Item.Layer != Query.LayerFilter)
		{
			continue;
		}
		if (!Query.bAllCategories && Item.CategoryTab != Query.CategoryTab)
		{
			continue;
		}
		if (!Query.SearchText.IsEmpty())
		{
			const FString Haystack = Item.DisplayName.ToString();
			if (!Haystack.ToLower().Contains(Query.SearchText.ToLower()))
			{
				continue;
			}
		}
		Results.Add(Item.ItemId);
	}

	Results.Sort([&Query, Wardrobe](const FName& A, const FName& B)
	{
		FApartmentLifeBuiltinClothingItem ItemA, ItemB;
		UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(A, ItemA);
		UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(B, ItemB);

		switch (Query.SortMode)
		{
		case EApartmentLifeClothingSortMode::PriceAsc: return ItemA.Price < ItemB.Price;
		case EApartmentLifeClothingSortMode::PriceDesc: return ItemA.Price > ItemB.Price;
		case EApartmentLifeClothingSortMode::Owned:
			if (Wardrobe)
			{
				const bool bOwnedA = Wardrobe->OwnsClothing(A);
				const bool bOwnedB = Wardrobe->OwnsClothing(B);
				if (bOwnedA != bOwnedB) return bOwnedA;
			}
			break;
		case EApartmentLifeClothingSortMode::Favorites:
			if (Wardrobe)
			{
				const bool bFavA = Wardrobe->IsFavoriteItem(A);
				const bool bFavB = Wardrobe->IsFavoriteItem(B);
				if (bFavA != bFavB) return bFavA;
			}
			break;
		case EApartmentLifeClothingSortMode::Style:
			return ScoreItemForStyleTags(A, Wardrobe ? Wardrobe->GetStyleProfile().PreferredStyleTags : TArray<FName>())
				> ScoreItemForStyleTags(B, Wardrobe ? Wardrobe->GetStyleProfile().PreferredStyleTags : TArray<FName>());
		default: break;
		}
		return A.LexicalLess(B);
	});

	return Results;
}

bool UApartmentLifeWardrobeLibrary::ShouldAutoRecommendOutfitForActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	return !Id.Contains(TEXT("wardrobe")) && !Id.Contains(TEXT("preview"));
}

void UApartmentLifeWardrobeLibrary::RecommendOutfitForActivity(UApartmentLifeWardrobeComponent* Wardrobe, FName ActivityId)
{
	if (!Wardrobe)
	{
		return;
	}

	const FString Id = ActivityId.ToString().ToLower();
	EApartmentLifeOutfitContext Context = EApartmentLifeOutfitContext::Everyday;

	if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("fitness")))
	{
		Context = EApartmentLifeOutfitContext::Athletic;
	}
	else if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("computer")))
	{
		Context = EApartmentLifeOutfitContext::Work;
	}
	else if (Id.Contains(TEXT("sleep")))
	{
		Context = EApartmentLifeOutfitContext::Sleep;
	}
	else if (Id.Contains(TEXT("relax")) || Id.Contains(TEXT("lounge")))
	{
		Context = EApartmentLifeOutfitContext::Lounge;
	}

	FApartmentLifeWeatherState Weather;
	Wardrobe->SelectOutfitForOutfitContext(Context, Weather);
}

FApartmentLifeOutfitMoodEffect UApartmentLifeWardrobeLibrary::ComputeOutfitMoodEffect(
	const UApartmentLifeWardrobeComponent* Wardrobe,
	FName ActivityId)
{
	FApartmentLifeOutfitMoodEffect Effect;
	if (!Wardrobe)
	{
		return Effect;
	}

	float TotalComfort = 0.f;
	int32 SlotCount = 0;
	bool bFavoriteOutfit = false;
	bool bBadActivityMatch = false;

	for (const FApartmentLifeEquippedClothingSlot& Slot : Wardrobe->GetEquippedSlots())
	{
		FApartmentLifeBuiltinClothingItem Item;
		if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(Slot.ClothingItemId, Item))
		{
			continue;
		}

		++SlotCount;
		TotalComfort += Item.ComfortRating;

		if (Wardrobe->IsFavoriteItem(Slot.ClothingItemId))
		{
			bFavoriteOutfit = true;
		}

		if (Slot.LaundryState == EApartmentLifeLaundryState::Dirty || Slot.LaundryState == EApartmentLifeLaundryState::Worn)
		{
			Effect.ComfortDelta -= 3.f;
			Effect.ConfidenceDelta -= 2.f;
		}

		if (!ActivityId.IsNone())
		{
			const FString Act = ActivityId.ToString().ToLower();
			if (Act.Contains(TEXT("yoga")) && !Item.bYogaCompatible)
			{
				bBadActivityMatch = true;
			}
		}
	}

	if (SlotCount > 0)
	{
		const float AvgComfort = TotalComfort / SlotCount;
		Effect.MoodDelta += AvgComfort * 2.f;
		Effect.ComfortDelta += AvgComfort * 5.f;
	}

	if (bFavoriteOutfit)
	{
		Effect.ConfidenceDelta += 5.f;
		Effect.MoodDelta += 2.f;
	}

	if (bBadActivityMatch)
	{
		Effect.ComfortDelta -= 5.f;
		Effect.EnergyDelta -= 2.f;
	}

	return Effect;
}

void UApartmentLifeWardrobeLibrary::ApplyOutfitMoodEffect(UApartmentLifeNPCSimulationComponent* Simulation, const FApartmentLifeOutfitMoodEffect& Effect)
{
	if (!Simulation)
	{
		return;
	}

	Simulation->Mood.OverallMood = FMath::Clamp(Simulation->Mood.OverallMood + Effect.MoodDelta, 0.f, 100.f);
	Simulation->Mood.Confidence = FMath::Clamp(Simulation->Mood.Confidence + Effect.ConfidenceDelta, 0.f, 100.f);
	Simulation->Mood.Comfort = FMath::Clamp(Simulation->Mood.Comfort + Effect.ComfortDelta, 0.f, 100.f);
	Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy + Effect.EnergyDelta, 0.f, 100.f);
}

bool UApartmentLifeWardrobeLibrary::CanAffordItem(const UApartmentLifeNPCSimulationComponent* Simulation, FName ItemId)
{
	if (!Simulation)
	{
		return false;
	}

	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return false;
	}

	return Simulation->Finance.Savings >= Item.Price;
}

float UApartmentLifeWardrobeLibrary::ScoreItemForStyleTags(FName ItemId, const TArray<FName>& PreferredTags)
{
	FApartmentLifeBuiltinClothingItem Item;
	if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		return 0.f;
	}

	float Score = 1.f;
	for (EApartmentLifeClothingStyleTag Tag : Item.StyleTags)
	{
		const UEnum* Enum = StaticEnum<EApartmentLifeClothingStyleTag>();
		if (Enum && PreferredTags.Contains(Enum->GetNameByValue(static_cast<int64>(Tag))))
		{
			Score += 0.5f;
		}
	}
	return Score;
}

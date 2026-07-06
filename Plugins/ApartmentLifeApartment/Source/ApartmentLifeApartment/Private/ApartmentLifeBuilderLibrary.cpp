// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeBuilderLibrary.h"

FTransform UApartmentLifeBuilderLibrary::SnapTransformToGrid(const FTransform& Transform, float GridSize)
{
	if (GridSize <= 0.f)
	{
		return Transform;
	}

	FTransform Snapped = Transform;
	FVector Location = Snapped.GetLocation();
	Location.X = FMath::GridSnap(Location.X, GridSize);
	Location.Y = FMath::GridSnap(Location.Y, GridSize);
	Location.Z = FMath::GridSnap(Location.Z, GridSize);
	Snapped.SetLocation(Location);

	FRotator Rotation = Snapped.Rotator();
	Rotation.Yaw = FMath::GridSnap(Rotation.Yaw, 90.f);
	Snapped.SetRotation(Rotation.Quaternion());

	return Snapped;
}

bool UApartmentLifeBuilderLibrary::ValidatePlacement(
	const UApartmentLifeFurnitureItemData* ItemData,
	const FTransform& Transform,
	EApartmentLifeRoomType Room,
	const FBox& RoomBounds,
	const TArray<FApartmentLifePlacedFurnitureInstance>& ExistingPlacements)
{
	if (!ItemData)
	{
		return false;
	}

	if (ItemData->AllowedRooms.Num() > 0 && !ItemData->AllowedRooms.Contains(Room))
	{
		return false;
	}

	const FVector Location = Transform.GetLocation();
	const FVector HalfExtent = ItemData->FootprintSize * 0.5f;
	const FBox ProposedBox(Location - HalfExtent, Location + HalfExtent);

	if (!RoomBounds.IsInside(ProposedBox))
	{
		return false;
	}

	for (const FApartmentLifePlacedFurnitureInstance& Existing : ExistingPlacements)
	{
		if (Existing.Room != Room || Existing.State != EApartmentLifeInventoryItemState::Placed)
		{
			continue;
		}
		// Overlap check uses same footprint — resolved item lookup happens at higher level for precision
		const FVector ExistingLoc = Existing.Transform.GetLocation();
		const float DistSq = FVector::DistSquared(Location, ExistingLoc);
		if (DistSq < FMath::Square(ItemData->FootprintSize.GetMax() * 0.5f))
		{
			return false;
		}
	}

	return true;
}

FApartmentLifeRoomScore UApartmentLifeBuilderLibrary::CalculateRoomScore(
	EApartmentLifeRoomType Room,
	const TArray<FApartmentLifePlacedFurnitureInstance>& Placements,
	const TArray<UApartmentLifeFurnitureItemData*>& ResolvedItems,
	const TArray<EApartmentLifeStyleTag>& PreferredStyles,
	float BaseLighting,
	float ApartmentNaturalLight)
{
	FApartmentLifeRoomScore Score;
	Score.Room = Room;

	int32 ItemCount = 0;
	float TotalComfort = 0.f;
	float TotalCleanliness = 0.f;
	float TotalStyle = 0.f;
	float TotalFunctionality = 0.f;
	float TotalEntertainment = 0.f;
	float TotalLighting = BaseLighting + ApartmentNaturalLight * 0.3f;

	for (int32 Index = 0; Index < Placements.Num(); ++Index)
	{
		const FApartmentLifePlacedFurnitureInstance& Placement = Placements[Index];
		if (Placement.Room != Room || Placement.State != EApartmentLifeInventoryItemState::Placed)
		{
			continue;
		}

		UApartmentLifeFurnitureItemData* Item = Index < ResolvedItems.Num() ? ResolvedItems[Index] : nullptr;
		if (!Item)
		{
			continue;
		}

		++ItemCount;
		TotalComfort += Item->ComfortValue * (Placement.Condition.Cleanliness / 100.f);
		TotalCleanliness += Placement.Condition.Cleanliness;
		TotalEntertainment += Item->EntertainmentValue;
		TotalLighting += Item->LightingContribution;
		TotalFunctionality += Item->InteractionPoints.Num() > 0 ? 60.f : 20.f;

		float StylePoints = 0.f;
		for (EApartmentLifeStyleTag Tag : Item->StyleTags)
		{
			if (PreferredStyles.Contains(Tag))
			{
				StylePoints += 20.f;
			}
		}
		TotalStyle += FMath::Min(StylePoints, 100.f);

		if (Placement.Condition.bIsBroken)
		{
			TotalFunctionality -= 20.f;
		}
	}

	if (ItemCount > 0)
	{
		Score.Comfort = TotalComfort / ItemCount;
		Score.Cleanliness = TotalCleanliness / ItemCount;
		Score.StyleMatch = TotalStyle / ItemCount;
		Score.Functionality = FMath::Clamp(TotalFunctionality / ItemCount, 0.f, 100.f);
		Score.EntertainmentValue = TotalEntertainment / ItemCount;
		Score.LightingQuality = FMath::Clamp(TotalLighting / ItemCount, 0.f, 100.f);
	}

	Score.Clutter = FMath::Clamp(static_cast<float>(ItemCount) * 8.f - 20.f, 0.f, 100.f);

	return Score;
}

float UApartmentLifeBuilderLibrary::ScoreFurnitureForDecorator(
	const UApartmentLifeFurnitureItemData* Item,
	const FApartmentLifeDecorationProfile& Profile,
	float AvailableBudget,
	float RemainingFloorSpace,
	EApartmentLifeRoomType TargetRoom)
{
	if (!Item || Item->Price > AvailableBudget)
	{
		return 0.f;
	}

	const float FootprintArea = Item->FootprintSize.X * Item->FootprintSize.Y;
	if (FootprintArea > RemainingFloorSpace)
	{
		return 0.f;
	}

	if (Item->AllowedRooms.Num() > 0 && !Item->AllowedRooms.Contains(TargetRoom))
	{
		return 0.f;
	}

	float Score = 0.3f;

	for (EApartmentLifeStyleTag Tag : Item->StyleTags)
	{
		if (Profile.PreferredStyles.Contains(Tag))
		{
			Score += 0.25f;
		}
	}

	if (Item->Category == EApartmentLifeFurnitureCategory::Computer || Item->Category == EApartmentLifeFurnitureCategory::TV)
	{
		Score += Profile.GamingInterest * 0.4f;
	}
	if (Item->Category == EApartmentLifeFurnitureCategory::WorkoutEquipment)
	{
		Score += Profile.FitnessInterest * 0.5f;
	}
	if (Item->Category == EApartmentLifeFurnitureCategory::Bookshelf)
	{
		Score += Profile.Creativity * 0.2f;
	}

	Score += (1.f - Profile.Tidiness) * (Item->Category == EApartmentLifeFurnitureCategory::Decoration ? 0.2f : 0.f);
	Score += Profile.Tidiness * (Item->Category == EApartmentLifeFurnitureCategory::StorageBin ? 0.3f : 0.f);

	const float BudgetFit = 1.f - FMath::Clamp(Item->Price / FMath::Max(AvailableBudget, 1.f), 0.f, 1.f);
	Score += BudgetFit * Profile.SpendingHabits * 0.2f;

	return FMath::Clamp(Score, 0.f, 2.f);
}

TArray<FApartmentLifePlacedFurnitureInstance> UApartmentLifeBuilderLibrary::RecommendDecorations(
	const FApartmentLifeDecorationProfile& Profile,
	float AvailableBudget,
	float RemainingFloorSpace,
	EApartmentLifeRoomType TargetRoom,
	const TArray<UApartmentLifeFurnitureItemData*>& Catalog)
{
	TArray<FApartmentLifePlacedFurnitureInstance> Recommendations;
	float BudgetLeft = AvailableBudget;
	float SpaceLeft = RemainingFloorSpace;

	TArray<TPair<float, UApartmentLifeFurnitureItemData*>> Scored;
	for (UApartmentLifeFurnitureItemData* Item : Catalog)
	{
		const float Score = ScoreFurnitureForDecorator(Item, Profile, BudgetLeft, SpaceLeft, TargetRoom);
		if (Score > 0.3f)
		{
			Scored.Add(TPair<float, UApartmentLifeFurnitureItemData*>(Score, Item));
		}
	}

	Scored.Sort([](const TPair<float, UApartmentLifeFurnitureItemData*>& A, const TPair<float, UApartmentLifeFurnitureItemData*>& B)
	{
		return A.Key > B.Key;
	});

	for (const TPair<float, UApartmentLifeFurnitureItemData*>& Entry : Scored)
	{
		UApartmentLifeFurnitureItemData* Item = Entry.Value;
		if (!Item || Item->Price > BudgetLeft)
		{
			continue;
		}

		const float Area = Item->FootprintSize.X * Item->FootprintSize.Y;
		if (Area > SpaceLeft)
		{
			continue;
		}

		FApartmentLifePlacedFurnitureInstance Instance;
		Instance.InstanceId = FGuid::NewGuid();
		Instance.FurnitureItemId = Item->AssetId;
		Instance.Room = TargetRoom;
		Instance.State = EApartmentLifeInventoryItemState::Owned;
		Instance.Condition.Durability = Item->DurabilityMax;
		Instance.Condition.Cleanliness = 100.f;
		Recommendations.Add(Instance);

		BudgetLeft -= Item->Price;
		SpaceLeft -= Area;

		if (BudgetLeft < 50.f || SpaceLeft < 100.f)
		{
			break;
		}
	}

	return Recommendations;
}

void UApartmentLifeBuilderLibrary::ApplyWearAndDirt(FApartmentLifeObjectCondition& Condition, float DeltaHours, float TidinessFactor)
{
	const float DirtRate = 0.02f * DeltaHours * (1.1f - TidinessFactor);
	Condition.DirtLevel = FMath::Clamp(Condition.DirtLevel + DirtRate, 0.f, 100.f);
	Condition.Cleanliness = FMath::Clamp(100.f - Condition.DirtLevel, 0.f, 100.f);

	Condition.Durability = FMath::Max(0.f, Condition.Durability - DeltaHours * 0.001f);

	if (Condition.DirtLevel > 30.f && !Condition.ActiveDirtTypes.Contains(EApartmentLifeDirtType::Dust))
	{
		Condition.ActiveDirtTypes.Add(EApartmentLifeDirtType::Dust);
	}

	if (Condition.Durability < 20.f)
	{
		Condition.bNeedsMaintenance = true;
	}
	if (Condition.Durability <= 0.f)
	{
		Condition.bIsBroken = true;
		Condition.ActiveDirtTypes.AddUnique(EApartmentLifeDirtType::Broken);
	}
}

float UApartmentLifeBuilderLibrary::GetSellPrice(const UApartmentLifeFurnitureItemData* Item, const FApartmentLifeObjectCondition& Condition)
{
	if (!Item)
	{
		return 0.f;
	}

	float Multiplier = 0.5f;
	Multiplier *= Condition.Cleanliness / 100.f;
	Multiplier *= Condition.Durability / 100.f;
	if (Condition.bIsBroken)
	{
		Multiplier *= 0.1f;
	}
	return Item->Price * Multiplier;
}

TArray<EApartmentLifeFurnitureInteraction> UApartmentLifeBuilderLibrary::GetInteractionsForCategory(EApartmentLifeFurnitureCategory Category)
{
	TArray<EApartmentLifeFurnitureInteraction> Interactions;

	switch (Category)
	{
	case EApartmentLifeFurnitureCategory::Bed:
		Interactions = { EApartmentLifeFurnitureInteraction::Sleep, EApartmentLifeFurnitureInteraction::Nap, EApartmentLifeFurnitureInteraction::Sit, EApartmentLifeFurnitureInteraction::Read, EApartmentLifeFurnitureInteraction::PhoneBrowse };
		break;
	case EApartmentLifeFurnitureCategory::Sofa:
		Interactions = { EApartmentLifeFurnitureInteraction::Sit, EApartmentLifeFurnitureInteraction::WatchTV, EApartmentLifeFurnitureInteraction::Talk, EApartmentLifeFurnitureInteraction::Relax, EApartmentLifeFurnitureInteraction::ListenMusic, EApartmentLifeFurnitureInteraction::PlayGame, EApartmentLifeFurnitureInteraction::PhoneBrowse };
		break;
	case EApartmentLifeFurnitureCategory::TV:
		Interactions = { EApartmentLifeFurnitureInteraction::WatchTV, EApartmentLifeFurnitureInteraction::Relax };
		break;
	case EApartmentLifeFurnitureCategory::Desk:
		Interactions = { EApartmentLifeFurnitureInteraction::UseComputer, EApartmentLifeFurnitureInteraction::Study, EApartmentLifeFurnitureInteraction::WorkFromHome, EApartmentLifeFurnitureInteraction::Freelance, EApartmentLifeFurnitureInteraction::BudgetReview };
		break;
	case EApartmentLifeFurnitureCategory::Mirror:
		Interactions = { EApartmentLifeFurnitureInteraction::Groom, EApartmentLifeFurnitureInteraction::CheckOutfit };
		break;
	case EApartmentLifeFurnitureCategory::Closet:
		Interactions = { EApartmentLifeFurnitureInteraction::ChangeOutfit, EApartmentLifeFurnitureInteraction::ManageWardrobe, EApartmentLifeFurnitureInteraction::OrganizeCloset };
		break;
	case EApartmentLifeFurnitureCategory::KitchenAppliance:
		Interactions = { EApartmentLifeFurnitureInteraction::Cook, EApartmentLifeFurnitureInteraction::Eat, EApartmentLifeFurnitureInteraction::CleanDishes, EApartmentLifeFurnitureInteraction::MakeSnack, EApartmentLifeFurnitureInteraction::GroceryCheck };
		break;
	case EApartmentLifeFurnitureCategory::BathroomFixture:
		Interactions = { EApartmentLifeFurnitureInteraction::Shower, EApartmentLifeFurnitureInteraction::BrushTeeth, EApartmentLifeFurnitureInteraction::SkinCare, EApartmentLifeFurnitureInteraction::HairCare };
		break;
	case EApartmentLifeFurnitureCategory::WorkoutEquipment:
		Interactions = { EApartmentLifeFurnitureInteraction::Yoga, EApartmentLifeFurnitureInteraction::Stretch, EApartmentLifeFurnitureInteraction::Exercise, EApartmentLifeFurnitureInteraction::Breathe };
		break;
	case EApartmentLifeFurnitureCategory::LaundryMachine:
		Interactions = { EApartmentLifeFurnitureInteraction::DoLaundry };
		break;
	default:
		Interactions = { EApartmentLifeFurnitureInteraction::Sit };
		break;
	}

	return Interactions;
}

FName UApartmentLifeBuilderLibrary::GetActivityIdForInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	switch (Interaction)
	{
	case EApartmentLifeFurnitureInteraction::Sleep: return FName(TEXT("activity.sleep.bed"));
	case EApartmentLifeFurnitureInteraction::Nap: return FName(TEXT("activity.sleep.nap"));
	case EApartmentLifeFurnitureInteraction::Read: return FName(TEXT("activity.read.book"));
	case EApartmentLifeFurnitureInteraction::WatchTV: return FName(TEXT("activity.entertainment.tv"));
	case EApartmentLifeFurnitureInteraction::UseComputer: return FName(TEXT("activity.work.computer"));
	case EApartmentLifeFurnitureInteraction::WorkFromHome: return FName(TEXT("activity.work.computer"));
	case EApartmentLifeFurnitureInteraction::Shower: return FName(TEXT("activity.hygiene.shower"));
	case EApartmentLifeFurnitureInteraction::Groom: return FName(TEXT("activity.groom.mirror"));
	case EApartmentLifeFurnitureInteraction::ManageWardrobe: return FName(TEXT("activity.dress.wardrobe"));
	case EApartmentLifeFurnitureInteraction::Yoga: return FName(TEXT("activity.fitness.yoga"));
	case EApartmentLifeFurnitureInteraction::Cook: return FName(TEXT("activity.cook.prepare"));
	case EApartmentLifeFurnitureInteraction::Eat: return FName(TEXT("activity.eat.meal"));
	default: return FName(TEXT("activity.relax.sit"));
	}
}

FName UApartmentLifeBuilderLibrary::GetDefaultSocketForInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	switch (Interaction)
	{
	case EApartmentLifeFurnitureInteraction::Sleep:
	case EApartmentLifeFurnitureInteraction::Nap: return FName(TEXT("Socket_Lie"));
	case EApartmentLifeFurnitureInteraction::Sit:
	case EApartmentLifeFurnitureInteraction::Relax:
	case EApartmentLifeFurnitureInteraction::WatchTV: return FName(TEXT("Socket_Sit"));
	case EApartmentLifeFurnitureInteraction::UseComputer:
	case EApartmentLifeFurnitureInteraction::WorkFromHome:
	case EApartmentLifeFurnitureInteraction::Study: return FName(TEXT("Socket_Chair"));
	case EApartmentLifeFurnitureInteraction::Groom: return FName(TEXT("Socket_Mirror"));
	case EApartmentLifeFurnitureInteraction::ManageWardrobe:
	case EApartmentLifeFurnitureInteraction::ChangeOutfit: return FName(TEXT("Socket_Closet"));
	case EApartmentLifeFurnitureInteraction::Shower: return FName(TEXT("Socket_Shower"));
	case EApartmentLifeFurnitureInteraction::Yoga: return FName(TEXT("Socket_Mat"));
	case EApartmentLifeFurnitureInteraction::Cook:
	case EApartmentLifeFurnitureInteraction::Eat: return FName(TEXT("Socket_Interact"));
	default: return FName(TEXT("Socket_Interact"));
	}
}

FText UApartmentLifeBuilderLibrary::GetDisplayNameForInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	switch (Interaction)
	{
	case EApartmentLifeFurnitureInteraction::Shower: return FText::FromString(TEXT("Shower Routine"));
	case EApartmentLifeFurnitureInteraction::UseComputer: return FText::FromString(TEXT("Remote Work"));
	case EApartmentLifeFurnitureInteraction::WorkFromHome: return FText::FromString(TEXT("Remote Work"));
	case EApartmentLifeFurnitureInteraction::Yoga: return FText::FromString(TEXT("Yoga"));
	case EApartmentLifeFurnitureInteraction::Sleep: return FText::FromString(TEXT("Sleep"));
	case EApartmentLifeFurnitureInteraction::Groom: return FText::FromString(TEXT("Groom"));
	case EApartmentLifeFurnitureInteraction::ManageWardrobe: return FText::FromString(TEXT("Open Wardrobe"));
	default: return UEnum::GetDisplayValueAsText(Interaction);
	}
}

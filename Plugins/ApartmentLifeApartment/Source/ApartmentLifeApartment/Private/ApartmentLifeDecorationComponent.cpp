// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDecorationComponent.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeFurnitureData.h"
#include "ApartmentLifeInventoryComponent.h"

void UApartmentLifeDecorationComponent::RunDecorationPass(float AvailableBudget)
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit)
	{
		return;
	}

	const float Budget = FMath::Min(AvailableBudget, Profile.MonthlyBudget);
	const float Space = GetRemainingFloorSpace();

	const TArray<FApartmentLifePlacedFurnitureInstance> Recommendations = UApartmentLifeBuilderLibrary::RecommendDecorations(
		Profile,
		Budget,
		Space,
		EApartmentLifeRoomType::LivingRoom,
		GetFurnitureCatalog());

	for (const FApartmentLifePlacedFurnitureInstance& Rec : Recommendations)
	{
		FApartmentLifePlacedFurnitureInstance Placed = Rec;
		Placed.Transform.SetLocation(FVector(FMath::FRandRange(-100.f, 100.f), FMath::FRandRange(-100.f, 100.f), 0.f));
		if (Unit->PlaceFurnitureInstance(Placed))
		{
			if (UApartmentLifeInventoryComponent* Inventory = Unit->FindComponentByClass<UApartmentLifeInventoryComponent>())
			{
				const FGuid EntryId = Inventory->AddItem(Placed.FurnitureItemId, EApartmentLifeInventoryItemState::Placed);
				Inventory->LinkToPlacement(EntryId, Placed.InstanceId);
			}
			OnDecorationPlaced.Broadcast(Placed.InstanceId);
		}
	}
}

bool UApartmentLifeDecorationComponent::ApplyUpgrade(FName UpgradeAssetId, float AvailableBudget)
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				UApartmentLifeApartmentUpgradeData* Upgrade = Cast<UApartmentLifeApartmentUpgradeData>(Registry->FindAsset(UpgradeAssetId));
				if (!Upgrade || Upgrade->Cost > AvailableBudget)
				{
					return false;
				}

				AppliedUpgradeHistory.Add(UpgradeAssetId);
				if (AApartmentLifeApartmentUnit* Unit = GetApartmentUnit())
				{
					Unit->ApplyUpgrade(Upgrade);
				}
				return true;
			}
		}
	}
	return false;
}

void UApartmentLifeDecorationComponent::SetDecorationProfile(const FApartmentLifeDecorationProfile& InProfile)
{
	Profile = InProfile;
}

AApartmentLifeApartmentUnit* UApartmentLifeDecorationComponent::GetApartmentUnit() const
{
	return Cast<AApartmentLifeApartmentUnit>(GetOwner());
}

TArray<UApartmentLifeFurnitureItemData*> UApartmentLifeDecorationComponent::GetFurnitureCatalog() const
{
	TArray<UApartmentLifeFurnitureItemData*> Catalog;
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("FurnitureItem"))))
				{
					if (UApartmentLifeFurnitureItemData* Item = Cast<UApartmentLifeFurnitureItemData>(Asset))
					{
						Catalog.Add(Item);
					}
				}
			}
		}
	}
	return Catalog;
}

float UApartmentLifeDecorationComponent::GetRemainingFloorSpace() const
{
	if (AApartmentLifeApartmentUnit* Unit = GetApartmentUnit())
	{
		return Unit->GetRemainingFloorSpace();
	}
	return 0.f;
}

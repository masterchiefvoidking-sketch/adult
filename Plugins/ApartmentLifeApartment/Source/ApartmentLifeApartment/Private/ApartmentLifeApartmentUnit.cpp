// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeFurniturePlacementComponent.h"
#include "Components/SceneComponent.h"

AApartmentLifeApartmentUnit::AApartmentLifeApartmentUnit()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PlacementComponent = CreateDefaultSubobject<UApartmentLifeFurniturePlacementComponent>(TEXT("Placement"));
	PlacementComponent->SetupAttachment(RootComponent);
	ApartmentId = FName(TEXT("Apartment_Default"));
}

void AApartmentLifeApartmentUnit::PlaceFurniture(const FApartmentLifeFurniturePlacement& Placement)
{
	FApartmentLifeFurniturePlacement NewPlacement = Placement;
	if (!NewPlacement.PlacementGuid.IsValid())
	{
		NewPlacement.PlacementGuid = FGuid::NewGuid();
	}

	FurnitureLayout.Add(NewPlacement);
	if (PlacementComponent)
	{
		PlacementComponent->SpawnFurnitureInstance(NewPlacement);
	}
}

bool AApartmentLifeApartmentUnit::RemoveFurniture(const FGuid& PlacementGuid)
{
	const int32 Removed = FurnitureLayout.RemoveAll([&PlacementGuid](const FApartmentLifeFurniturePlacement& Item)
	{
		return Item.PlacementGuid == PlacementGuid;
	});

	if (Removed > 0 && PlacementComponent)
	{
		PlacementComponent->DestroyFurnitureInstance(PlacementGuid);
		return true;
	}
	return false;
}

FString AApartmentLifeApartmentUnit::GetSaveId_Implementation() const
{
	return FString::Printf(TEXT("apartment_%s"), *ApartmentId.ToString());
}

void AApartmentLifeApartmentUnit::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("FurnitureCount"), FString::FromInt(FurnitureLayout.Num()));
}

void AApartmentLifeApartmentUnit::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	for (const FApartmentLifeFurniturePlacement& Placement : FurnitureLayout)
	{
		if (PlacementComponent)
		{
			PlacementComponent->SpawnFurnitureInstance(Placement);
		}
	}
}

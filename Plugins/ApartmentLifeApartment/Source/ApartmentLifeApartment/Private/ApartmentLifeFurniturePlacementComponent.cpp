// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeFurniturePlacementComponent.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "Engine/World.h"

AActor* UApartmentLifeFurniturePlacementComponent::SpawnFurnitureInstance(const FApartmentLifeFurniturePlacement& Placement)
{
	UApartmentLifeFurnitureItemData* Data = ResolveFurnitureData(Placement.FurnitureItemId);
	if (!Data || !Data->Mesh || !GetWorld())
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* FurnitureActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Placement.Transform, Params);
	if (!FurnitureActor)
	{
		return nullptr;
	}

	SpawnedFurniture.Add(Placement.PlacementGuid, FurnitureActor);
	return FurnitureActor;
}

void UApartmentLifeFurniturePlacementComponent::DestroyFurnitureInstance(const FGuid& PlacementGuid)
{
	if (TObjectPtr<AActor>* Found = SpawnedFurniture.Find(PlacementGuid))
	{
		if (AActor* Actor = Found->Get())
		{
			Actor->Destroy();
		}
		SpawnedFurniture.Remove(PlacementGuid);
	}
}

UApartmentLifeFurnitureItemData* UApartmentLifeFurniturePlacementComponent::ResolveFurnitureData(FName FurnitureItemId) const
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				return Cast<UApartmentLifeFurnitureItemData>(Registry->FindAsset(FurnitureItemId));
			}
		}
	}
	return nullptr;
}

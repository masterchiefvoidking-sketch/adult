// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeFurniturePlacementComponent.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "Engine/World.h"

AApartmentLifeFurnitureActor* UApartmentLifeFurniturePlacementComponent::SpawnFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance)
{
	UApartmentLifeFurnitureItemData* Data = ResolveFurnitureData(Instance.FurnitureItemId);
	if (!GetWorld())
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AApartmentLifeFurnitureActor* FurnitureActor = GetWorld()->SpawnActor<AApartmentLifeFurnitureActor>(
		AApartmentLifeFurnitureActor::StaticClass(), Instance.Transform, Params);

	if (!FurnitureActor)
	{
		return nullptr;
	}

	FurnitureActor->InitializeFromInstance(Instance, Data);
	SpawnedFurniture.Add(Instance.InstanceId, FurnitureActor);
	return FurnitureActor;
}

void UApartmentLifeFurniturePlacementComponent::UpdateFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance)
{
	if (TObjectPtr<AApartmentLifeFurnitureActor>* Found = SpawnedFurniture.Find(Instance.InstanceId))
	{
		if (AApartmentLifeFurnitureActor* Actor = Found->Get())
		{
			Actor->SetActorTransform(Instance.Transform);
			UApartmentLifeFurnitureItemData* Data = ResolveFurnitureData(Instance.FurnitureItemId);
			Actor->InitializeFromInstance(Instance, Data);
		}
	}
	else
	{
		SpawnFurnitureInstance(Instance);
	}
}

void UApartmentLifeFurniturePlacementComponent::DestroyFurnitureInstance(const FGuid& InstanceId)
{
	if (TObjectPtr<AApartmentLifeFurnitureActor>* Found = SpawnedFurniture.Find(InstanceId))
	{
		if (AApartmentLifeFurnitureActor* Actor = Found->Get())
		{
			Actor->Destroy();
		}
		SpawnedFurniture.Remove(InstanceId);
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

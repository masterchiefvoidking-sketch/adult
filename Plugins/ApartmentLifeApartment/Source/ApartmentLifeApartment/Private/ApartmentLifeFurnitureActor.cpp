// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "ApartmentLifeFurnitureData.h"
#include "Components/StaticMeshComponent.h"

AApartmentLifeFurnitureActor::AApartmentLifeFurnitureActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

void AApartmentLifeFurnitureActor::InitializeFromInstance(const FApartmentLifePlacedFurnitureInstance& Instance, UApartmentLifeFurnitureItemData* ItemData)
{
	InstanceId = Instance.InstanceId;
	SetActorTransform(Instance.Transform);

	if (ItemData)
	{
		if (ItemData->Mesh)
		{
			MeshComponent->SetStaticMesh(ItemData->Mesh);
		}
		InteractionPoints = ItemData->InteractionPoints;
		if (InteractionPoints.Num() == 0)
		{
			for (EApartmentLifeFurnitureInteraction Interaction : UApartmentLifeBuilderLibrary::GetInteractionsForCategory(ItemData->Category))
			{
				FApartmentLifeInteractionPoint Point;
				Point.InteractionType = Interaction;
				Point.ActivityId = FName(*FString::Printf(TEXT("activity.%s"), *UEnum::GetValueAsString(Interaction)));
				InteractionPoints.Add(Point);
			}
		}
	}
}

bool AApartmentLifeFurnitureActor::TryInteract(EApartmentLifeFurnitureInteraction Interaction, FName& OutActivityId) const
{
	for (const FApartmentLifeInteractionPoint& Point : InteractionPoints)
	{
		if (Point.InteractionType == Interaction)
		{
			OutActivityId = Point.ActivityId;
			return !OutActivityId.IsNone();
		}
	}
	return false;
}

TArray<EApartmentLifeFurnitureInteraction> AApartmentLifeFurnitureActor::GetAvailableInteractions() const
{
	TArray<EApartmentLifeFurnitureInteraction> Result;
	for (const FApartmentLifeInteractionPoint& Point : InteractionPoints)
	{
		Result.Add(Point.InteractionType);
	}
	return Result;
}

TArray<EApartmentLifeFurnitureInteraction> AApartmentLifeFurnitureActor::GetAvailableInteractions_Implementation() const
{
	return GetAvailableInteractions();
}

bool AApartmentLifeFurnitureActor::ExecuteInteraction_Implementation(AActor* Interactor, EApartmentLifeFurnitureInteraction Interaction, FName& OutActivityId)
{
	return TryInteract(Interaction, OutActivityId);
}

FText AApartmentLifeFurnitureActor::GetInteractionPrompt_Implementation(EApartmentLifeFurnitureInteraction Interaction) const
{
	return UApartmentLifeBuilderLibrary::GetDisplayNameForInteraction(Interaction);
}

void AApartmentLifeFurnitureActor::SetInteractionsForCategory(EApartmentLifeFurnitureCategory Category)
{
	InteractionPoints.Reset();
	for (EApartmentLifeFurnitureInteraction Interaction : UApartmentLifeBuilderLibrary::GetInteractionsForCategory(Category))
	{
		FApartmentLifeInteractionPoint Point;
		Point.InteractionType = Interaction;
		Point.ActivityId = UApartmentLifeBuilderLibrary::GetActivityIdForInteraction(Interaction);
		InteractionPoints.Add(Point);
	}
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeInteractable.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeInteractionLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeActivityLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AActor* UApartmentLifeInteractionSelectionComponent::TraceInteractable(APlayerController* PlayerController, float TraceDistance) const
{
	if (!PlayerController)
	{
		return nullptr;
	}

	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector End = CamLoc + CamRot.Vector() * TraceDistance;
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionSelectTrace), true, GetOwner());
	if (!GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, End, ECC_Visibility, Params))
	{
		return nullptr;
	}

	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return nullptr;
	}

	if (HitActor->Implements<UApartmentLifeInteractable>() || HitActor->IsA<AApartmentLifeFurnitureActor>())
	{
		return HitActor;
	}

	return nullptr;
}

TArray<FApartmentLifeInteractionDescriptor> UApartmentLifeInteractionSelectionComponent::BuildDescriptorsForActor(AActor* Target) const
{
	TArray<FApartmentLifeInteractionDescriptor> Descriptors;
	if (!Target)
	{
		return Descriptors;
	}

	TArray<EApartmentLifeFurnitureInteraction> Interactions;
	if (Target->Implements<UApartmentLifeInteractable>())
	{
		Interactions = IApartmentLifeInteractable::Execute_GetAvailableInteractions(Target);
	}
	else if (AApartmentLifeFurnitureActor* Furniture = Cast<AApartmentLifeFurnitureActor>(Target))
	{
		Interactions = Furniture->GetAvailableInteractions();
	}

	for (EApartmentLifeFurnitureInteraction Interaction : Interactions)
	{
		Descriptors.Add(UApartmentLifeInteractionLibrary::BuildDescriptor(Interaction));
	}

	return Descriptors;
}

bool UApartmentLifeInteractionSelectionComponent::SelectFromView(APlayerController* PlayerController, float TraceDistance)
{
	AActor* HitActor = TraceInteractable(PlayerController, TraceDistance);
	if (!HitActor)
	{
		ClearSelection();
		return false;
	}

	SelectInteractable(HitActor);
	return true;
}

void UApartmentLifeInteractionSelectionComponent::SelectInteractable(AActor* Target)
{
	if (!Target)
	{
		ClearSelection();
		return;
	}

	SelectedTarget = Target;
	AvailableActions = BuildDescriptorsForActor(Target);
	OnInteractableSelected.Broadcast(Target, AvailableActions);
}

void UApartmentLifeInteractionSelectionComponent::ClearSelection()
{
	SelectedTarget = nullptr;
	AvailableActions.Empty();
	OnInteractableCleared.Broadcast();
}

bool UApartmentLifeInteractionSelectionComponent::ExecuteInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	AActor* Target = SelectedTarget.Get();
	if (!Target)
	{
		return false;
	}

	FName ActivityId;
	bool bSuccess = false;

	if (Target->Implements<UApartmentLifeInteractable>())
	{
		bSuccess = IApartmentLifeInteractable::Execute_ExecuteInteraction(Target, GetOwner(), Interaction, ActivityId);
	}
	else if (AApartmentLifeFurnitureActor* Furniture = Cast<AApartmentLifeFurnitureActor>(Target))
	{
		bSuccess = Furniture->TryInteract(Interaction, ActivityId);
	}

	if (!bSuccess)
	{
		return false;
	}

	if (ActivityId.IsNone())
	{
		ActivityId = UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(Interaction);
	}

	return StartActivityOnOwner(ActivityId);
}

bool UApartmentLifeInteractionSelectionComponent::ExecuteInteractionByIndex(int32 ActionIndex)
{
	if (!AvailableActions.IsValidIndex(ActionIndex))
	{
		return false;
	}

	const FApartmentLifeInteractionDescriptor& Descriptor = AvailableActions[ActionIndex];
	return ExecuteInteraction(Descriptor.InteractionType);
}

bool UApartmentLifeInteractionSelectionComponent::StartActivityOnOwner(FName ActivityId)
{
	if (ActivityId.IsNone() || !GetOwner())
	{
		return false;
	}

	if (UApartmentLifeActivityComponent* Activity = GetOwner()->FindComponentByClass<UApartmentLifeActivityComponent>())
	{
		const EApartmentLifeRoomType PreferredRoom = UApartmentLifeActivityLibrary::GetPreferredRoomForActivity(ActivityId);
		return Activity->StartActivity(ActivityId, PreferredRoom);
	}
	return false;
}

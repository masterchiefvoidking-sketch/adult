// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInteractionComponent.h"
#include "ApartmentLifeInteractable.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeInteractionLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

bool UApartmentLifeInteractionComponent::TryInteractWithActor(AActor* Target, EApartmentLifeFurnitureInteraction Interaction)
{
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
		if (bSuccess && ActivityId.IsNone())
		{
			ActivityId = UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(Interaction);
		}
	}

	if (!bSuccess)
	{
		return false;
	}

	if (ActivityId.IsNone())
	{
		ActivityId = UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(Interaction);
	}

	FocusedTarget = Target;
	OnInteractionPerformed.Broadcast(Target, Interaction);
	return StartActivityOnOwner(ActivityId);
}

bool UApartmentLifeInteractionComponent::TryInteractFromView(APlayerController* PlayerController, float TraceDistance)
{
	if (!PlayerController)
	{
		return false;
	}

	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector End = CamLoc + CamRot.Vector() * TraceDistance;
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), true, GetOwner());
	if (!GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, End, ECC_Visibility, Params))
	{
		return false;
	}

	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return false;
	}

	TArray<EApartmentLifeFurnitureInteraction> Interactions;
	if (HitActor->Implements<UApartmentLifeInteractable>())
	{
		Interactions = IApartmentLifeInteractable::Execute_GetAvailableInteractions(HitActor);
	}
	else if (AApartmentLifeFurnitureActor* Furniture = Cast<AApartmentLifeFurnitureActor>(HitActor))
	{
		Interactions = Furniture->GetAvailableInteractions();
	}

	if (Interactions.Num() == 0)
	{
		return false;
	}

	const EApartmentLifeFurnitureInteraction Primary = UApartmentLifeInteractionLibrary::GetPrimaryInteraction(Interactions);
	return TryInteractWithActor(HitActor, Primary);
}

bool UApartmentLifeInteractionComponent::StartActivityOnOwner(FName ActivityId)
{
	if (ActivityId.IsNone() || !GetOwner())
	{
		return false;
	}

	if (UApartmentLifeActivityComponent* Activity = GetOwner()->FindComponentByClass<UApartmentLifeActivityComponent>())
	{
		return Activity->StartActivity(ActivityId);
	}
	return false;
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInteractionHudComponent.h"
#include "ApartmentLifeUiOverlayGate.h"
#include "Engine/Engine.h"

void UApartmentLifeInteractionHudComponent::BindSelectionComponent(UApartmentLifeInteractionSelectionComponent* SelectionComponent)
{
	if (BoundSelection.IsValid())
	{
		BoundSelection->OnInteractableSelected.RemoveDynamic(this, &UApartmentLifeInteractionHudComponent::HandleInteractableSelected);
		BoundSelection->OnInteractableCleared.RemoveDynamic(this, &UApartmentLifeInteractionHudComponent::HandleInteractableCleared);
	}

	BoundSelection = SelectionComponent;
	if (SelectionComponent)
	{
		SelectionComponent->OnInteractableSelected.AddDynamic(this, &UApartmentLifeInteractionHudComponent::HandleInteractableSelected);
		SelectionComponent->OnInteractableCleared.AddDynamic(this, &UApartmentLifeInteractionHudComponent::HandleInteractableCleared);
	}
}

void UApartmentLifeInteractionHudComponent::RefreshOverlay() const
{
	if (!BoundSelection.IsValid())
	{
		return;
	}

	if (AActor* Target = BoundSelection->GetSelectedInteractable())
	{
		HandleInteractableSelected(Target, BoundSelection->GetAvailableActions());
	}
}

void UApartmentLifeInteractionHudComponent::HandleInteractableSelected(AActor* Target, const TArray<FApartmentLifeInteractionDescriptor>& Actions)
{
	if (FApartmentLifeUiOverlayGate::bSuppressDebugOverlays || !GEngine || !Target)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(7001, 5.f, FColor::Green, FString::Printf(TEXT("Selected: %s"), *Target->GetName()));

	for (int32 Index = 0; Index < Actions.Num(); ++Index)
	{
		const FApartmentLifeInteractionDescriptor& Action = Actions[Index];
		GEngine->AddOnScreenDebugMessage(
			7010 + Index,
			5.f,
			FColor::White,
			FString::Printf(TEXT("  [%d] %s — %s (%d min)"),
				Index + 1,
				*Action.DisplayName.ToString(),
				*Action.StatEffectSummary.ToString(),
				Action.DurationMinutes));
	}
}

void UApartmentLifeInteractionHudComponent::HandleInteractableCleared()
{
	if (FApartmentLifeUiOverlayGate::bSuppressDebugOverlays || !GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(7001, 0.1f, FColor::Silver, TEXT("Selection cleared"));
}

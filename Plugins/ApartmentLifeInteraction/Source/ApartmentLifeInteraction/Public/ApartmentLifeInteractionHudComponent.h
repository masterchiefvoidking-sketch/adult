// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeInteractionHudComponent.generated.h"

/** Lightweight on-screen action list for selected interactables (no UMG asset required). */
UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEINTERACTION_API UApartmentLifeInteractionHudComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void BindSelectionComponent(UApartmentLifeInteractionSelectionComponent* SelectionComponent);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	void RefreshOverlay() const;

protected:
	UFUNCTION()
	void HandleInteractableSelected(AActor* Target, const TArray<FApartmentLifeInteractionDescriptor>& Actions);

	UFUNCTION()
	void HandleInteractableCleared();

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeInteractionSelectionComponent> BoundSelection;
};

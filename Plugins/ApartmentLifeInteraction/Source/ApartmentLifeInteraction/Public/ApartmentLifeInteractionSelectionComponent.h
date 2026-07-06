// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInteractionTypes.h"
#include "ApartmentLifeInteractionSelectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableSelected, AActor*, Target, const TArray<FApartmentLifeInteractionDescriptor>&, Actions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableCleared);

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEINTERACTION_API UApartmentLifeInteractionSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	bool SelectFromView(APlayerController* PlayerController, float TraceDistance = 800.f);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	void SelectInteractable(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	bool ExecuteInteraction(EApartmentLifeFurnitureInteraction Interaction);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	bool ExecuteInteractionByIndex(int32 ActionIndex);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Interaction")
	AActor* GetSelectedInteractable() const { return SelectedTarget.Get(); }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Interaction")
	const TArray<FApartmentLifeInteractionDescriptor>& GetAvailableActions() const { return AvailableActions; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Interaction")
	FOnInteractableSelected OnInteractableSelected;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Interaction")
	FOnInteractableCleared OnInteractableCleared;

protected:
	AActor* TraceInteractable(APlayerController* PlayerController, float TraceDistance) const;
	TArray<FApartmentLifeInteractionDescriptor> BuildDescriptorsForActor(AActor* Target) const;
	bool StartActivityOnOwner(FName ActivityId);

	UPROPERTY()
	TWeakObjectPtr<AActor> SelectedTarget;

	UPROPERTY()
	TArray<FApartmentLifeInteractionDescriptor> AvailableActions;
};

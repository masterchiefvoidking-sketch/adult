// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionPerformed, AActor*, Target, EApartmentLifeFurnitureInteraction, Interaction);

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEINTERACTION_API UApartmentLifeInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	bool TryInteractWithActor(AActor* Target, EApartmentLifeFurnitureInteraction Interaction = EApartmentLifeFurnitureInteraction::Sit);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Interaction")
	bool TryInteractFromView(APlayerController* PlayerController, float TraceDistance = 500.f);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Interaction")
	AActor* GetFocusedInteractable() const { return FocusedTarget.Get(); }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Interaction")
	FOnInteractionPerformed OnInteractionPerformed;

protected:
	bool StartActivityOnOwner(FName ActivityId);

	UPROPERTY()
	TWeakObjectPtr<AActor> FocusedTarget;
};

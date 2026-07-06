// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInteractable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UApartmentLifeInteractable : public UInterface
{
	GENERATED_BODY()
};

class APARTMENTLIFEAPARTMENT_API IApartmentLifeInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Interaction")
	TArray<EApartmentLifeFurnitureInteraction> GetAvailableInteractions() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Interaction")
	bool ExecuteInteraction(AActor* Interactor, EApartmentLifeFurnitureInteraction Interaction, FName& OutActivityId);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Interaction")
	FText GetInteractionPrompt(EApartmentLifeFurnitureInteraction Interaction) const;
};

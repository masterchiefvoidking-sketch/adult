// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInteractionLibrary.generated.h"

UCLASS()
class APARTMENTLIFEINTERACTION_API UApartmentLifeInteractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Interaction")
	static FName GetDefaultActivityForInteraction(EApartmentLifeFurnitureInteraction Interaction);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Interaction")
	static EApartmentLifeFurnitureInteraction GetPrimaryInteraction(const TArray<EApartmentLifeFurnitureInteraction>& Interactions);
};

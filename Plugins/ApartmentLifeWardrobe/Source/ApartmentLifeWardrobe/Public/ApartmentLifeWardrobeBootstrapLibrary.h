// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeWardrobeBootstrapLibrary.generated.h"

class UApartmentLifeWardrobeComponent;

/** Seeds the first vertical slice wardrobe without requiring clothing data assets. */
UCLASS()
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeBootstrapLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	static void SeedVerticalSliceWardrobe(UApartmentLifeWardrobeComponent* Wardrobe);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	static TArray<FName> GetVerticalSliceClothingIds();
};

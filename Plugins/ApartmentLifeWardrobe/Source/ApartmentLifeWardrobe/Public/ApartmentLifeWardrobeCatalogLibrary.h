// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeCatalogLibrary.generated.h"

UCLASS()
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Catalog")
	static TArray<FApartmentLifeBuiltinClothingItem> GetBuiltinCatalog();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Catalog")
	static bool TryGetBuiltinItem(FName ItemId, FApartmentLifeBuiltinClothingItem& OutItem);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Catalog")
	static EApartmentLifeClothingLayer SlotToLayer(EApartmentLifeClothingSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Catalog")
	static EApartmentLifeClothingIncomeTier GetIncomeTierForSavings(float Savings);
};

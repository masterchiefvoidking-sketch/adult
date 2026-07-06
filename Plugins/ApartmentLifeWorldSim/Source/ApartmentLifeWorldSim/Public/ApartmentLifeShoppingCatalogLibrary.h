// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeShoppingCatalogLibrary.generated.h"

UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeShoppingCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Shopping")
	static TArray<FApartmentLifeBuiltinShopItem> GetBuiltinCatalog();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Shopping")
	static bool TryGetShopItem(FName ItemId, FApartmentLifeBuiltinShopItem& OutItem);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Shopping")
	static TArray<FApartmentLifeBuiltinShopItem> GetItemsForCategory(EApartmentLifeShopCategory Category);
};

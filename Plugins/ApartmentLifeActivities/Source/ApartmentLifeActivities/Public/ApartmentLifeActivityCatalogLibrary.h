// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeActivityTypes.h"
#include "ApartmentLifeActivityCatalogLibrary.generated.h"

UCLASS()
class APARTMENTLIFEACTIVITIES_API UApartmentLifeActivityCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity|Catalog")
	static TArray<FApartmentLifeActivityDefinition> GetBuiltinCatalog();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity|Catalog")
	static bool TryGetActivityDefinition(FName ActivityId, FApartmentLifeActivityDefinition& OutDefinition);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity|Catalog")
	static TArray<FApartmentLifeRoutineChainDefinition> GetBuiltinRoutineChains();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity|Catalog")
	static bool TryGetRoutineChain(FName ChainId, FApartmentLifeRoutineChainDefinition& OutChain);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity|Catalog")
	static TArray<FApartmentLifeActivityDefinition> GetActivitiesForRoom(EApartmentLifeActivityRoom Room);
};

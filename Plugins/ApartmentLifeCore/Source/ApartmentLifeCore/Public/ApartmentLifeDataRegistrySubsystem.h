// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeDataRegistrySubsystem.generated.h"

/**
 * Runtime lookup table for data assets loaded from Primary Asset bundles.
 * Supports hot-swapping content packs without code changes.
 */
UCLASS()
class APARTMENTLIFECORE_API UApartmentLifeDataRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Data")
	void RegisterAsset(UApartmentLifePrimaryDataAsset* Asset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Data")
	void UnregisterAsset(FName AssetId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Data")
	UApartmentLifePrimaryDataAsset* FindAsset(FName AssetId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Data")
	TArray<UApartmentLifePrimaryDataAsset*> GetAssetsByType(FName AssetType) const;

protected:
	UPROPERTY()
	TMap<FName, UApartmentLifePrimaryDataAsset*> AssetMap;

	UPROPERTY()
	TMap<FName, TArray<UApartmentLifePrimaryDataAsset*>> AssetsByType;
};

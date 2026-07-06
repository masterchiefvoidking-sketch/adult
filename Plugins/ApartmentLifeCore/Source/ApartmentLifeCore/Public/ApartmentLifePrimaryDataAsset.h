// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ApartmentLifePrimaryDataAsset.generated.h"

/**
 * Base class for all gameplay data assets. Provides stable IDs for save compatibility
 * and registry lookups. Subclass per feature domain — never embed gameplay logic in assets.
 */
UCLASS(Abstract, BlueprintType)
class APARTMENTLIFECORE_API UApartmentLifePrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName AssetId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity", meta = (MultiLine = true))
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	UTexture2D* Icon = nullptr;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(GetPrimaryAssetType(), AssetId);
	}

	virtual FName GetPrimaryAssetType() const
	{
		return FName(TEXT("ApartmentLife"));
	}
};

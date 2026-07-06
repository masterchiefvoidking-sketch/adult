// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeClothingItemData.generated.h"

USTRUCT(BlueprintType)
struct FApartmentLifeClothingFitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fit")
	float MinShoulderWidth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fit")
	float MaxShoulderWidth = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fit")
	float SleeveAdjustMin = -2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fit")
	float SleeveAdjustMax = 2.f;
};

/** Data asset for a single clothing item with fit, physics, and style metadata. */
UCLASS(BlueprintType)
class APARTMENTLIFEWARDROBE_API UApartmentLifeClothingItemData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ClothingItem")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	EApartmentLifeClothingCategory Category = EApartmentLifeClothingCategory::Tops;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	EApartmentLifeClothingLayer Layer = EApartmentLifeClothingLayer::Tops;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	USkeletalMesh* ClothingMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	bool bEnableClothPhysics = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	bool bEnableHairCollision = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	float Price = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<FName> StyleTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<FName> ColorVariantIds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<FName> MaterialVariantIds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	FApartmentLifeClothingFitData FitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<EApartmentLifeSeason> SeasonalTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<EApartmentLifeOccasion> SuitableOccasions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<EApartmentLifeOutfitContext> SuitableContexts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	float WarmthRating = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	bool bYogaCompatible = false;
};

UCLASS(BlueprintType)
class APARTMENTLIFEWARDROBE_API UApartmentLifeOutfitPresetData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("OutfitPreset")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Outfit")
	TArray<FApartmentLifeEquippedClothingSlot> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Outfit")
	EApartmentLifeOutfitContext Context = EApartmentLifeOutfitContext::Everyday;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Outfit")
	EApartmentLifeOccasion DefaultOccasion = EApartmentLifeOccasion::Everyday;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeClothingItemData.generated.h"

/** Data asset for a single clothing item. Meshes, physics, and seasonal tags are authored here. */
UCLASS(BlueprintType)
class APARTMENTLIFEWARDROBE_API UApartmentLifeClothingItemData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ClothingItem")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	EApartmentLifeClothingCategory Category = EApartmentLifeClothingCategory::Casual;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	EApartmentLifeClothingLayer Layer = EApartmentLifeClothingLayer::Base;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	USkeletalMesh* ClothingMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	bool bEnableClothPhysics = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<EApartmentLifeSeason> SeasonalTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	TArray<EApartmentLifeOccasion> SuitableOccasions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	float WarmthRating = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothing")
	bool bRequiresMatureContent = false;
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
	EApartmentLifeOccasion DefaultOccasion = EApartmentLifeOccasion::Everyday;
};

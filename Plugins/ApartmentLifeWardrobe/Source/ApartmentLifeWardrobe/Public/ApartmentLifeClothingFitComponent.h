// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeClothingFitComponent.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeClothingItemData;

/** Applies body-driven clothing fit offsets to equipped meshes. */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeClothingFitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void RefreshClothingFit(const FApartmentLifeBodyFitProfile& BodyFit);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool DoesClothingFit(const UApartmentLifeClothingItemData* Item, const FApartmentLifeBodyFitProfile& BodyFit) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool DoesBuiltinClothingFit(FName ItemId, const FApartmentLifeBodyFitProfile& BodyFit) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	float GetComputedSleeveOffset() const { return ComputedSleeveOffset; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	float GetComputedPantOffset() const { return ComputedPantOffset; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	FVector GetComputedScaleAdjustment() const { return ComputedScaleAdjustment; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ValidateEquippedFit(UApartmentLifeWardrobeComponent* Wardrobe, const FApartmentLifeBodyFitProfile& BodyFit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fit")
	float ComputedSleeveOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fit")
	float ComputedPantOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fit")
	FVector ComputedScaleAdjustment = FVector::OneVector;
};

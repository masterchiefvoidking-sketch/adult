// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeComponent.generated.h"

class UApartmentLifeClothingItemData;
class UApartmentLifeOutfitPresetData;
class UApartmentLifeDataRegistrySubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWardrobeUpdated);

/**
 * Manages equipped clothing, closet inventory, laundry state, and automatic outfit selection
 * based on weather, schedule, and occasion.
 */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UApartmentLifeWardrobeComponent();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void EquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void UnequipLayer(EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ApplyOutfitPreset(UApartmentLifeOutfitPresetData* Preset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SelectOutfitForContext(EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FApartmentLifeEquippedClothingSlot>& GetEquippedSlots() const { return EquippedSlots; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void MarkLayerDirty(EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void StartLaundryCycle();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Wardrobe")
	FOnWardrobeUpdated OnWardrobeUpdated;

	// IApartmentLifeSaveable
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FApartmentLifeEquippedClothingSlot> EquippedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FName> ClosetInventory;

	void RefreshVisuals();
	UApartmentLifeClothingItemData* ResolveClothingData(FName ClothingItemId) const;
	float ScoreClothingForContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather) const;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeComponent.generated.h"

class UApartmentLifeClothingItemData;
class UApartmentLifeOutfitPresetData;
class UApartmentLifeNPCSimulationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWardrobeUpdated);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UApartmentLifeWardrobeComponent();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool EquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void UnequipLayer(EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ApplyOutfitPreset(UApartmentLifeOutfitPresetData* Preset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ApplyFavoriteOutfit(FName OutfitNameId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool ApplyOutfitPresetByType(EApartmentLifeOutfitPresetType PresetType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool PreviewEquipClothing(FName ClothingItemId, EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SaveCurrentAsFavorite(FName OutfitNameId, EApartmentLifeOutfitContext Context);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SaveOutfitPreset(FName OutfitNameId, const FText& DisplayName, EApartmentLifeOutfitContext Context, EApartmentLifeOutfitPresetType PresetType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool RenameOutfitPreset(FName OutfitNameId, const FText& NewDisplayName);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool DeleteOutfitPreset(FName OutfitNameId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SelectOutfitForContext(EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SelectOutfitForOutfitContext(EApartmentLifeOutfitContext Context, const FApartmentLifeWeatherState& Weather);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	bool PurchaseClothing(UApartmentLifeNPCSimulationComponent* Simulation, FName ClothingItemId, int32 DayPurchased = 0);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void AddOwnedClothing(FName ClothingItemId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool OwnsClothing(FName ClothingItemId) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ToggleFavoriteItem(FName ClothingItemId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool IsFavoriteItem(FName ClothingItemId) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SetPreviewOutfit(const TArray<FApartmentLifeEquippedClothingSlot>& PreviewSlots);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ApplyPreviewOutfit();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void ClearPreviewOutfit();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool ComparePreviewWithEquipped() const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void MarkLayerDirty(EApartmentLifeClothingLayer Layer);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void MarkEquippedWorn();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void StartLaundryCycle();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void AdvanceLaundryCycle();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SetStyleProfile(const FApartmentLifeWardrobeStyleProfile& Profile);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const FApartmentLifeWardrobeStyleProfile& GetStyleProfile() const { return StyleProfile; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void SetClosetInventory(const TArray<FName>& ItemIds);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FName>& GetClosetInventory() const { return ClosetInventory; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FApartmentLifeEquippedClothingSlot>& GetEquippedSlots() const { return EquippedSlots; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FApartmentLifeFavoriteOutfit>& GetFavoriteOutfits() const { return FavoriteOutfits; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FName>& GetFavoriteItemIds() const { return FavoriteItemIds; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FApartmentLifeShoppingRecord>& GetShoppingHistory() const { return ShoppingHistory; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	const TArray<FApartmentLifeEquippedClothingSlot>& GetPreviewSlots() const { return PreviewSlots; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Wardrobe")
	FOnWardrobeUpdated OnWardrobeUpdated;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void RefreshVisuals();
	void ResolveLayerConflicts(EApartmentLifeClothingLayer Layer);
	UApartmentLifeClothingItemData* ResolveClothingData(FName ClothingItemId) const;
	float ScoreBuiltinItem(const FApartmentLifeBuiltinClothingItem& Item, EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather) const;
	float ScoreBuiltinItemForContext(const FApartmentLifeBuiltinClothingItem& Item, EApartmentLifeOutfitContext Context, const FApartmentLifeWeatherState& Weather) const;
	float ScoreClothingForContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOccasion Occasion, const FApartmentLifeWeatherState& Weather) const;
	float ScoreClothingForOutfitContext(const UApartmentLifeClothingItemData* Item, EApartmentLifeOutfitContext Context, const FApartmentLifeWeatherState& Weather) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FApartmentLifeEquippedClothingSlot> EquippedSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FApartmentLifeEquippedClothingSlot> PreviewSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FApartmentLifeFavoriteOutfit> FavoriteOutfits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FName> ClosetInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FName> FavoriteItemIds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
	TArray<FApartmentLifeShoppingRecord> ShoppingHistory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wardrobe")
	FApartmentLifeWardrobeStyleProfile StyleProfile;
};

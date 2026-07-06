// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeUiController.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeNPCSimulationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWardrobeUiStateChanged);

/** Lightweight wardrobe UI state + on-screen overlay (no UMG asset required). */
UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeUiController : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		UApartmentLifeWardrobeComponent* InWardrobe,
		UApartmentLifeNPCSimulationComponent* InSimulation);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void OpenWardrobe();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void CloseWardrobe();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void ToggleWardrobe();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	bool IsWardrobeOpen() const { return bIsOpen; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetCategoryTab(EApartmentLifeWardrobeCategoryTab Tab);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetSlotFilter(EApartmentLifeClothingLayer Layer, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetSearchText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetSortMode(EApartmentLifeClothingSortMode SortMode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetOwnedOnly(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetFavoritesOnly(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SetShoppingMode(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	bool IsShoppingMode() const { return bShoppingMode; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SelectItemIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void PreviewSelectedItem();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void EquipSelectedItem();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void UnequipSelectedLayer();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	bool PurchaseSelectedItem();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void ToggleFavoriteSelectedItem();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void ApplyPreviewToEquipped();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void ClearPreview();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	bool IsPreviewDifferentFromEquipped() const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void SaveCurrentOutfitPreset(FName PresetId, const FText& DisplayName, EApartmentLifeOutfitPresetType PresetType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	bool RenameOutfitPreset(FName PresetId, const FText& NewDisplayName);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	bool DeleteOutfitPreset(FName PresetId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void ApplyOutfitPresetByType(EApartmentLifeOutfitPresetType PresetType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void RecommendOutfitForActivity(FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void RefreshItemList();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|UI")
	void RefreshOverlay() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	const TArray<FName>& GetVisibleItemIds() const { return VisibleItemIds; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	FName GetSelectedItemId() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|UI")
	const FApartmentLifeWardrobeFilterQuery& GetFilterQuery() const { return FilterQuery; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Wardrobe|UI")
	FOnWardrobeUiStateChanged OnWardrobeUiStateChanged;

protected:
	void BroadcastStateChanged();
	EApartmentLifeOutfitContext PresetTypeToContext(EApartmentLifeOutfitPresetType PresetType) const;
	EApartmentLifeClothingLayer ResolveLayerForItem(FName ItemId) const;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeWardrobeComponent> Wardrobe;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeNPCSimulationComponent> Simulation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe|UI")
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe|UI")
	bool bShoppingMode = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe|UI")
	FApartmentLifeWardrobeFilterQuery FilterQuery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe|UI")
	TArray<FName> VisibleItemIds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe|UI")
	int32 SelectedItemIndex = INDEX_NONE;
};

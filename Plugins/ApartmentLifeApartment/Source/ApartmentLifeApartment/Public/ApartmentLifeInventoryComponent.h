// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FGuid, EntryId);

/**
 * Apartment inventory: owned, placed, stored, sold, gifted, broken, dirty, upgraded items.
 * Supports filter/search/sort via Blueprint or UI layer.
 */
UCLASS(ClassGroup = Inventory, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeInventoryComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Inventory")
	FGuid AddItem(FName FurnitureItemId, EApartmentLifeInventoryItemState InitialState = EApartmentLifeInventoryItemState::Owned);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Inventory")
	bool RemoveItem(const FGuid& EntryId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Inventory")
	bool SetItemState(const FGuid& EntryId, EApartmentLifeInventoryItemState NewState);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Inventory")
	bool LinkToPlacement(const FGuid& EntryId, const FGuid& PlacementId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Inventory")
	TArray<FApartmentLifeInventoryEntry> GetItemsByState(EApartmentLifeInventoryItemState State) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Inventory")
	TArray<FApartmentLifeInventoryEntry> GetItemsByCategory(EApartmentLifeFurnitureCategory Category) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Inventory")
	TArray<FApartmentLifeInventoryEntry> SearchItems(const FString& Query) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Inventory")
	const TArray<FApartmentLifeInventoryEntry>& GetAllItems() const { return Items; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Inventory")
	FOnInventoryChanged OnInventoryChanged;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FApartmentLifeInventoryEntry> Items;

	FApartmentLifeInventoryEntry* FindEntry(const FGuid& EntryId);
};

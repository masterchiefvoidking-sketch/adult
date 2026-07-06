// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeSaveGame.h"
#include "ApartmentLifeSaveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeSaveCompleted, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeLoadCompleted, bool, bSuccess);

/**
 * Aggregates IApartmentLifeSaveable objects across the world and persists to disk slots.
 */
UCLASS()
class APARTMENTLIFECORE_API UApartmentLifeSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool SaveToSlot(int32 SlotIndex, const FString& SlotName = TEXT("Save"));

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool LoadFromSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool DoesSaveExist(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void CollectSaveables(UApartmentLifeSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void ApplySaveables(const UApartmentLifeSaveGame* SaveGame);

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Save")
	FOnApartmentLifeSaveCompleted OnSaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Save")
	FOnApartmentLifeLoadCompleted OnLoadCompleted;

protected:
	FString BuildSlotName(int32 SlotIndex) const;
	UWorld* GetActiveWorld() const;
};

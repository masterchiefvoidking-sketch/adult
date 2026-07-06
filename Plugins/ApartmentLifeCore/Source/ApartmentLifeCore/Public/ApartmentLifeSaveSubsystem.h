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
	bool SaveToSlot(int32 SlotIndex, const FString& SlotName = TEXT("Save"), bool bAutosave = false);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool LoadFromSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool DoesSaveExist(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool DeleteSaveSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool GetSlotMetadata(int32 SlotIndex, FApartmentLifeSaveSlotMetadata& OutMetadata, int32& OutSaveVersion) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void RequestAutosave(int32 SlotIndex = 0, FName Reason = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void SetAutosaveEnabled(bool bEnabled) { bAutosaveEnabled = bEnabled; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Save")
	bool IsAutosaveEnabled() const { return bAutosaveEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void CollectSaveables(UApartmentLifeSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void ApplySaveables(const UApartmentLifeSaveGame* SaveGame);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	void ApplyPostLoadRefresh();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Save")
	bool MigrateSaveGame(UApartmentLifeSaveGame* SaveGame) const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Save")
	FOnApartmentLifeSaveCompleted OnSaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Save")
	FOnApartmentLifeLoadCompleted OnLoadCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment Life|Save")
	float AutosaveCooldownSeconds = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment Life|Save")
	int32 DefaultAutosaveSlot = 0;

	bool bAutosaveEnabled = true;

protected:
	FString BuildSlotName(int32 SlotIndex) const;
	UWorld* GetActiveWorld() const;
	void PopulateMetadata(UApartmentLifeSaveGame* SaveGame, const FString& SlotName, bool bAutosave) const;
	bool CanLoadSaveVersion(int32 Version) const;
	double LastAutosaveWorldTime = -1000.0;
};

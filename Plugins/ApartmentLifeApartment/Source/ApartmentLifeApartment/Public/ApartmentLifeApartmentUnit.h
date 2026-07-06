// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeFurnitureData.h"
#include "ApartmentLifeApartmentUnit.generated.h"

class UApartmentLifeFurniturePlacementComponent;
class UApartmentLifeInventoryComponent;
class UApartmentLifeBuildModeComponent;
class UApartmentLifeCleaningComponent;
class UApartmentLifeDecorationComponent;
class UApartmentLifeApartmentLayoutData;
class UApartmentLifeApartmentUpgradeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomScoresUpdated, EApartmentLifeRoomType, Room);

/**
 * Root actor for a customizable apartment. Integrates layout, inventory, build mode,
 * cleaning, decoration AI, and room scoring.
 */
UCLASS()
class APARTMENTLIFEAPARTMENT_API AApartmentLifeApartmentUnit : public AActor, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	AApartmentLifeApartmentUnit();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void InitializeFromLayout(UApartmentLifeApartmentLayoutData* LayoutData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool PlaceFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool UpdateFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool RemoveFurnitureInstance(const FGuid& InstanceId, bool bAddToInventory = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool GetFurnitureInstance(const FGuid& InstanceId, FApartmentLifePlacedFurnitureInstance& OutInstance) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	float SellFurnitureInstance(const FGuid& InstanceId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void ApplyUpgrade(const UApartmentLifeApartmentUpgradeData* Upgrade);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void RecalculateRoomScores();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	FApartmentLifeRoomScore GetRoomScore(EApartmentLifeRoomType Room) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	float GetRemainingFloorSpace() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	const FApartmentLifeApartmentMetadata& GetMetadata() const { return Metadata; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	const TArray<FApartmentLifePlacedFurnitureInstance>& GetPlacedFurniture() const { return PlacedFurniture; }

	// Legacy API
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void PlaceFurniture(const FApartmentLifeFurniturePlacement& Placement);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool RemoveFurniture(const FGuid& PlacementGuid);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	UApartmentLifeInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	UApartmentLifeBuildModeComponent* GetBuildModeComponent() const { return BuildModeComponent; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Apartment")
	FOnRoomScoresUpdated OnRoomScoresUpdated;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHourAdvanced(const struct FApartmentLifeGameTime& NewTime);

	UApartmentLifeFurnitureItemData* ResolveFurnitureData(FName ItemId) const;
	TArray<UApartmentLifeFurnitureItemData*> ResolveAllPlacedItems() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeFurniturePlacementComponent> PlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeBuildModeComponent> BuildModeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeCleaningComponent> CleaningComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeDecorationComponent> DecorationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TArray<FApartmentLifePlacedFurnitureInstance> PlacedFurniture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TMap<EApartmentLifeRoomType, FApartmentLifeRoomScore> RoomScores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	FName ApartmentId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	FApartmentLifeApartmentMetadata Metadata;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	TArray<EApartmentLifeStyleTag> PreferredStyles;

	UPROPERTY()
	TObjectPtr<UApartmentLifeApartmentLayoutData> LayoutData;

	float UsedFloorSpace = 0.f;
};

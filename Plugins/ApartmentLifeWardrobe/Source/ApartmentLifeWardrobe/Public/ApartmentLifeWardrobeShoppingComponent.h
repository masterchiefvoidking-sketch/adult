// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeShoppingComponent.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeNPCSimulationComponent;

/** Local-only shopping browse/purchase wrapper for the wardrobe catalog. */
UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeShoppingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		UApartmentLifeWardrobeComponent* InWardrobe,
		UApartmentLifeNPCSimulationComponent* InSimulation);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|Shopping")
	TArray<FName> GetBrowseResults(const FApartmentLifeWardrobeFilterQuery& Query) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|Shopping")
	TArray<FName> GetAffordableRecommendations() const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|Shopping")
	bool TryPurchase(FName ItemId, int32 DayPurchased = 0);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Shopping")
	bool CanAfford(FName ItemId) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|Shopping")
	void PreviewItem(FName ItemId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe|Shopping")
	bool EquipAfterPurchase(FName ItemId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe|Shopping")
	EApartmentLifeClothingIncomeTier GetCurrentIncomeTier() const;

protected:
	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeWardrobeComponent> Wardrobe;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeNPCSimulationComponent> Simulation;
};

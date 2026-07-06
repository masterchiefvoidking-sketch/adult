// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeFinanceUiController.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeProgressionComponent;

UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWORLDSIM_API UApartmentLifeFinanceUiController : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		UApartmentLifeNPCSimulationComponent* InSimulation,
		UApartmentLifeProgressionComponent* InProgression);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance|UI")
	void ToggleBudgetOverlay();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance|UI")
	void ToggleShoppingOverlay(EApartmentLifeShopCategory Category);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance|UI")
	bool PurchaseShopItemIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance|UI")
	void RefreshOverlay() const;

protected:
	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeNPCSimulationComponent> Simulation;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeProgressionComponent> Progression;

	bool bBudgetOpen = false;
	bool bShoppingOpen = false;
	EApartmentLifeShopCategory ShoppingCategory = EApartmentLifeShopCategory::Furniture;
	TArray<FApartmentLifeBuiltinShopItem> VisibleShopItems;
};

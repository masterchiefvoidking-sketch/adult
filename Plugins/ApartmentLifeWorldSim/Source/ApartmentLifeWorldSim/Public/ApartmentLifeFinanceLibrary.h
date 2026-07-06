// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeFinanceLibrary.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeProgressionComponent;

UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeFinanceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance")
	static void RecordIncome(UApartmentLifeProgressionComponent* Progression, float Amount);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance")
	static void RecordSpending(
		UApartmentLifeProgressionComponent* Progression,
		EApartmentLifeShopCategory Category,
		float Amount);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Finance")
	static FApartmentLifeDailyBudget GetCurrentBudget(const UApartmentLifeProgressionComponent* Progression);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Finance")
	static void AdvanceDailyBudget(UApartmentLifeProgressionComponent* Progression, const UApartmentLifeNPCSimulationComponent* Simulation);
};

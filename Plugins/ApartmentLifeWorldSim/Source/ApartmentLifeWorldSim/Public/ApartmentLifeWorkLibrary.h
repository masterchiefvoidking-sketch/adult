// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeWorkLibrary.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeProgressionComponent;

UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeWorkLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work")
	static TArray<FApartmentLifeWorkTypeDefinition> GetBuiltinWorkTypes();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work")
	static bool TryGetWorkType(FName WorkTypeId, FApartmentLifeWorkTypeDefinition& OutDefinition);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work")
	static bool TryGetWorkTypeForActivity(FName ActivityId, FApartmentLifeWorkTypeDefinition& OutDefinition);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work")
	static bool IsComputerWorkActivity(FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work")
	static FApartmentLifeWorkSessionResult ComputeWorkSessionResult(
		UApartmentLifeNPCSimulationComponent* Simulation,
		UApartmentLifeProgressionComponent* Progression,
		FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work")
	static void ApplyWorkSessionResult(
		UApartmentLifeNPCSimulationComponent* Simulation,
		UApartmentLifeProgressionComponent* Progression,
		const FApartmentLifeWorkSessionResult& Result);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work")
	static TArray<FApartmentLifeWorkTypeDefinition> GetAvailableWorkTypes(
		const UApartmentLifeNPCSimulationComponent* Simulation,
		const UApartmentLifeProgressionComponent* Progression);
};

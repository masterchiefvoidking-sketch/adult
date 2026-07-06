// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeUnlockLibrary.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeProgressionComponent;

UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeUnlockLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Unlocks")
	static TArray<FName> EvaluateUnlocks(
		UApartmentLifeNPCSimulationComponent* Simulation,
		UApartmentLifeProgressionComponent* Progression);
};

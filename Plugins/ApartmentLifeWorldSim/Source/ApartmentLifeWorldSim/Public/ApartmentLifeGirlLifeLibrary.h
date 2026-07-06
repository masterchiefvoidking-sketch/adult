// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeGirlLifeLibrary.generated.h"

class UApartmentLifeNPCSimulationComponent;

/** Builtin activity durations when no data assets exist (private single-character build). */
UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeGirlLifeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Girl Life")
	static int32 GetBuiltinActivityDurationMinutes(FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Girl Life")
	static void ApplyActivitySkillGain(FApartmentLifeSkillSet& Skills, FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Girl Life")
	static void ApplyActivityCompletion(UApartmentLifeNPCSimulationComponent* Simulation, FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Girl Life")
	static float ComputeComputerWorkPayout(const UApartmentLifeNPCSimulationComponent* Simulation, FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Girl Life")
	static void ConfigureRemoteWorkCareer(UApartmentLifeNPCSimulationComponent* Simulation, FName WorkTypeId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Girl Life")
	static bool IsComputerWorkActivity(FName ActivityId);
};

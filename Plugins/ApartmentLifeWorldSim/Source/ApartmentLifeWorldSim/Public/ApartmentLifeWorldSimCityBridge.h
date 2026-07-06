// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeCityTypes.h"
#include "ApartmentLifeWorldSimCityBridge.generated.h"

UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeWorldSimCityBridge : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City Bridge")
	static float ComputeNPCCommuteMinutes(const FApartmentLifeCareerState& Career, EApartmentLifeTransportMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City Bridge")
	static float ComputeDistrictRentForNPC(FName DistrictId, const FApartmentLifeCityEconomyState& Economy);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City Bridge")
	static void ApplyActivitySkillGain(FApartmentLifeSkillSet& Skills, FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City Bridge")
	static EApartmentLifeSimulationLOD GetSimulationLODForDistance(float DistanceMeters, bool bPlayerDistrict);
};

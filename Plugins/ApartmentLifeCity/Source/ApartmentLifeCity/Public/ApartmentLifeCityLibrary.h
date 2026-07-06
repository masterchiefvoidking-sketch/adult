// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCityTypes.h"
#include "ApartmentLifeCityDataAssets.h"
#include "ApartmentLifeCityLibrary.generated.h"

UCLASS()
class APARTMENTLIFECITY_API UApartmentLifeCityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	static EApartmentLifeDayPhase GetDayPhaseForHour(int32 Hour);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	static bool IsBusinessOpen(const FApartmentLifeBuildingHours& Hours, int32 Hour, EApartmentLifeDayPhase Phase);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static FApartmentLifeTravelPlan ComputeTravelPlan(
		FName OriginDistrictId,
		FName DestinationDistrictId,
		EApartmentLifeTransportMode PreferredMode,
		const UApartmentLifeTransitRouteData* Route);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static float ComputeCommuteMinutes(FName HomeDistrictId, FName WorkDistrictId, EApartmentLifeTransportMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static float ComputeDistrictRent(const FApartmentLifeDistrictStats& District, const FApartmentLifeCityEconomyState& Economy);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static EApartmentLifeSimulationLOD ComputeSimulationLOD(float DistanceToPlayerMeters, bool bIsPlayerDistrict);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static TArray<FName> GetAvailableItemsAtBuilding(const UApartmentLifeCityBuildingData* Building, int32 Hour);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static float ApplyWeatherToTraffic(float BaseTraffic, EApartmentLifeWeather Weather);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	static void TickCityEconomy(FApartmentLifeCityEconomyState& Economy, EApartmentLifeSeason Season, int32 DayOfMonth);
};

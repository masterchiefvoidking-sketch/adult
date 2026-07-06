// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCityTypes.h"
#include "ApartmentLifeCitySubsystem.generated.h"

class UApartmentLifeDistrictData;
class UApartmentLifeCityBuildingData;
class UApartmentLifeCityPOIData;
class UApartmentLifeCityEventData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCityEventStarted, const FApartmentLifeActiveCityEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDistrictSimulationUpdated, FName, DistrictId);

UCLASS()
class APARTMENTLIFECITY_API UApartmentLifeCitySubsystem : public UWorldSubsystem, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void RegisterDistrict(UApartmentLifeDistrictData* District);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void RegisterBuilding(UApartmentLifeCityBuildingData* Building);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void RegisterPOI(UApartmentLifeCityPOIData* POI);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void SetPlayerDistrict(FName DistrictId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void UpdateNPCLocation(FName CharacterId, FName DistrictId, FName POIId, float DistanceToPlayer);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	const FApartmentLifeCityEconomyState& GetEconomy() const { return Economy; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	FName GetPlayerDistrictId() const { return PlayerDistrictId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	TArray<FName> GetBuildingsInDistrict(FName DistrictId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|City")
	TArray<FName> GetOpenBuildingsAtHour(FName DistrictId, int32 Hour) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|City")
	void SimulateBackgroundDistricts();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|City")
	FOnCityEventStarted OnCityEventStarted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|City")
	FOnDistrictSimulationUpdated OnDistrictSimulationUpdated;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UFUNCTION()
	void HandleHourAdvanced(const struct FApartmentLifeGameTime& NewTime);

	UFUNCTION()
	void HandleDayAdvanced(const struct FApartmentLifeGameTime& NewTime);

	void EvaluateCityEvents(const FApartmentLifeGameTime& Time);
	void UpdateEconomy(const FApartmentLifeGameTime& Time);
	void UpdateDistrictTraffic(const FApartmentLifeGameTime& Time);

	UPROPERTY()
	TMap<FName, TObjectPtr<UApartmentLifeDistrictData>> Districts;

	UPROPERTY()
	TMap<FName, TObjectPtr<UApartmentLifeCityBuildingData>> Buildings;

	UPROPERTY()
	TMap<FName, TObjectPtr<UApartmentLifeCityPOIData>> POIs;

	UPROPERTY()
	TArray<UApartmentLifeCityEventData*> CityEvents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "City")
	FApartmentLifeCityEconomyState Economy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "City")
	TArray<FApartmentLifeActiveCityEvent> ActiveEvents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "City")
	TMap<FName, FApartmentLifeNPCLocationState> NPCLocations;

	FName PlayerDistrictId;
	bool bTimeBound = false;
};

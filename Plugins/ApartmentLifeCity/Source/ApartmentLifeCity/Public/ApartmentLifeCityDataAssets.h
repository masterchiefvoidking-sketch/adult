// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeCityTypes.h"
#include "ApartmentLifeCityDataAssets.generated.h"

UCLASS(BlueprintType)
class APARTMENTLIFECITY_API UApartmentLifeDistrictData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("District")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
	EApartmentLifeCityDistrict DistrictType = EApartmentLifeCityDistrict::Residential;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
	FApartmentLifeDistrictStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
	TArray<FName> BuildingIds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
	float WeatherTemperatureBias = 0.f;
};

UCLASS(BlueprintType)
class APARTMENTLIFECITY_API UApartmentLifeCityBuildingData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("CityBuilding")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	EApartmentLifeBuildingType BuildingType = EApartmentLifeBuildingType::ApartmentBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FName DistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FApartmentLifeBuildingHours Hours;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	int32 EmployeeCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	int32 MaxCustomers = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FApartmentLifeStoreInventory Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	TArray<FName> ApartmentUnitIds;
};

UCLASS(BlueprintType)
class APARTMENTLIFECITY_API UApartmentLifeCityPOIData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("CityPOI")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "POI")
	FName BuildingId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "POI")
	FName DistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "POI")
	EApartmentLifeBuildingType POIType = EApartmentLifeBuildingType::Cafe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "POI")
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "POI")
	FName LinkedActivityId;
};

UCLASS(BlueprintType)
class APARTMENTLIFECITY_API UApartmentLifeCityEventData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("CityEvent")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	EApartmentLifeCityEventType EventType = EApartmentLifeCityEventType::HolidayFestival;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	TArray<EApartmentLifeSeason> ValidSeasons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	bool bRequiresWeekend = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	int32 DurationHours = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float MoodBoost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float EconomyBoost = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float SaleDiscount = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	FName LinkedActivityId;
};

UCLASS(BlueprintType)
class APARTMENTLIFECITY_API UApartmentLifeTransitRouteData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("TransitRoute")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transit")
	FName FromDistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transit")
	FName ToDistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transit")
	EApartmentLifeTransportMode Mode = EApartmentLifeTransportMode::PublicTransit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transit")
	float TravelMinutes = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transit")
	float Cost = 2.5f;
};

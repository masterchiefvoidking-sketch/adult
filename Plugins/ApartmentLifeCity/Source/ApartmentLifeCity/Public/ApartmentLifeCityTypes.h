// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCityTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeCityDistrict : uint8
{
	Downtown			UMETA(DisplayName = "Downtown"),
	Residential			UMETA(DisplayName = "Residential"),
	LuxuryDistrict		UMETA(DisplayName = "Luxury District"),
	UniversityArea		UMETA(DisplayName = "University Area"),
	ShoppingDistrict	UMETA(DisplayName = "Shopping District"),
	BusinessDistrict	UMETA(DisplayName = "Business District"),
	Waterfront			UMETA(DisplayName = "Waterfront"),
	EntertainmentDistrict UMETA(DisplayName = "Entertainment District"),
	Parks				UMETA(DisplayName = "Parks"),
	Suburbs				UMETA(DisplayName = "Suburbs")
};

UENUM(BlueprintType)
enum class EApartmentLifeBuildingType : uint8
{
	ApartmentBuilding	UMETA(DisplayName = "Apartment Building"),
	GroceryStore		UMETA(DisplayName = "Grocery Store"),
	ClothingStore		UMETA(DisplayName = "Clothing Store"),
	FurnitureStore		UMETA(DisplayName = "Furniture Store"),
	ElectronicsStore	UMETA(DisplayName = "Electronics Store"),
	Cafe				UMETA(DisplayName = "Cafe"),
	Restaurant			UMETA(DisplayName = "Restaurant"),
	Bakery				UMETA(DisplayName = "Bakery"),
	Library				UMETA(DisplayName = "Library"),
	Bookstore			UMETA(DisplayName = "Bookstore"),
	Gym					UMETA(DisplayName = "Gym"),
	YogaStudio			UMETA(DisplayName = "Yoga Studio"),
	Park				UMETA(DisplayName = "Park"),
	Office				UMETA(DisplayName = "Office"),
	University			UMETA(DisplayName = "University"),
	Hospital			UMETA(DisplayName = "Hospital"),
	Salon				UMETA(DisplayName = "Salon"),
	ConvenienceStore	UMETA(DisplayName = "Convenience Store")
};

UENUM(BlueprintType)
enum class EApartmentLifeTransportMode : uint8
{
	Walking			UMETA(DisplayName = "Walking"),
	Bicycle			UMETA(DisplayName = "Bicycle"),
	PublicTransit	UMETA(DisplayName = "Public Transit"),
	RideShare		UMETA(DisplayName = "Ride Share"),
	PersonalCar		UMETA(DisplayName = "Personal Car")
};

UENUM(BlueprintType)
enum class EApartmentLifeDayPhase : uint8
{
	MorningRush		UMETA(DisplayName = "Morning Rush"),
	BusinessHours	UMETA(DisplayName = "Business Hours"),
	Afternoon		UMETA(DisplayName = "Afternoon"),
	Evening			UMETA(DisplayName = "Evening"),
	LateNight		UMETA(DisplayName = "Late Night")
};

UENUM(BlueprintType)
enum class EApartmentLifeCityEventType : uint8
{
	HolidayFestival		UMETA(DisplayName = "Holiday Festival"),
	FarmersMarket		UMETA(DisplayName = "Farmers Market"),
	Concert				UMETA(DisplayName = "Concert"),
	CommunityFair		UMETA(DisplayName = "Community Fair"),
	ApartmentMeeting	UMETA(DisplayName = "Apartment Meeting"),
	SeasonalCelebration UMETA(DisplayName = "Seasonal Celebration"),
	SportingEvent		UMETA(DisplayName = "Sporting Event"),
	NeighborhoodCleanup UMETA(DisplayName = "Neighborhood Cleanup"),
	SaleEvent			UMETA(DisplayName = "Sale Event")
};

UENUM(BlueprintType)
enum class EApartmentLifeSimulationLOD : uint8
{
	Full		UMETA(DisplayName = "Full"),
	Reduced		UMETA(DisplayName = "Reduced"),
	Background	UMETA(DisplayName = "Background"),
	Dormant		UMETA(DisplayName = "Dormant")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeDistrictStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	EApartmentLifeCityDistrict DistrictType = EApartmentLifeCityDistrict::Residential;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	int32 Population = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	float AverageIncome = 3500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District", meta = (ClampMin = "0", ClampMax = "100"))
	float CrimeLevel = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	float AverageRent = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	float PropertyValueIndex = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District", meta = (ClampMin = "0", ClampMax = "100"))
	float TrafficDensity = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District", meta = (ClampMin = "0", ClampMax = "100"))
	float NoiseLevel = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	bool bHasPublicTransit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "District")
	TArray<FName> LocalEventIds;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeBuildingHours
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hours")
	int32 OpenHour = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hours")
	int32 CloseHour = 21;

	APARTMENTLIFECITY_API bool IsOpenAtHour(int32 Hour) const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeStoreInventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FName> ItemIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, int32> StockLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float SaleDiscount = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeCityEconomyState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float EmploymentRate = 0.92f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float HousingDemand = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float AverageSalary = 3200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float RentFluctuation = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float SeasonalDemandMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	TArray<FName> ActiveSaleEvents;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeTravelPlan
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
	FName OriginPOIId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
	FName DestinationPOIId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
	EApartmentLifeTransportMode Mode = EApartmentLifeTransportMode::Walking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
	float TravelMinutes = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Travel")
	float TravelCost = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeActiveCityEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FName EventId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	EApartmentLifeCityEventType EventType = EApartmentLifeCityEventType::HolidayFestival;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FName DistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FApartmentLifeGameTime StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	int32 DurationHours = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	bool bIsActive = false;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECITY_API FApartmentLifeNPCLocationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FName CurrentDistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FName CurrentPOIId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	EApartmentLifeSimulationLOD SimulationLOD = EApartmentLifeSimulationLOD::Full;
};

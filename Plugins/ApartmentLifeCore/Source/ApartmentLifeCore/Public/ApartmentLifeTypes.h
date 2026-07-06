// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.generated.h"

/** In-game calendar time. The world never waits for the player. */
USTRUCT(BlueprintType)
struct APARTMENTLIFECORE_API FApartmentLifeGameTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Year = 2026;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = "1", ClampMax = "12"))
	int32 Month = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = "1", ClampMax = "31"))
	int32 Day = 6;

	/** Hour of day [0, 23]. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = "0", ClampMax = "23"))
	int32 Hour = 8;

	/** Minute [0, 59]. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = "0", ClampMax = "59"))
	int32 Minute = 0;

	/** Total elapsed in-game minutes since campaign start. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int64 TotalMinutes = 0;

	bool operator==(const FApartmentLifeGameTime& Other) const;
	bool operator<(const FApartmentLifeGameTime& Other) const;

	APARTMENTLIFECORE_API void AddMinutes(int32 Minutes);
	APARTMENTLIFECORE_API float GetTimeOfDayNormalized() const;
	APARTMENTLIFECORE_API FString ToDisplayString() const;
};

UENUM(BlueprintType)
enum class EApartmentLifeSeason : uint8
{
	Spring	UMETA(DisplayName = "Spring"),
	Summer	UMETA(DisplayName = "Summer"),
	Autumn	UMETA(DisplayName = "Autumn"),
	Winter	UMETA(DisplayName = "Winter")
};

UENUM(BlueprintType)
enum class EApartmentLifeWeather : uint8
{
	Clear		UMETA(DisplayName = "Clear"),
	Cloudy		UMETA(DisplayName = "Cloudy"),
	Rain		UMETA(DisplayName = "Rain"),
	Storm		UMETA(DisplayName = "Storm"),
	Snow		UMETA(DisplayName = "Snow"),
	Heatwave	UMETA(DisplayName = "Heatwave")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECORE_API FApartmentLifeWeatherState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	EApartmentLifeWeather Weather = EApartmentLifeWeather::Clear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float TemperatureCelsius = 22.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	float Humidity = 0.45f;
};

UENUM(BlueprintType)
enum class EApartmentLifeRoomType : uint8
{
	LivingRoom	UMETA(DisplayName = "Living Room"),
	Kitchen		UMETA(DisplayName = "Kitchen"),
	DiningRoom	UMETA(DisplayName = "Dining Room"),
	Bedroom		UMETA(DisplayName = "Bedroom"),
	Bathroom	UMETA(DisplayName = "Bathroom"),
	WalkInCloset	UMETA(DisplayName = "Walk-In Closet"),
	LaundryRoom	UMETA(DisplayName = "Laundry Room"),
	Balcony		UMETA(DisplayName = "Balcony"),
	Office		UMETA(DisplayName = "Office")
};

UENUM(BlueprintType)
enum class EApartmentLifeOccasion : uint8
{
	Everyday	UMETA(DisplayName = "Everyday"),
	Work		UMETA(DisplayName = "Work"),
	FormalEvent	UMETA(DisplayName = "Formal Event"),
	Date		UMETA(DisplayName = "Date"),
	Athletic	UMETA(DisplayName = "Athletic"),
	Sleep		UMETA(DisplayName = "Sleep"),
	Relaxing	UMETA(DisplayName = "Relaxing")
};

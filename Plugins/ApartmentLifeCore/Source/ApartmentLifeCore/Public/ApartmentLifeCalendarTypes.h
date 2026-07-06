// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCalendarTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeWeekday : uint8
{
	Sunday		UMETA(DisplayName = "Sunday"),
	Monday		UMETA(DisplayName = "Monday"),
	Tuesday		UMETA(DisplayName = "Tuesday"),
	Wednesday	UMETA(DisplayName = "Wednesday"),
	Thursday	UMETA(DisplayName = "Thursday"),
	Friday		UMETA(DisplayName = "Friday"),
	Saturday	UMETA(DisplayName = "Saturday")
};

UENUM(BlueprintType)
enum class EApartmentLifeTimeAcceleration : uint8
{
	Paused	UMETA(DisplayName = "Paused"),
	Realtime1x	UMETA(DisplayName = "1x"),
	Speed2x		UMETA(DisplayName = "2x"),
	Speed4x		UMETA(DisplayName = "4x"),
	Speed8x		UMETA(DisplayName = "8x")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECORE_API FApartmentLifeHolidayDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	FName HolidayId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar", meta = (ClampMin = "1", ClampMax = "12"))
	int32 Month = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar", meta = (ClampMin = "1", ClampMax = "31"))
	int32 Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	bool bIsPublicHoliday = true;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECORE_API FApartmentLifeBirthdayRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar", meta = (ClampMin = "1", ClampMax = "12"))
	int32 BirthMonth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar", meta = (ClampMin = "1", ClampMax = "31"))
	int32 BirthDay = 1;
};

/** Pure calendar math utilities — no UObject dependencies. */
struct APARTMENTLIFECORE_API FApartmentLifeCalendarUtils
{
	static int32 GetDaysInMonth(int32 Year, int32 Month);
	static EApartmentLifeWeekday GetWeekday(int32 Year, int32 Month, int32 Day);
	static bool IsWeekend(int32 Year, int32 Month, int32 Day);
	static EApartmentLifeSeason GetSeasonForMonth(int32 Month);
	static bool IsSameCalendarDay(const FApartmentLifeGameTime& A, int32 Month, int32 Day);
	static void SyncCalendarFields(FApartmentLifeGameTime& Time, int32 EpochYear, int32 EpochMonth, int32 EpochDay, int32 EpochHour, int32 EpochMinute);
};

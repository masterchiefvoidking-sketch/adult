// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCalendarTypes.h"
#include "ApartmentLifeGameTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeMinuteAdvanced, const FApartmentLifeGameTime&, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeHourAdvanced, const FApartmentLifeGameTime&, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeDayAdvanced, const FApartmentLifeGameTime&, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnApartmentLifeSeasonChanged, EApartmentLifeSeason, OldSeason, EApartmentLifeSeason, NewSeason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeHolidayStarted, FName, HolidayId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeBirthdayTriggered, FName, CharacterId);

/**
 * Persistent world clock. Supports pause, 1x/2x/4x/8x acceleration, calendar, seasons,
 * holidays, and birthdays. NPC simulation subscribes to advancement delegates.
 */
UCLASS()
class APARTMENTLIFECORE_API UApartmentLifeGameTimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void SetTimeAcceleration(EApartmentLifeTimeAcceleration Acceleration);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void SetPaused(bool bInPaused);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	bool IsPaused() const { return bPaused; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	EApartmentLifeTimeAcceleration GetTimeAcceleration() const { return CurrentAcceleration; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	float GetTimeScale() const { return ActiveTimeScale; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	FApartmentLifeGameTime GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	FApartmentLifeWeatherState GetCurrentWeather() const { return CurrentWeather; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	EApartmentLifeSeason GetCurrentSeason() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	EApartmentLifeWeekday GetCurrentWeekday() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	bool IsWeekend() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	bool IsHolidayToday(FName& OutHolidayId) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void SetCurrentTime(const FApartmentLifeGameTime& NewTime);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void SetWeather(const FApartmentLifeWeatherState& NewWeather);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void RegisterHoliday(const FApartmentLifeHolidayDefinition& Holiday);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void RegisterBirthday(const FApartmentLifeBirthdayRecord& Birthday);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	const TArray<FApartmentLifeHolidayDefinition>& GetRegisteredHolidays() const { return Holidays; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	const TArray<FApartmentLifeBirthdayRecord>& GetRegisteredBirthdays() const { return Birthdays; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void RestoreFromSave(
		const FApartmentLifeGameTime& InTime,
		const FApartmentLifeWeatherState& InWeather,
		EApartmentLifeSeason InSeason,
		const TArray<FApartmentLifeHolidayDefinition>& InHolidays,
		const TArray<FApartmentLifeBirthdayRecord>& InBirthdays);

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeMinuteAdvanced OnMinuteAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeHourAdvanced OnHourAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeDayAdvanced OnDayAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeSeasonChanged OnSeasonChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeHolidayStarted OnHolidayStarted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeBirthdayTriggered OnBirthdayTriggered;

protected:
	void TickTime(float DeltaSeconds);
	void ProcessCalendarEvents(const FApartmentLifeGameTime& PreviousTime, const FApartmentLifeGameTime& NewTime);
	float ResolveTimeScale(EApartmentLifeTimeAcceleration Acceleration) const;
	void UpdateSeasonFromCalendar();
	void EvolveWeatherForSeason();

	UPROPERTY()
	FApartmentLifeGameTime CurrentTime;

	UPROPERTY()
	FApartmentLifeWeatherState CurrentWeather;

	UPROPERTY()
	TArray<FApartmentLifeHolidayDefinition> Holidays;

	UPROPERTY()
	TArray<FApartmentLifeBirthdayRecord> Birthdays;

	EApartmentLifeTimeAcceleration CurrentAcceleration = EApartmentLifeTimeAcceleration::Realtime1x;
	EApartmentLifeSeason CurrentSeason = EApartmentLifeSeason::Summer;
	float ActiveTimeScale = 1.f;
	bool bPaused = false;
	float AccumulatedRealSeconds = 0.f;
	int32 LastProcessedDay = -1;
	int32 LastProcessedHour = -1;
	FName LastHolidayBroadcast;

	FTimerHandle TickTimerHandle;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeGameTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeMinuteAdvanced, const FApartmentLifeGameTime&, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeDayAdvanced, const FApartmentLifeGameTime&, NewTime);

/**
 * Drives the living world clock. NPC schedules, bills, and activities advance even when
 * the player is idle. Time scale is data-driven per difficulty or debug settings.
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
	void SetTimeScale(float InTimeScale);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	float GetTimeScale() const { return TimeScale; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	FApartmentLifeGameTime GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Time")
	FApartmentLifeWeatherState GetCurrentWeather() const { return CurrentWeather; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void AdvanceMinutes(int32 Minutes);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Time")
	void SetWeather(const FApartmentLifeWeatherState& NewWeather);

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeMinuteAdvanced OnMinuteAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Time")
	FOnApartmentLifeDayAdvanced OnDayAdvanced;

protected:
	void TickTime(float DeltaSeconds);

	UPROPERTY()
	FApartmentLifeGameTime CurrentTime;

	UPROPERTY()
	FApartmentLifeWeatherState CurrentWeather;

	float TimeScale = 60.f;
	float AccumulatedRealSeconds = 0.f;
	int32 LastKnownDay = 0;

	FTimerHandle TickTimerHandle;
};

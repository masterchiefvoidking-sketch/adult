// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeGameTimeSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UApartmentLifeGameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentTime.Year = 2026;
	CurrentTime.Month = 7;
	CurrentTime.Day = 6;
	CurrentTime.Hour = 8;
	CurrentTime.Minute = 0;
	CurrentTime.TotalMinutes = 0;
	LastKnownDay = CurrentTime.Day;

	CurrentWeather.Weather = EApartmentLifeWeather::Clear;
	CurrentWeather.TemperatureCelsius = 24.f;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TickTimerHandle,
			[this]() { TickTime(0.25f); },
			0.25f,
			true);
	}
}

void UApartmentLifeGameTimeSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TickTimerHandle);
	}
	Super::Deinitialize();
}

void UApartmentLifeGameTimeSubsystem::SetTimeScale(float InTimeScale)
{
	TimeScale = FMath::Max(0.f, InTimeScale);
}

void UApartmentLifeGameTimeSubsystem::AdvanceMinutes(int32 Minutes)
{
	if (Minutes <= 0)
	{
		return;
	}

	const int32 PreviousDay = CurrentTime.Day;
	CurrentTime.AddMinutes(Minutes);
	OnMinuteAdvanced.Broadcast(CurrentTime);

	if (CurrentTime.Day != PreviousDay)
	{
		OnDayAdvanced.Broadcast(CurrentTime);
	}
}

void UApartmentLifeGameTimeSubsystem::SetWeather(const FApartmentLifeWeatherState& NewWeather)
{
	CurrentWeather = NewWeather;
}

void UApartmentLifeGameTimeSubsystem::TickTime(float DeltaSeconds)
{
	if (TimeScale <= 0.f)
	{
		return;
	}

	AccumulatedRealSeconds += DeltaSeconds;
	const float SecondsPerGameMinute = 60.f / TimeScale;

	while (AccumulatedRealSeconds >= SecondsPerGameMinute)
	{
		AccumulatedRealSeconds -= SecondsPerGameMinute;
		AdvanceMinutes(1);
	}
}

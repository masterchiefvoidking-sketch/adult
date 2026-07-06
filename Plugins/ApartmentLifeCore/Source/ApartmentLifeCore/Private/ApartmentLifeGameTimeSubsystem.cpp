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
	LastProcessedDay = CurrentTime.Day;
	LastProcessedHour = CurrentTime.Hour;
	LastHolidayBroadcast = NAME_None;

	CurrentWeather.Weather = EApartmentLifeWeather::Clear;
	CurrentWeather.TemperatureCelsius = 24.f;

	CurrentSeason = FApartmentLifeCalendarUtils::GetSeasonForMonth(CurrentTime.Month);
	SetTimeAcceleration(EApartmentLifeTimeAcceleration::Realtime1x);

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

void UApartmentLifeGameTimeSubsystem::SetTimeAcceleration(EApartmentLifeTimeAcceleration Acceleration)
{
	CurrentAcceleration = Acceleration;
	bPaused = (Acceleration == EApartmentLifeTimeAcceleration::Paused);
	ActiveTimeScale = ResolveTimeScale(Acceleration);
}

void UApartmentLifeGameTimeSubsystem::SetPaused(bool bInPaused)
{
	bPaused = bInPaused;
	if (bPaused)
	{
		CurrentAcceleration = EApartmentLifeTimeAcceleration::Paused;
		ActiveTimeScale = 0.f;
	}
	else if (CurrentAcceleration == EApartmentLifeTimeAcceleration::Paused)
	{
		SetTimeAcceleration(EApartmentLifeTimeAcceleration::Realtime1x);
	}
}

float UApartmentLifeGameTimeSubsystem::ResolveTimeScale(EApartmentLifeTimeAcceleration Acceleration) const
{
	switch (Acceleration)
	{
	case EApartmentLifeTimeAcceleration::Paused:
		return 0.f;
	case EApartmentLifeTimeAcceleration::Realtime1x:
		return 1.f;
	case EApartmentLifeTimeAcceleration::Speed2x:
		return 2.f;
	case EApartmentLifeTimeAcceleration::Speed4x:
		return 4.f;
	case EApartmentLifeTimeAcceleration::Speed8x:
		return 8.f;
	default:
		return 1.f;
	}
}

EApartmentLifeSeason UApartmentLifeGameTimeSubsystem::GetCurrentSeason() const
{
	return CurrentSeason;
}

EApartmentLifeWeekday UApartmentLifeGameTimeSubsystem::GetCurrentWeekday() const
{
	return FApartmentLifeCalendarUtils::GetWeekday(CurrentTime.Year, CurrentTime.Month, CurrentTime.Day);
}

bool UApartmentLifeGameTimeSubsystem::IsWeekend() const
{
	return FApartmentLifeCalendarUtils::IsWeekend(CurrentTime.Year, CurrentTime.Month, CurrentTime.Day);
}

bool UApartmentLifeGameTimeSubsystem::IsHolidayToday(FName& OutHolidayId) const
{
	for (const FApartmentLifeHolidayDefinition& Holiday : Holidays)
	{
		if (Holiday.Month == CurrentTime.Month && Holiday.Day == CurrentTime.Day)
		{
			OutHolidayId = Holiday.HolidayId;
			return true;
		}
	}
	return false;
}

void UApartmentLifeGameTimeSubsystem::SetCurrentTime(const FApartmentLifeGameTime& NewTime)
{
	CurrentTime = NewTime;
	FApartmentLifeCalendarUtils::SyncCalendarFields(CurrentTime, 2026, 7, 6, 8, 0);
	UpdateSeasonFromCalendar();
}

void UApartmentLifeGameTimeSubsystem::AdvanceMinutes(int32 Minutes)
{
	if (Minutes <= 0)
	{
		return;
	}

	const FApartmentLifeGameTime PreviousTime = CurrentTime;
	CurrentTime.AddMinutes(Minutes);
	ProcessCalendarEvents(PreviousTime, CurrentTime);
	OnMinuteAdvanced.Broadcast(CurrentTime);

	if (CurrentTime.Hour != PreviousTime.Hour)
	{
		OnHourAdvanced.Broadcast(CurrentTime);
	}

	if (CurrentTime.Day != PreviousTime.Day || CurrentTime.Month != PreviousTime.Month || CurrentTime.Year != PreviousTime.Year)
	{
		OnDayAdvanced.Broadcast(CurrentTime);
	}
}

void UApartmentLifeGameTimeSubsystem::SetWeather(const FApartmentLifeWeatherState& NewWeather)
{
	CurrentWeather = NewWeather;
}

void UApartmentLifeGameTimeSubsystem::RegisterHoliday(const FApartmentLifeHolidayDefinition& Holiday)
{
	Holidays.AddUnique(Holiday);
}

void UApartmentLifeGameTimeSubsystem::RegisterBirthday(const FApartmentLifeBirthdayRecord& Birthday)
{
	Birthdays.AddUnique(Birthday);
}

void UApartmentLifeGameTimeSubsystem::ProcessCalendarEvents(const FApartmentLifeGameTime& PreviousTime, const FApartmentLifeGameTime& NewTime)
{
	if (NewTime.Day != PreviousTime.Day || NewTime.Month != PreviousTime.Month)
	{
		UpdateSeasonFromCalendar();
		EvolveWeatherForSeason();

		FName HolidayId;
		if (IsHolidayToday(HolidayId) && HolidayId != LastHolidayBroadcast)
		{
			LastHolidayBroadcast = HolidayId;
			OnHolidayStarted.Broadcast(HolidayId);
		}
		else if (!IsHolidayToday(HolidayId))
		{
			LastHolidayBroadcast = NAME_None;
		}

		for (const FApartmentLifeBirthdayRecord& Birthday : Birthdays)
		{
			if (Birthday.BirthMonth == NewTime.Month && Birthday.BirthDay == NewTime.Day)
			{
				OnBirthdayTriggered.Broadcast(Birthday.CharacterId);
			}
		}
	}
}

void UApartmentLifeGameTimeSubsystem::UpdateSeasonFromCalendar()
{
	const EApartmentLifeSeason NewSeason = FApartmentLifeCalendarUtils::GetSeasonForMonth(CurrentTime.Month);
	if (NewSeason != CurrentSeason)
	{
		const EApartmentLifeSeason OldSeason = CurrentSeason;
		CurrentSeason = NewSeason;
		OnSeasonChanged.Broadcast(OldSeason, NewSeason);
	}
}

void UApartmentLifeGameTimeSubsystem::EvolveWeatherForSeason()
{
	switch (CurrentSeason)
	{
	case EApartmentLifeSeason::Winter:
		CurrentWeather.TemperatureCelsius = FMath::RandRange(0.f, 8.f);
		CurrentWeather.Weather = FMath::RandRange(0, 1) == 0 ? EApartmentLifeWeather::Snow : EApartmentLifeWeather::Cloudy;
		break;
	case EApartmentLifeSeason::Summer:
		CurrentWeather.TemperatureCelsius = FMath::RandRange(24.f, 36.f);
		CurrentWeather.Weather = FMath::RandRange(0, 4) == 0 ? EApartmentLifeWeather::Heatwave : EApartmentLifeWeather::Clear;
		break;
	case EApartmentLifeSeason::Spring:
		CurrentWeather.TemperatureCelsius = FMath::RandRange(12.f, 22.f);
		CurrentWeather.Weather = EApartmentLifeWeather::Rain;
		break;
	case EApartmentLifeSeason::Autumn:
	default:
		CurrentWeather.TemperatureCelsius = FMath::RandRange(8.f, 18.f);
		CurrentWeather.Weather = EApartmentLifeWeather::Cloudy;
		break;
	}
}

void UApartmentLifeGameTimeSubsystem::TickTime(float DeltaSeconds)
{
	if (bPaused || ActiveTimeScale <= 0.f)
	{
		return;
	}

	AccumulatedRealSeconds += DeltaSeconds;
	const float SecondsPerGameMinute = 60.f / ActiveTimeScale;

	while (AccumulatedRealSeconds >= SecondsPerGameMinute)
	{
		AccumulatedRealSeconds -= SecondsPerGameMinute;
		AdvanceMinutes(1);
	}
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeGameState.h"
#include "ApartmentLifeGameTimeSubsystem.h"

FApartmentLifeGameTime AApartmentLifeGameState::GetWorldTime() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			return TimeSubsystem->GetCurrentTime();
		}
	}
	return FApartmentLifeGameTime();
}

FApartmentLifeWeatherState AApartmentLifeGameState::GetWorldWeather() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			return TimeSubsystem->GetCurrentWeather();
		}
	}
	return FApartmentLifeWeatherState();
}

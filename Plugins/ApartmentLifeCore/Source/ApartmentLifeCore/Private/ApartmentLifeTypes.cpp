// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCalendarTypes.h"

bool FApartmentLifeGameTime::operator==(const FApartmentLifeGameTime& Other) const
{
	return TotalMinutes == Other.TotalMinutes;
}

bool FApartmentLifeGameTime::operator<(const FApartmentLifeGameTime& Other) const
{
	return TotalMinutes < Other.TotalMinutes;
}

void FApartmentLifeGameTime::AddMinutes(int32 Minutes)
{
	if (Minutes <= 0)
	{
		return;
	}

	TotalMinutes += Minutes;
	FApartmentLifeCalendarUtils::SyncCalendarFields(*this, 2026, 7, 6, 8, 0);
}

float FApartmentLifeGameTime::GetTimeOfDayNormalized() const
{
	return (static_cast<float>(Hour) + static_cast<float>(Minute) / 60.f) / 24.f;
}

FString FApartmentLifeGameTime::ToDisplayString() const
{
	return FString::Printf(TEXT("%04d-%02d-%02d %02d:%02d"), Year, Month, Day, Hour, Minute);
}

int32 FApartmentLifeGameTime::GetDayOfYear() const
{
	int32 DayOfYear = Day;
	for (int32 M = 1; M < Month; ++M)
	{
		DayOfYear += FApartmentLifeCalendarUtils::GetDaysInMonth(Year, M);
	}
	return DayOfYear;
}

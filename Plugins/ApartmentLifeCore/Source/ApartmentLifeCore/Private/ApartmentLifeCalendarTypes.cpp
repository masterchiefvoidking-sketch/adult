// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCalendarTypes.h"

int32 FApartmentLifeCalendarUtils::GetDaysInMonth(int32 Year, int32 Month)
{
	static const int32 DaysPerMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (Month < 1 || Month > 12)
	{
		return 30;
	}

	int32 Days = DaysPerMonth[Month - 1];
	const bool bLeapYear = (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0);
	if (Month == 2 && bLeapYear)
	{
		++Days;
	}
	return Days;
}

EApartmentLifeWeekday FApartmentLifeCalendarUtils::GetWeekday(int32 Year, int32 Month, int32 Day)
{
	if (Month < 3)
	{
		Month += 12;
		Year -= 1;
	}

	const int32 K = Year % 100;
	const int32 J = Year / 100;
	int32 H = (Day + ((13 * (Month + 1)) / 5) + K + (K / 4) + (J / 4) + (5 * J)) % 7;

	// Zeller: 0=Saturday; map to our enum where 0=Sunday
	const int32 SundayBased = (H + 6) % 7;
	return static_cast<EApartmentLifeWeekday>(SundayBased);
}

bool FApartmentLifeCalendarUtils::IsWeekend(int32 Year, int32 Month, int32 Day)
{
	const EApartmentLifeWeekday Weekday = GetWeekday(Year, Month, Day);
	return Weekday == EApartmentLifeWeekday::Saturday || Weekday == EApartmentLifeWeekday::Sunday;
}

EApartmentLifeSeason FApartmentLifeCalendarUtils::GetSeasonForMonth(int32 Month)
{
	switch (Month)
	{
	case 12:
	case 1:
	case 2:
		return EApartmentLifeSeason::Winter;
	case 3:
	case 4:
	case 5:
		return EApartmentLifeSeason::Spring;
	case 6:
	case 7:
	case 8:
		return EApartmentLifeSeason::Summer;
	default:
		return EApartmentLifeSeason::Autumn;
	}
}

bool FApartmentLifeCalendarUtils::IsSameCalendarDay(const FApartmentLifeGameTime& A, int32 Month, int32 Day)
{
	return A.Month == Month && A.Day == Day;
}

void FApartmentLifeCalendarUtils::SyncCalendarFields(FApartmentLifeGameTime& Time, int32 EpochYear, int32 EpochMonth, int32 EpochDay, int32 EpochHour, int32 EpochMinute)
{
	int64 Remaining = Time.TotalMinutes;

	int32 Year = EpochYear;
	int32 Month = EpochMonth;
	int32 Day = EpochDay;
	int32 Hour = EpochHour;
	int32 Minute = EpochMinute;

	Minute += Remaining % 60;
	Remaining /= 60;
	Hour += Remaining % 24;
	Remaining /= 24;

	while (Minute >= 60)
	{
		Minute -= 60;
		++Hour;
	}

	while (Hour >= 24)
	{
		Hour -= 24;
		++Day;
	}

	while (Day > GetDaysInMonth(Year, Month))
	{
		Day -= GetDaysInMonth(Year, Month);
		++Month;
		if (Month > 12)
		{
			Month = 1;
			++Year;
		}
	}

	while (Remaining > 0)
	{
		const int32 DaysInMonth = GetDaysInMonth(Year, Month);
		const int32 DaysLeftInMonth = DaysInMonth - Day + 1;

		if (Remaining >= DaysLeftInMonth)
		{
			Remaining -= DaysLeftInMonth;
			Day = 1;
			++Month;
			if (Month > 12)
			{
				Month = 1;
				++Year;
			}
		}
		else
		{
			Day += static_cast<int32>(Remaining);
			Remaining = 0;
		}
	}

	Time.Year = Year;
	Time.Month = Month;
	Time.Day = Day;
	Time.Hour = Hour;
	Time.Minute = Minute;
}

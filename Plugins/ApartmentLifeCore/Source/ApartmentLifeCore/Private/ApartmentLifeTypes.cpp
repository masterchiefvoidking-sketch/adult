// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeTypes.h"

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
	Minute += Minutes;

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

	while (Day > 28)
	{
		const int32 DaysInMonth = (Month == 2) ? 28 : ((Month == 4 || Month == 6 || Month == 9 || Month == 11) ? 30 : 31);
		if (Day > DaysInMonth)
		{
			Day -= DaysInMonth;
			++Month;
			if (Month > 12)
			{
				Month = 1;
				++Year;
			}
		}
		else
		{
			break;
		}
	}
}

float FApartmentLifeGameTime::GetTimeOfDayNormalized() const
{
	return (static_cast<float>(Hour) + static_cast<float>(Minute) / 60.f) / 24.f;
}

FString FApartmentLifeGameTime::ToDisplayString() const
{
	return FString::Printf(TEXT("%04d-%02d-%02d %02d:%02d"), Year, Month, Day, Hour, Minute);
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCityTypes.h"

bool FApartmentLifeBuildingHours::IsOpenAtHour(int32 Hour) const
{
	if (CloseHour > OpenHour)
	{
		return Hour >= OpenHour && Hour < CloseHour;
	}
	return Hour >= OpenHour || Hour < CloseHour;
}

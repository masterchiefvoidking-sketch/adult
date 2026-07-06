// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeContentSettings.h"

UApartmentLifeContentSettings::UApartmentLifeContentSettings()
{
	CategoryName = TEXT("Apartment Life");
}

const UApartmentLifeContentSettings* UApartmentLifeContentSettings::Get()
{
	return GetDefault<UApartmentLifeContentSettings>();
}

bool UApartmentLifeContentSettings::IsExplicitContentAllowed()
{
	const UApartmentLifeContentSettings* Settings = Get();
	return Settings && Settings->bEnableExplicitActivities;
}

bool UApartmentLifeContentSettings::IsNudityAllowed()
{
	const UApartmentLifeContentSettings* Settings = Get();
	return Settings && Settings->bEnableNudity;
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeApartmentTypes.h"

float FApartmentLifeRoomScore::GetOverallScore() const
{
	return (Comfort + Cleanliness + StyleMatch + Functionality + LightingQuality + EntertainmentValue) / 6.f - Clutter * 0.3f;
}

float FApartmentLifeRoomScore::GetMoodImpact() const
{
	return GetOverallScore() * 0.01f;
}

float FApartmentLifeRoomScore::GetSleepQualityImpact() const
{
	return (Comfort * 0.5f + Cleanliness * 0.2f + LightingQuality * 0.1f) / 100.f;
}

float FApartmentLifeRoomScore::GetProductivityImpact() const
{
	return (Functionality * 0.5f + LightingQuality * 0.3f - Clutter * 0.5f) / 100.f;
}

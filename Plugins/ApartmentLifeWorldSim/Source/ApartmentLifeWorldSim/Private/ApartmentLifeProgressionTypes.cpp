// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeProgressionTypes.h"

float FApartmentLifeComputerQualityState::GetProductivityBonus() const
{
	const float AvgTier = (ComputerTier + MonitorTier + DeskTier + ChairTier + InternetTier) / 5.f;
	return (AvgTier - 1.f) * 0.05f;
}

float FApartmentLifeComputerQualityState::GetStressReductionBonus() const
{
	return ChairTier * 0.02f + InternetTier * 0.01f;
}

float FApartmentLifeComputerQualityState::GetSkillXpBonus() const
{
	return ComputerTier * 0.03f + MonitorTier * 0.02f;
}

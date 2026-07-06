// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorldSimCityBridge.h"
#include "ApartmentLifeCityLibrary.h"

float UApartmentLifeWorldSimCityBridge::ComputeNPCCommuteMinutes(const FApartmentLifeCareerState& Career, EApartmentLifeTransportMode Mode)
{
	return UApartmentLifeCityLibrary::ComputeCommuteMinutes(Career.HomeDistrictId, Career.WorkDistrictId, Mode);
}

float UApartmentLifeWorldSimCityBridge::ComputeDistrictRentForNPC(FName DistrictId, const FApartmentLifeCityEconomyState& Economy)
{
	FApartmentLifeDistrictStats Stats;
	Stats.AverageRent = 900.f;
	return UApartmentLifeCityLibrary::ComputeDistrictRent(Stats, Economy);
}

void UApartmentLifeWorldSimCityBridge::ApplyActivitySkillGain(FApartmentLifeSkillSet& Skills, FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("cook"))) Skills.GainSkill(EApartmentLifeSkill::Cooking, 0.5f);
	if (Id.Contains(TEXT("exercise")) || Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("gym"))) Skills.GainSkill(EApartmentLifeSkill::Fitness, 0.5f);
	if (Id.Contains(TEXT("clean"))) Skills.GainSkill(EApartmentLifeSkill::HomeMaintenance, 0.3f);
	if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("office"))) Skills.GainSkill(EApartmentLifeSkill::Organization, 0.2f);
	if (Id.Contains(TEXT("social")) || Id.Contains(TEXT("talk"))) Skills.GainSkill(EApartmentLifeSkill::Communication, 0.3f);
	if (Id.Contains(TEXT("game"))) Skills.GainSkill(EApartmentLifeSkill::Creativity, 0.2f);
	if (Id.Contains(TEXT("read"))) Skills.GainSkill(EApartmentLifeSkill::Creativity, 0.2f);
}

EApartmentLifeSimulationLOD UApartmentLifeWorldSimCityBridge::GetSimulationLODForDistance(float DistanceMeters, bool bPlayerDistrict)
{
	return UApartmentLifeCityLibrary::ComputeSimulationLOD(DistanceMeters, bPlayerDistrict);
}

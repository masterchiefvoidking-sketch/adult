// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCityLibrary.h"

EApartmentLifeDayPhase UApartmentLifeCityLibrary::GetDayPhaseForHour(int32 Hour)
{
	if (Hour >= 6 && Hour < 9) return EApartmentLifeDayPhase::MorningRush;
	if (Hour >= 9 && Hour < 12) return EApartmentLifeDayPhase::BusinessHours;
	if (Hour >= 12 && Hour < 17) return EApartmentLifeDayPhase::Afternoon;
	if (Hour >= 17 && Hour < 22) return EApartmentLifeDayPhase::Evening;
	return EApartmentLifeDayPhase::LateNight;
}

bool UApartmentLifeCityLibrary::IsBusinessOpen(const FApartmentLifeBuildingHours& Hours, int32 Hour, EApartmentLifeDayPhase Phase)
{
	if (Phase == EApartmentLifeDayPhase::LateNight)
	{
		return false;
	}
	return Hours.IsOpenAtHour(Hour);
}

FApartmentLifeTravelPlan UApartmentLifeCityLibrary::ComputeTravelPlan(
	FName OriginDistrictId,
	FName DestinationDistrictId,
	EApartmentLifeTransportMode PreferredMode,
	const UApartmentLifeTransitRouteData* Route)
{
	FApartmentLifeTravelPlan Plan;
	Plan.Mode = PreferredMode;

	if (Route)
	{
		Plan.TravelMinutes = Route->TravelMinutes;
		Plan.TravelCost = Route->Cost;
	}
	else
	{
		switch (PreferredMode)
		{
		case EApartmentLifeTransportMode::Walking: Plan.TravelMinutes = 25.f; break;
		case EApartmentLifeTransportMode::Bicycle: Plan.TravelMinutes = 12.f; break;
		case EApartmentLifeTransportMode::PublicTransit: Plan.TravelMinutes = 18.f; Plan.TravelCost = 2.5f; break;
		case EApartmentLifeTransportMode::RideShare: Plan.TravelMinutes = 10.f; Plan.TravelCost = 12.f; break;
		case EApartmentLifeTransportMode::PersonalCar: Plan.TravelMinutes = 8.f; Plan.TravelCost = 5.f; break;
		}
	}

	if (OriginDistrictId == DestinationDistrictId)
	{
		Plan.TravelMinutes *= 0.3f;
	}

	return Plan;
}

float UApartmentLifeCityLibrary::ComputeCommuteMinutes(FName HomeDistrictId, FName WorkDistrictId, EApartmentLifeTransportMode Mode)
{
	return ComputeTravelPlan(HomeDistrictId, WorkDistrictId, Mode, nullptr).TravelMinutes;
}

float UApartmentLifeCityLibrary::ComputeDistrictRent(const FApartmentLifeDistrictStats& District, const FApartmentLifeCityEconomyState& Economy)
{
	return District.AverageRent * Economy.RentFluctuation * District.PropertyValueIndex;
}

EApartmentLifeSimulationLOD UApartmentLifeCityLibrary::ComputeSimulationLOD(float DistanceToPlayerMeters, bool bIsPlayerDistrict)
{
	if (bIsPlayerDistrict || DistanceToPlayerMeters < 2000.f) return EApartmentLifeSimulationLOD::Full;
	if (DistanceToPlayerMeters < 5000.f) return EApartmentLifeSimulationLOD::Reduced;
	if (DistanceToPlayerMeters < 12000.f) return EApartmentLifeSimulationLOD::Background;
	return EApartmentLifeSimulationLOD::Dormant;
}

TArray<FName> UApartmentLifeCityLibrary::GetAvailableItemsAtBuilding(const UApartmentLifeCityBuildingData* Building, int32 Hour)
{
	TArray<FName> Available;
	if (!Building || !Building->Hours.IsOpenAtHour(Hour))
	{
		return Available;
	}

	for (const FName& ItemId : Building->Inventory.ItemIds)
	{
		if (const int32* Stock = Building->Inventory.StockLevels.Find(ItemId))
		{
			if (*Stock > 0)
			{
				Available.Add(ItemId);
			}
		}
		else
		{
			Available.Add(ItemId);
		}
	}
	return Available;
}

float UApartmentLifeCityLibrary::ApplyWeatherToTraffic(float BaseTraffic, EApartmentLifeWeather Weather)
{
	switch (Weather)
	{
	case EApartmentLifeWeather::Rain: return BaseTraffic * 1.15f;
	case EApartmentLifeWeather::Storm: return BaseTraffic * 1.3f;
	case EApartmentLifeWeather::Snow: return BaseTraffic * 1.25f;
	case EApartmentLifeWeather::Heatwave: return BaseTraffic * 0.95f;
	default: return BaseTraffic;
	}
}

void UApartmentLifeCityLibrary::TickCityEconomy(FApartmentLifeCityEconomyState& Economy, EApartmentLifeSeason Season, int32 DayOfMonth)
{
	switch (Season)
	{
	case EApartmentLifeSeason::Summer: Economy.SeasonalDemandMultiplier = 1.1f; break;
	case EApartmentLifeSeason::Winter: Economy.SeasonalDemandMultiplier = 0.95f; break;
	default: Economy.SeasonalDemandMultiplier = 1.f; break;
	}

	if (DayOfMonth == 1)
	{
		Economy.RentFluctuation = FMath::Clamp(Economy.RentFluctuation + (Economy.HousingDemand - 0.5f) * 0.05f, 0.8f, 1.3f);
	}
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCitySubsystem.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "ApartmentLifeCityLibrary.h"
#include "ApartmentLifeCityDataAssets.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "JsonObjectConverter.h"

void UApartmentLifeCitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("District"))))
				{
					RegisterDistrict(Cast<UApartmentLifeDistrictData>(Asset));
				}
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("CityBuilding"))))
				{
					RegisterBuilding(Cast<UApartmentLifeCityBuildingData>(Asset));
				}
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("CityPOI"))))
				{
					RegisterPOI(Cast<UApartmentLifeCityPOIData>(Asset));
				}
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("CityEvent"))))
				{
					if (UApartmentLifeCityEventData* Event = Cast<UApartmentLifeCityEventData>(Asset))
					{
						CityEvents.Add(Event);
					}
				}
			}
		}

		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnHourAdvanced.AddDynamic(this, &UApartmentLifeCitySubsystem::HandleHourAdvanced);
			TimeSubsystem->OnDayAdvanced.AddDynamic(this, &UApartmentLifeCitySubsystem::HandleDayAdvanced);
			bTimeBound = true;
		}
	}

	FApartmentLifeSaveableRegistry::Register(this);
}

void UApartmentLifeCitySubsystem::Deinitialize()
{
	FApartmentLifeSaveableRegistry::Unregister(this);
	Super::Deinitialize();
}

void UApartmentLifeCitySubsystem::RegisterDistrict(UApartmentLifeDistrictData* District)
{
	if (District)
	{
		Districts.Add(District->AssetId, District);
	}
}

void UApartmentLifeCitySubsystem::RegisterBuilding(UApartmentLifeCityBuildingData* Building)
{
	if (Building)
	{
		Buildings.Add(Building->AssetId, Building);
	}
}

void UApartmentLifeCitySubsystem::RegisterPOI(UApartmentLifeCityPOIData* POI)
{
	if (POI)
	{
		POIs.Add(POI->AssetId, POI);
	}
}

void UApartmentLifeCitySubsystem::SetPlayerDistrict(FName DistrictId)
{
	PlayerDistrictId = DistrictId;
}

void UApartmentLifeCitySubsystem::UpdateNPCLocation(FName CharacterId, FName DistrictId, FName POIId, float DistanceToPlayer)
{
	FApartmentLifeNPCLocationState& State = NPCLocations.FindOrAdd(CharacterId);
	State.CharacterId = CharacterId;
	State.CurrentDistrictId = DistrictId;
	State.CurrentPOIId = POIId;
	State.SimulationLOD = UApartmentLifeCityLibrary::ComputeSimulationLOD(DistanceToPlayer, DistrictId == PlayerDistrictId);
}

TArray<FName> UApartmentLifeCitySubsystem::GetBuildingsInDistrict(FName DistrictId) const
{
	TArray<FName> Result;
	for (const TPair<FName, TObjectPtr<UApartmentLifeCityBuildingData>>& Pair : Buildings)
	{
		if (Pair.Value && Pair.Value->DistrictId == DistrictId)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

TArray<FName> UApartmentLifeCitySubsystem::GetOpenBuildingsAtHour(FName DistrictId, int32 Hour) const
{
	TArray<FName> Result;
	const EApartmentLifeDayPhase Phase = UApartmentLifeCityLibrary::GetDayPhaseForHour(Hour);

	for (const TPair<FName, TObjectPtr<UApartmentLifeCityBuildingData>>& Pair : Buildings)
	{
		if (Pair.Value && Pair.Value->DistrictId == DistrictId
			&& UApartmentLifeCityLibrary::IsBusinessOpen(Pair.Value->Hours, Hour, Phase))
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

void UApartmentLifeCitySubsystem::SimulateBackgroundDistricts()
{
	for (const TPair<FName, TObjectPtr<UApartmentLifeDistrictData>>& Pair : Districts)
	{
		if (Pair.Key == PlayerDistrictId)
		{
			continue;
		}

		OnDistrictSimulationUpdated.Broadcast(Pair.Key);
	}
}

void UApartmentLifeCitySubsystem::HandleHourAdvanced(const FApartmentLifeGameTime& NewTime)
{
	UpdateDistrictTraffic(NewTime);
	SimulateBackgroundDistricts();
}

void UApartmentLifeCitySubsystem::HandleDayAdvanced(const FApartmentLifeGameTime& NewTime)
{
	UpdateEconomy(NewTime);
	EvaluateCityEvents(NewTime);
}

void UApartmentLifeCitySubsystem::EvaluateCityEvents(const FApartmentLifeGameTime& Time)
{
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			for (UApartmentLifeCityEventData* EventData : CityEvents)
			{
				if (!EventData) continue;
				if (EventData->bRequiresWeekend && !TimeSubsystem->IsWeekend()) continue;
				if (EventData->ValidSeasons.Num() > 0 && !EventData->ValidSeasons.Contains(TimeSubsystem->GetCurrentSeason())) continue;

				if (FMath::FRand() < 0.08f)
				{
					FApartmentLifeActiveCityEvent Active;
					Active.EventId = EventData->AssetId;
					Active.EventType = EventData->EventType;
					Active.StartTime = Time;
					Active.DurationHours = EventData->DurationHours;
					Active.bIsActive = true;

					if (Districts.Num() > 0)
					{
						TArray<FName> Keys;
						Districts.GetKeys(Keys);
						Active.DistrictId = Keys[FMath::RandRange(0, Keys.Num() - 1)];
					}

					ActiveEvents.Add(Active);
					Economy.ActiveSaleEvents.AddUnique(EventData->AssetId);
					OnCityEventStarted.Broadcast(Active);
					return;
				}
			}
		}
	}
}

void UApartmentLifeCitySubsystem::UpdateEconomy(const FApartmentLifeGameTime& Time)
{
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			UApartmentLifeCityLibrary::TickCityEconomy(Economy, TimeSubsystem->GetCurrentSeason(), Time.Day);
		}
	}
}

void UApartmentLifeCitySubsystem::UpdateDistrictTraffic(const FApartmentLifeGameTime& Time)
{
	EApartmentLifeWeather Weather = EApartmentLifeWeather::Clear;
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Weather = TimeSubsystem->GetCurrentWeather().Weather;
		}
	}

	for (TPair<FName, TObjectPtr<UApartmentLifeDistrictData>>& Pair : Districts)
	{
		if (UApartmentLifeDistrictData* District = Pair.Value)
		{
			const EApartmentLifeDayPhase Phase = UApartmentLifeCityLibrary::GetDayPhaseForHour(Time.Hour);
			float Traffic = District->Stats.TrafficDensity;
			if (Phase == EApartmentLifeDayPhase::MorningRush || Phase == EApartmentLifeDayPhase::Evening)
			{
				Traffic *= 1.4f;
			}
			District->Stats.TrafficDensity = UApartmentLifeCityLibrary::ApplyWeatherToTraffic(Traffic, Weather);
		}
	}
}

FString UApartmentLifeCitySubsystem::GetSaveId_Implementation() const
{
	return TEXT("city_subsystem");
}

void UApartmentLifeCitySubsystem::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString EconomyJson;
	FJsonObjectConverter::UStructToJsonObjectString(Economy, EconomyJson);
	OutData.Add(TEXT("Economy"), EconomyJson);

	FString EventsJson;
	FJsonObjectConverter::UStructToJsonObjectString(ActiveEvents, EventsJson);
	OutData.Add(TEXT("ActiveEvents"), EventsJson);

	FString LocationsJson;
	FJsonObjectConverter::UStructToJsonObjectString(NPCLocations, LocationsJson);
	OutData.Add(TEXT("NPCLocations"), LocationsJson);

	OutData.Add(TEXT("PlayerDistrict"), PlayerDistrictId.ToString());
}

void UApartmentLifeCitySubsystem::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* EconomyJson = InData.Find(TEXT("Economy")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*EconomyJson, &Economy);
	}
	if (const FString* EventsJson = InData.Find(TEXT("ActiveEvents")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*EventsJson, &ActiveEvents);
	}
	if (const FString* LocationsJson = InData.Find(TEXT("NPCLocations")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*LocationsJson, &NPCLocations);
	}
	if (const FString* District = InData.Find(TEXT("PlayerDistrict")))
	{
		PlayerDistrictId = FName(**District);
	}
}

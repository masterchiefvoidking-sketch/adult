// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorldEventSubsystem.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeDataRegistrySubsystem.h"

void UApartmentLifeWorldEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnDayAdvanced.AddDynamic(this, &UApartmentLifeWorldEventSubsystem::HandleDayAdvanced);
		}

		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("RandomEvent"))))
				{
					if (UApartmentLifeRandomEventData* EventData = Cast<UApartmentLifeRandomEventData>(Asset))
					{
						RegisterEventData(EventData);
					}
				}
			}
		}
	}
}

void UApartmentLifeWorldEventSubsystem::RegisterEventData(UApartmentLifeRandomEventData* EventData)
{
	if (EventData)
	{
		RegisteredEvents.AddUnique(EventData);
	}
}

void UApartmentLifeWorldEventSubsystem::HandleDayAdvanced(const FApartmentLifeGameTime& NewTime)
{
	EvaluateDailyEvents();
}

void UApartmentLifeWorldEventSubsystem::EvaluateDailyEvents()
{
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			for (UApartmentLifeRandomEventData* EventData : RegisteredEvents)
			{
				if (!EventData)
				{
					continue;
				}

				if (EventData->bRequiresWeekend && !TimeSubsystem->IsWeekend())
				{
					continue;
				}

				if (EventData->ValidSeasons.Num() > 0 && !EventData->ValidSeasons.Contains(TimeSubsystem->GetCurrentSeason()))
				{
					continue;
				}

				const float Probability = ComputeEventProbability(EventData);
				if (FMath::FRand() < Probability)
				{
					OnWorldEventTriggered.Broadcast(EventData->AssetId);
					return;
				}
			}
		}
	}
}

float UApartmentLifeWorldEventSubsystem::ComputeEventProbability(const UApartmentLifeRandomEventData* EventData) const
{
	return EventData ? FMath::Clamp(EventData->BaseProbability, 0.f, 1.f) : 0.f;
}

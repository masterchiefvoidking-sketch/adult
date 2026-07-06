// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCleaningComponent.h"
#include "ApartmentLifeBuilderLibrary.h"

void UApartmentLifeCleaningComponent::TickCleaningSimulation(float DeltaHours, float OccupantTidiness)
{
	OnCleaningStateChanged.Broadcast();
}

void UApartmentLifeCleaningComponent::CleanInstance(FApartmentLifePlacedFurnitureInstance& Instance)
{
	Instance.Condition.DirtLevel = 0.f;
	Instance.Condition.Cleanliness = 100.f;
	Instance.Condition.ActiveDirtTypes.Empty();
	Instance.Condition.ActiveDirtTypes.Remove(EApartmentLifeDirtType::Dust);
	OnCleaningStateChanged.Broadcast();
}

bool UApartmentLifeCleaningComponent::RequestMaintenance(FApartmentLifePlacedFurnitureInstance& Instance)
{
	if (!Instance.Condition.bNeedsMaintenance && !Instance.Condition.bIsBroken)
	{
		return false;
	}

	Instance.Condition.bNeedsMaintenance = false;
	Instance.Condition.bIsBroken = false;
	Instance.Condition.Durability = FMath::Min(Instance.Condition.Durability + 40.f, 100.f);
	Instance.Condition.ActiveDirtTypes.Remove(EApartmentLifeDirtType::Broken);
	OnCleaningStateChanged.Broadcast();
	return true;
}

void UApartmentLifeCleaningComponent::AutoCleanByTidiness(TArray<FApartmentLifePlacedFurnitureInstance>& Instances, float Tidiness, float Mood)
{
	if (FMath::FRand() > Tidiness * 0.5f + Mood * 0.001f)
	{
		return;
	}

	for (FApartmentLifePlacedFurnitureInstance& Instance : Instances)
	{
		if (Instance.Condition.DirtLevel > 20.f)
		{
			UApartmentLifeBuilderLibrary::ApplyWearAndDirt(Instance.Condition, -1.f, Tidiness);
			Instance.Condition.DirtLevel = FMath::Max(0.f, Instance.Condition.DirtLevel - 30.f);
			Instance.Condition.Cleanliness = 100.f - Instance.Condition.DirtLevel;
		}
	}
	OnCleaningStateChanged.Broadcast();
}

float UApartmentLifeCleaningComponent::GetApartmentCleanliness(const TArray<FApartmentLifePlacedFurnitureInstance>& Instances) const
{
	if (Instances.Num() == 0)
	{
		return 100.f;
	}

	float Total = 0.f;
	for (const FApartmentLifePlacedFurnitureInstance& Instance : Instances)
	{
		Total += Instance.Condition.Cleanliness;
	}
	return Total / Instances.Num();
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeBedroomRoutineComponent.h"

void UApartmentLifeBedroomRoutineComponent::StartRoutine(EApartmentLifeBedroomRoutineType RoutineType)
{
	ActiveRoutine = RoutineType;
}

void UApartmentLifeBedroomRoutineComponent::EndRoutine()
{
	ActiveRoutine = EApartmentLifeBedroomRoutineType::None;
}

FString UApartmentLifeBedroomRoutineComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("bedroom_%s"), *Owner->GetName());
	}
	return TEXT("bedroom_unknown");
}

void UApartmentLifeBedroomRoutineComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("ActiveRoutine"), FString::FromInt(static_cast<int32>(ActiveRoutine)));
}

void UApartmentLifeBedroomRoutineComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Value = InData.Find(TEXT("ActiveRoutine")))
	{
		ActiveRoutine = static_cast<EApartmentLifeBedroomRoutineType>(FCString::Atoi(**Value));
	}
}

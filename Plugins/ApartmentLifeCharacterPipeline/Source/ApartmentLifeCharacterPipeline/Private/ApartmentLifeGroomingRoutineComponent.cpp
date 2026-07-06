// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeGroomingRoutineComponent.h"
#include "ApartmentLifeAnimationData.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "JsonObjectConverter.h"

void UApartmentLifeGroomingRoutineComponent::StartRoutine(UApartmentLifeGroomingRoutineData* RoutineData)
{
	ActiveRoutine = RoutineData;
	if (RoutineData && RoutineData->Steps.Num() > 0)
	{
		RoutineState.CurrentStep = RoutineData->Steps[0];
		RoutineState.PreferredStepOrder = RoutineData->Steps;
		OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
	}
}

void UApartmentLifeGroomingRoutineComponent::StartBuiltinShowerRoutine()
{
	ActiveRoutine = nullptr;
	RoutineState = FApartmentLifeGroomingRoutineState();
	RoutineState.PreferredStepOrder = {
		EApartmentLifeGroomingStep::Shower,
		EApartmentLifeGroomingStep::BrushTeeth,
		EApartmentLifeGroomingStep::HairCare,
		EApartmentLifeGroomingStep::SkinCare
	};
	RoutineState.CurrentStep = EApartmentLifeGroomingStep::Shower;
	OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
}

void UApartmentLifeGroomingRoutineComponent::StartBuiltinMirrorRoutine()
{
	ActiveRoutine = nullptr;
	RoutineState = FApartmentLifeGroomingRoutineState();
	RoutineState.PreferredStepOrder = {
		EApartmentLifeGroomingStep::WashFace,
		EApartmentLifeGroomingStep::HairCare,
		EApartmentLifeGroomingStep::Makeup,
		EApartmentLifeGroomingStep::MirrorCheck
	};
	RoutineState.CurrentStep = EApartmentLifeGroomingStep::WashFace;
	OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
}

void UApartmentLifeGroomingRoutineComponent::AdvanceStep()
{
	if (RoutineState.PreferredStepOrder.Num() == 0)
	{
		return;
	}

	if (ActiveRoutine)
	{
		RoutineState = UApartmentLifeCharacterPipelineLibrary::AdvanceGroomingStep(RoutineState, ActiveRoutine);
		OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
		return;
	}

	int32 Index = RoutineState.PreferredStepOrder.Find(RoutineState.CurrentStep);
	if (Index == INDEX_NONE)
	{
		Index = 0;
	}
	else if (Index + 1 >= RoutineState.PreferredStepOrder.Num())
	{
		OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
		return;
	}
	else
	{
		Index++;
	}

	RoutineState.CurrentStep = RoutineState.PreferredStepOrder[Index];
	RoutineState.Hygiene = FMath::Min(RoutineState.Hygiene + 5.f, 100.f);
	RoutineState.Confidence = FMath::Min(RoutineState.Confidence + 2.f, 100.f);
	OnGroomingStepChanged.Broadcast(RoutineState.CurrentStep);
}

float UApartmentLifeGroomingRoutineComponent::GetScheduleReadinessBonus() const
{
	return RoutineState.Hygiene * 0.01f + RoutineState.Confidence * 0.005f;
}

FString UApartmentLifeGroomingRoutineComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("grooming_%s"), *Owner->GetName());
	return TEXT("grooming_unknown");
}

void UApartmentLifeGroomingRoutineComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(RoutineState, Json);
	OutData.Add(TEXT("RoutineState"), Json);
}

void UApartmentLifeGroomingRoutineComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Json = InData.Find(TEXT("RoutineState")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &RoutineState);
	}
}

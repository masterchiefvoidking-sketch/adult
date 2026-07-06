// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "JsonObjectConverter.h"

void UApartmentLifeAnimationComponent::SetAnimationGroup(EApartmentLifeAnimationGroup Group)
{
	AnimationState.CurrentGroup = Group;
	AnimationState.MontageId = UApartmentLifeCharacterPipelineLibrary::GetAnimationIdForGroup(Group);
	OnAnimationGroupChanged.Broadcast(Group);
}

FString UApartmentLifeAnimationComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("anim_%s"), *Owner->GetName());
	return TEXT("anim_unknown");
}

void UApartmentLifeAnimationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(FitnessProgress, Json);
	OutData.Add(TEXT("FitnessProgress"), Json);
}

void UApartmentLifeAnimationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Json = InData.Find(TEXT("FitnessProgress")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &FitnessProgress);
	}
}

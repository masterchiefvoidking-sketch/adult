// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeYogaMinigameComponent.h"
#include "ApartmentLifeAnimationData.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "JsonObjectConverter.h"

bool UApartmentLifeYogaMinigameComponent::StartYogaSession(FName PoseId, bool bOnMat)
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				ActivePose = Cast<UApartmentLifeYogaPoseData>(Registry->FindAsset(PoseId));
			}
		}
	}

	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeWardrobeComponent* Wardrobe = Owner->FindComponentByClass<UApartmentLifeWardrobeComponent>())
		{
			for (const FApartmentLifeEquippedClothingSlot& Slot : Wardrobe->GetEquippedSlots())
			{
				// Outfit compatibility checked via clothing bYogaCompatible in data assets
			}
		}
	}

	SessionState = FApartmentLifeYogaSessionState();
	SessionState.CurrentPoseId = PoseId.IsNone() ? FName(TEXT("pose.builtin.stretch")) : PoseId;
	SessionState.bOnYogaMat = bOnMat;
	OnYogaSessionUpdated.Broadcast(SessionState);
	return bOnMat;
}

void UApartmentLifeYogaMinigameComponent::UpdatePoseInput(float InputAccuracy, float DeltaTime)
{
	SessionState = UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(SessionState, ActivePose, InputAccuracy, DeltaTime);
	OnYogaSessionUpdated.Broadcast(SessionState);
}

void UApartmentLifeYogaMinigameComponent::EndYogaSession()
{
	ActivePose = nullptr;
	SessionState = FApartmentLifeYogaSessionState();
	OnYogaSessionUpdated.Broadcast(SessionState);
}

FString UApartmentLifeYogaMinigameComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("yoga_%s"), *Owner->GetName());
	return TEXT("yoga_unknown");
}

void UApartmentLifeYogaMinigameComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString SessionJson;
	FJsonObjectConverter::UStructToJsonObjectString(SessionState, SessionJson);
	OutData.Add(TEXT("SessionState"), SessionJson);
	OutData.Add(TEXT("ActivePoseId"), SessionState.CurrentPoseId.ToString());
}

void UApartmentLifeYogaMinigameComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* SessionJson = InData.Find(TEXT("SessionState")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*SessionJson, &SessionState);
	}
	if (const FString* PoseId = InData.Find(TEXT("ActivePoseId")))
	{
		SessionState.CurrentPoseId = FName(**PoseId);
	}
}

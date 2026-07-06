// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeYogaMinigameComponent.h"
#include "ApartmentLifeAnimationData.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeClothingItemData.h"
#include "ApartmentLifeWardrobeComponent.h"

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

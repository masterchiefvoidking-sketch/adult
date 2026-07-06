// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeAnimationLibrary.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "JsonObjectConverter.h"

void UApartmentLifeAnimationComponent::SetAnimationGroup(EApartmentLifeAnimationGroup Group)
{
	TransitionToGroup(Group, EApartmentLifeAnimationTransitionKind::Blend);
}

void UApartmentLifeAnimationComponent::TransitionToGroup(
	EApartmentLifeAnimationGroup Group,
	EApartmentLifeAnimationTransitionKind TransitionKind)
{
	PreviousGroup = AnimationState.CurrentGroup;
	AnimationState.TransitionKind = TransitionKind;
	AnimationState.TransitionMontageId = UApartmentLifeAnimationLibrary::GetTransitionMontageId(PreviousGroup, Group);
	AnimationState.CurrentGroup = Group;
	AnimationState.MontageId = UApartmentLifeCharacterPipelineLibrary::GetAnimationIdForGroup(Group);
	AnimationState.bPhysicsHairEnabled = UApartmentLifeAnimationLibrary::ShouldEnablePhysicsForGroup(Group);
	AnimationState.bPhysicsClothEnabled = AnimationState.bPhysicsHairEnabled;
	OnAnimationGroupChanged.Broadcast(Group);
	BroadcastState();
}

void UApartmentLifeAnimationComponent::PlayActivityAnimation(
	FName ActivityId,
	EApartmentLifeAnimationGroup Group,
	const FApartmentLifeInteractionAlignmentSet& Alignment,
	float Mood,
	float Energy)
{
	AnimationState.ActiveActivityId = ActivityId;
	AnimationState.FacialExpression = UApartmentLifeAnimationLibrary::GetFacialExpressionForActivity(ActivityId, Mood, Energy);
	ApplyAlignment(Alignment);
	TransitionToGroup(Group, UApartmentLifeAnimationLibrary::GetTransitionBetweenGroups(PreviousGroup, Group));
	AnimationState.MontageId = UApartmentLifeAnimationLibrary::GetMontageIdForActivity(ActivityId);
	BroadcastState();
}

void UApartmentLifeAnimationComponent::PlayYogaPose(FName PoseId, FName PoseMontageId)
{
	AnimationState.ActiveActivityId = PoseId;
	AnimationState.bUseFullBodyIK = true;
	TransitionToGroup(EApartmentLifeAnimationGroup::Yoga, EApartmentLifeAnimationTransitionKind::Blend);
	AnimationState.MontageId = PoseMontageId.IsNone() ? FName(*FString::Printf(TEXT("montage.yoga.%s"), *PoseId.ToString())) : PoseMontageId;
	BroadcastState();
}

void UApartmentLifeAnimationComponent::SetFacialExpression(EApartmentLifeFacialExpression Expression)
{
	AnimationState.FacialExpression = Expression;
	AnimationState.bEnableFacialAnimation = true;
	BroadcastState();
}

void UApartmentLifeAnimationComponent::SetMontageId(FName MontageId)
{
	AnimationState.MontageId = MontageId;
	BroadcastState();
}

void UApartmentLifeAnimationComponent::ApplyMovementStyles(
	EApartmentLifeMovementPersonality Personality,
	FName WalkStyleId,
	FName IdleStyleId)
{
	AnimationState.MovementPersonality = Personality;
	AnimationState.WalkStyleId = WalkStyleId;
	AnimationState.IdleStyleId = IdleStyleId;
	BroadcastState();
}

void UApartmentLifeAnimationComponent::ResetToIdle()
{
	AnimationState = FApartmentLifeAnimationState();
	AnimationState.CurrentGroup = EApartmentLifeAnimationGroup::Idle;
	AnimationState.MontageId = UApartmentLifeCharacterPipelineLibrary::GetAnimationIdForGroup(EApartmentLifeAnimationGroup::Idle);
	PreviousGroup = EApartmentLifeAnimationGroup::Idle;
	OnAnimationGroupChanged.Broadcast(EApartmentLifeAnimationGroup::Idle);
	BroadcastState();
}

void UApartmentLifeAnimationComponent::MarkCompletedAnimationEvent(FName EventId)
{
	AnimationState.LastCompletedAnimationEvent = EventId;
	BroadcastState();
}

void UApartmentLifeAnimationComponent::RestoreAnimationAfterLoad()
{
	if (AnimationState.CurrentGroup != EApartmentLifeAnimationGroup::Idle)
	{
		OnAnimationGroupChanged.Broadcast(AnimationState.CurrentGroup);
	}
	BroadcastState();
}

void UApartmentLifeAnimationComponent::BroadcastState()
{
	OnAnimationStateChanged.Broadcast(AnimationState);
}

void UApartmentLifeAnimationComponent::ApplyAlignment(const FApartmentLifeInteractionAlignmentSet& Alignment)
{
	if (!Alignment.bHasAlignment)
	{
		return;
	}

	AnimationState.Alignment = Alignment;
	AnimationState.IkTargets = UApartmentLifeAnimationLibrary::BuildIkTargetsFromAlignment(Alignment);
	AnimationState.bUseFullBodyIK = true;
	if (AActor* Owner = GetOwner())
	{
		UApartmentLifeAnimationLibrary::AlignCharacterToInteraction(Owner, Alignment);
	}
}

FString UApartmentLifeAnimationComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("anim_%s"), *Owner->GetName());
	return TEXT("anim_unknown");
}

void UApartmentLifeAnimationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString FitnessJson;
	FJsonObjectConverter::UStructToJsonObjectString(FitnessProgress, FitnessJson);
	OutData.Add(TEXT("FitnessProgress"), FitnessJson);

	FString StateJson;
	FJsonObjectConverter::UStructToJsonObjectString(AnimationState, StateJson);
	OutData.Add(TEXT("AnimationState"), StateJson);
	OutData.Add(TEXT("PreviousGroup"), UEnum::GetValueAsString(PreviousGroup));
}

void UApartmentLifeAnimationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* FitnessJson = InData.Find(TEXT("FitnessProgress")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*FitnessJson, &FitnessProgress);
	}
	if (const FString* StateJson = InData.Find(TEXT("AnimationState")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*StateJson, &AnimationState);
	}
	if (const FString* Prev = InData.Find(TEXT("PreviousGroup")))
	{
		const UEnum* Enum = StaticEnum<EApartmentLifeAnimationGroup>();
		if (Enum)
		{
			const int64 Value = Enum->GetValueByNameString(*Prev);
			if (Value != INDEX_NONE)
			{
				PreviousGroup = static_cast<EApartmentLifeAnimationGroup>(Value);
			}
		}
	}
}

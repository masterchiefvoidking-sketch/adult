// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeAnimationLibrary.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeBodyCustomizationComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "GameFramework/Character.h"

namespace
{
	FTransform BuildPointTransform(const AActor* Actor, const FApartmentLifeInteractionPoint& Point)
	{
		if (!Actor)
		{
			return FTransform::Identity;
		}

		if (!Point.SocketName.IsNone())
		{
			if (const USceneComponent* Root = Actor->GetRootComponent())
			{
				if (Root->DoesSocketExist(Point.SocketName))
				{
					return Root->GetSocketTransform(Point.SocketName, ERelativeTransformSpace::RTS_World);
				}
			}
		}

		return FTransform(Point.LocalRotation, Actor->GetActorLocation() + Point.LocalOffset);
	}
}

FName UApartmentLifeAnimationLibrary::GetMontageIdForActivity(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (UApartmentLifeActivityLibrary::TryGetDefinition(ActivityId, Def) && !Def.MontageId.IsNone())
	{
		return Def.MontageId;
	}

	FString Id = ActivityId.ToString();
	if (Id.StartsWith(TEXT("activity.")))
	{
		Id = Id.RightChop(9);
	}
	return FName(*FString::Printf(TEXT("montage.%s"), *Id));
}

EApartmentLifeAnimationGroup UApartmentLifeAnimationLibrary::GetAnimationGroupForGroomingStep(EApartmentLifeGroomingStep Step)
{
	switch (Step)
	{
	case EApartmentLifeGroomingStep::Shower: return EApartmentLifeAnimationGroup::ShowerRoutine;
	case EApartmentLifeGroomingStep::BrushTeeth:
	case EApartmentLifeGroomingStep::WashFace:
	case EApartmentLifeGroomingStep::HairCare:
	case EApartmentLifeGroomingStep::Makeup:
	case EApartmentLifeGroomingStep::SkinCare:
	case EApartmentLifeGroomingStep::MirrorCheck: return EApartmentLifeAnimationGroup::MirrorRoutine;
	case EApartmentLifeGroomingStep::ChangeClothes: return EApartmentLifeAnimationGroup::Dressing;
	case EApartmentLifeGroomingStep::Laundry: return EApartmentLifeAnimationGroup::Laundry;
	case EApartmentLifeGroomingStep::ClosetOrg: return EApartmentLifeAnimationGroup::Dressing;
	default: return EApartmentLifeAnimationGroup::Grooming;
	}
}

EApartmentLifeFacialExpression UApartmentLifeAnimationLibrary::GetFacialExpressionForActivity(
	FName ActivityId, float Mood, float Energy)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("sleep"))) return EApartmentLifeFacialExpression::Sleepy;
	if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("exercise"))) return EApartmentLifeFacialExpression::Focused;
	if (Id.Contains(TEXT("relax")) || Id.Contains(TEXT("tv"))) return EApartmentLifeFacialExpression::Relaxed;
	if (Id.Contains(TEXT("talk")) || Id.Contains(TEXT("social"))) return EApartmentLifeFacialExpression::Happy;
	if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("computer"))) return EApartmentLifeFacialExpression::Focused;
	if (Energy < 30.f) return EApartmentLifeFacialExpression::Tired;
	if (Mood < 35.f) return EApartmentLifeFacialExpression::Stressed;
	if (Mood > 75.f) return EApartmentLifeFacialExpression::Happy;
	return EApartmentLifeFacialExpression::Neutral;
}

EApartmentLifeMovementPersonality UApartmentLifeAnimationLibrary::GetMovementPersonalityFromWalkStyle(EApartmentLifeWalkStyle WalkStyle)
{
	switch (WalkStyle)
	{
	case EApartmentLifeWalkStyle::Confident: return EApartmentLifeMovementPersonality::Confident;
	case EApartmentLifeWalkStyle::Energetic: return EApartmentLifeMovementPersonality::Energetic;
	case EApartmentLifeWalkStyle::Relaxed: return EApartmentLifeMovementPersonality::Relaxed;
	case EApartmentLifeWalkStyle::Reserved: return EApartmentLifeMovementPersonality::Shy;
	default: return EApartmentLifeMovementPersonality::Soft;
	}
}

EApartmentLifeAnimationTransitionKind UApartmentLifeAnimationLibrary::GetTransitionBetweenGroups(
	EApartmentLifeAnimationGroup FromGroup,
	EApartmentLifeAnimationGroup ToGroup)
{
	if (FromGroup == ToGroup)
	{
		return EApartmentLifeAnimationTransitionKind::Instant;
	}

	const bool bFromLocomotion = FromGroup == EApartmentLifeAnimationGroup::Walking
		|| FromGroup == EApartmentLifeAnimationGroup::Running
		|| FromGroup == EApartmentLifeAnimationGroup::TurnInPlace;
	const bool bToSit = ToGroup == EApartmentLifeAnimationGroup::Sitting
		|| ToGroup == EApartmentLifeAnimationGroup::ChairSitting
		|| ToGroup == EApartmentLifeAnimationGroup::CouchSitting
		|| ToGroup == EApartmentLifeAnimationGroup::FloorSitting;
	const bool bFromSit = FromGroup == EApartmentLifeAnimationGroup::Sitting
		|| FromGroup == EApartmentLifeAnimationGroup::ChairSitting
		|| FromGroup == EApartmentLifeAnimationGroup::CouchSitting
		|| FromGroup == EApartmentLifeAnimationGroup::FloorSitting;
	const bool bToLie = ToGroup == EApartmentLifeAnimationGroup::LyingDown
		|| ToGroup == EApartmentLifeAnimationGroup::Sleeping;
	const bool bFromLie = FromGroup == EApartmentLifeAnimationGroup::LyingDown
		|| FromGroup == EApartmentLifeAnimationGroup::Sleeping;

	if ((bFromLocomotion && bToSit) || (bFromSit && ToGroup == EApartmentLifeAnimationGroup::Stand)
		|| (bFromSit && ToGroup == EApartmentLifeAnimationGroup::Walking)
		|| (bFromLie && ToGroup == EApartmentLifeAnimationGroup::WakeUp)
		|| (FromGroup == EApartmentLifeAnimationGroup::Walking && bToLie))
	{
		return EApartmentLifeAnimationTransitionKind::MontageBridge;
	}

	return EApartmentLifeAnimationTransitionKind::Blend;
}

FName UApartmentLifeAnimationLibrary::GetTransitionMontageId(
	EApartmentLifeAnimationGroup FromGroup,
	EApartmentLifeAnimationGroup ToGroup)
{
	if (GetTransitionBetweenGroups(FromGroup, ToGroup) != EApartmentLifeAnimationTransitionKind::MontageBridge)
	{
		return NAME_None;
	}

	return FName(*FString::Printf(
		TEXT("montage.transition.%s_to_%s"),
		*UEnum::GetValueAsString(FromGroup),
		*UEnum::GetValueAsString(ToGroup)));
}

bool UApartmentLifeAnimationLibrary::ResolveAlignmentFromFurniture(
	AActor* FurnitureActor,
	FName ActivityId,
	FApartmentLifeInteractionAlignmentSet& OutAlignment)
{
	AApartmentLifeFurnitureActor* Furniture = Cast<AApartmentLifeFurnitureActor>(FurnitureActor);
	if (!Furniture)
	{
		return false;
	}

	EApartmentLifeFurnitureInteraction BestInteraction = EApartmentLifeFurnitureInteraction::Sit;
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("sleep")) || Id.Contains(TEXT("nap"))) BestInteraction = EApartmentLifeFurnitureInteraction::Sleep;
	else if (Id.Contains(TEXT("computer")) || Id.Contains(TEXT("work"))) BestInteraction = EApartmentLifeFurnitureInteraction::UseComputer;
	else if (Id.Contains(TEXT("cook"))) BestInteraction = EApartmentLifeFurnitureInteraction::Cook;
	else if (Id.Contains(TEXT("shower"))) BestInteraction = EApartmentLifeFurnitureInteraction::Shower;
	else if (Id.Contains(TEXT("mirror")) || Id.Contains(TEXT("groom"))) BestInteraction = EApartmentLifeFurnitureInteraction::Groom;
	else if (Id.Contains(TEXT("wardrobe")) || Id.Contains(TEXT("outfit"))) BestInteraction = EApartmentLifeFurnitureInteraction::ChangeOutfit;
	else if (Id.Contains(TEXT("yoga"))) BestInteraction = EApartmentLifeFurnitureInteraction::Yoga;
	else if (Id.Contains(TEXT("tv")) || Id.Contains(TEXT("watch"))) BestInteraction = EApartmentLifeFurnitureInteraction::WatchTV;
	else if (Id.Contains(TEXT("read"))) BestInteraction = EApartmentLifeFurnitureInteraction::Read;

	const FApartmentLifeInteractionPoint* SelectedPoint = nullptr;
	for (const FApartmentLifeInteractionPoint& Point : Furniture->GetInteractionPoints())
	{
		if (Point.InteractionType == BestInteraction)
		{
			SelectedPoint = &Point;
			break;
		}
		if (!SelectedPoint && (Point.ActivityId == ActivityId || Point.ActivityId.IsNone()))
		{
			SelectedPoint = &Point;
		}
	}

	if (!SelectedPoint)
	{
		return false;
	}

	const FTransform Entry = BuildPointTransform(Furniture, *SelectedPoint);
	OutAlignment.bHasAlignment = true;
	OutAlignment.EntryTransform = Entry;
	OutAlignment.ExitTransform = FTransform(SelectedPoint->LocalRotation, Entry.GetLocation() + SelectedPoint->ExitOffset);
	OutAlignment.FacingDirection = Entry.GetRotation().Vector();
	OutAlignment.HandsTarget = Entry.GetLocation() + Entry.GetRotation().RotateVector(SelectedPoint->HandsTargetOffset);
	OutAlignment.FeetTarget = Entry.GetLocation() + Entry.GetRotation().RotateVector(SelectedPoint->FeetTargetOffset);
	OutAlignment.SeatTarget = Entry.GetLocation() + Entry.GetRotation().RotateVector(SelectedPoint->SeatTargetOffset);
	OutAlignment.CameraFocusPoint = Entry.GetLocation() + Entry.GetRotation().RotateVector(SelectedPoint->CameraFocusOffset);
	OutAlignment.FurnitureActorName = Furniture->GetFName();
	OutAlignment.InteractionSocketName = SelectedPoint->SocketName;
	return true;
}

FApartmentLifeAnimationIkTargets UApartmentLifeAnimationLibrary::BuildIkTargetsFromAlignment(
	const FApartmentLifeInteractionAlignmentSet& Alignment)
{
	FApartmentLifeAnimationIkTargets Targets;
	if (!Alignment.bHasAlignment)
	{
		return Targets;
	}

	Targets.bEnableFootIk = true;
	Targets.bEnableHandIk = !Alignment.HandsTarget.IsNearlyZero();
	Targets.bEnableHeadLookAt = !Alignment.CameraFocusPoint.IsNearlyZero();
	Targets.bEnableEyeTracking = Targets.bEnableHeadLookAt;
	Targets.LeftFootTarget = Alignment.FeetTarget + FVector(-12.f, 0.f, 0.f);
	Targets.RightFootTarget = Alignment.FeetTarget + FVector(12.f, 0.f, 0.f);
	Targets.LeftHandTarget = Alignment.HandsTarget + FVector(-8.f, 0.f, 0.f);
	Targets.RightHandTarget = Alignment.HandsTarget + FVector(8.f, 0.f, 0.f);
	Targets.HeadLookAtTarget = Alignment.CameraFocusPoint;
	return Targets;
}

bool UApartmentLifeAnimationLibrary::AlignCharacterToInteraction(
	AActor* Character,
	const FApartmentLifeInteractionAlignmentSet& Alignment)
{
	if (!Character || !Alignment.bHasAlignment)
	{
		return false;
	}

	const FVector Location = Alignment.EntryTransform.GetLocation();
	const FRotator Rotation = Alignment.EntryTransform.Rotator();
	if (ACharacter* AsCharacter = Cast<ACharacter>(Character))
	{
		AsCharacter->SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		Character->SetActorLocationAndRotation(Location, Rotation);
	}
	return true;
}

void UApartmentLifeAnimationLibrary::ApplyActivityAnimation(
	UApartmentLifeAnimationComponent* Animation,
	FName ActivityId,
	AActor* ContextActor,
	UApartmentLifeBodyCustomizationComponent* Body,
	UApartmentLifeNPCSimulationComponent* Simulation)
{
	if (!Animation || ActivityId.IsNone())
	{
		return;
	}

	const EApartmentLifeAnimationGroup TargetGroup = UApartmentLifeActivityLibrary::GetAnimationGroupForActivity(ActivityId);
	float Mood = 60.f;
	float Energy = 60.f;
	if (Simulation)
	{
		Mood = Simulation->GetMood().OverallMood;
		Energy = Simulation->GetMood().Energy;
	}

	FApartmentLifeInteractionAlignmentSet Alignment;
	if (ContextActor)
	{
		ResolveAlignmentFromFurniture(ContextActor, ActivityId, Alignment);
	}

	Animation->PlayActivityAnimation(ActivityId, TargetGroup, Alignment, Mood, Energy);

	if (Body)
	{
		Animation->ApplyMovementStyles(
			GetMovementPersonalityFromWalkStyle(Body->GetBodyCustomization().WalkStyle),
			Body->GetWalkStyleId(),
			Body->GetIdleStyleId());
	}
}

void UApartmentLifeAnimationLibrary::ApplyGroomingStepAnimation(
	UApartmentLifeAnimationComponent* Animation,
	EApartmentLifeGroomingStep Step)
{
	if (!Animation)
	{
		return;
	}

	const EApartmentLifeAnimationGroup Group = GetAnimationGroupForGroomingStep(Step);
	Animation->TransitionToGroup(Group, EApartmentLifeAnimationTransitionKind::Blend);
	Animation->SetFacialExpression(
		Step == EApartmentLifeGroomingStep::Shower ? EApartmentLifeFacialExpression::Relaxed : EApartmentLifeFacialExpression::Focused);
}

void UApartmentLifeAnimationLibrary::ApplyYogaPoseAnimation(
	UApartmentLifeAnimationComponent* Animation,
	FName PoseId,
	FName PoseMontageId)
{
	if (!Animation)
	{
		return;
	}

	Animation->PlayYogaPose(PoseId, PoseMontageId);
	Animation->SetFacialExpression(EApartmentLifeFacialExpression::Focused);
}

bool UApartmentLifeAnimationLibrary::ShouldEnablePhysicsForGroup(EApartmentLifeAnimationGroup Group)
{
	switch (Group)
	{
	case EApartmentLifeAnimationGroup::Sleeping:
	case EApartmentLifeAnimationGroup::LyingDown:
		return false;
	default:
		return true;
	}
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeActivityFallbackLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeGirlLifeLibrary.h"
#include "ApartmentLifeWorldSimLibrary.h"
#include "ApartmentLifeActivityCameraLibrary.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeProgressionComponent.h"

bool UApartmentLifeActivityLibrary::TryGetDefinition(FName ActivityId, FApartmentLifeActivityDefinition& OutDefinition)
{
	return UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(ActivityId, OutDefinition);
}

int32 UApartmentLifeActivityLibrary::GetDurationMinutes(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		return Def.DurationMinutes;
	}
	return UApartmentLifeActivityFallbackLibrary::GetBuiltinDurationMinutes(ActivityId);
}

FText UApartmentLifeActivityLibrary::GetDisplayName(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		return Def.DisplayName;
	}
	return FText::FromName(ActivityId);
}

EApartmentLifeActivityRoom UApartmentLifeActivityLibrary::RoomTypeToActivityRoom(EApartmentLifeRoomType Room)
{
	switch (Room)
	{
	case EApartmentLifeRoomType::Bedroom: return EApartmentLifeActivityRoom::Bedroom;
	case EApartmentLifeRoomType::Bathroom: return EApartmentLifeActivityRoom::Bathroom;
	case EApartmentLifeRoomType::LivingRoom: return EApartmentLifeActivityRoom::LivingRoom;
	case EApartmentLifeRoomType::Kitchen: return EApartmentLifeActivityRoom::Kitchen;
	case EApartmentLifeRoomType::DiningRoom: return EApartmentLifeActivityRoom::DiningRoom;
	case EApartmentLifeRoomType::WalkInCloset: return EApartmentLifeActivityRoom::WalkInCloset;
	case EApartmentLifeRoomType::Office: return EApartmentLifeActivityRoom::Office;
	case EApartmentLifeRoomType::LaundryRoom: return EApartmentLifeActivityRoom::LaundryRoom;
	default: return EApartmentLifeActivityRoom::Any;
	}
}

EApartmentLifeRoomType UApartmentLifeActivityLibrary::ActivityRoomToRoomType(EApartmentLifeActivityRoom Room)
{
	switch (Room)
	{
	case EApartmentLifeActivityRoom::Bedroom: return EApartmentLifeRoomType::Bedroom;
	case EApartmentLifeActivityRoom::Bathroom: return EApartmentLifeRoomType::Bathroom;
	case EApartmentLifeActivityRoom::LivingRoom: return EApartmentLifeRoomType::LivingRoom;
	case EApartmentLifeActivityRoom::Kitchen: return EApartmentLifeRoomType::Kitchen;
	case EApartmentLifeActivityRoom::DiningRoom: return EApartmentLifeRoomType::DiningRoom;
	case EApartmentLifeActivityRoom::WalkInCloset: return EApartmentLifeRoomType::Bedroom;
	case EApartmentLifeActivityRoom::Office: return EApartmentLifeRoomType::Office;
	case EApartmentLifeActivityRoom::LaundryRoom: return EApartmentLifeRoomType::LaundryRoom;
	case EApartmentLifeActivityRoom::YogaArea: return EApartmentLifeRoomType::LivingRoom;
	default: return EApartmentLifeRoomType::LivingRoom;
	}
}

EApartmentLifeRoomType UApartmentLifeActivityLibrary::GetPreferredRoomForActivity(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		return ActivityRoomToRoomType(Def.RequiredRoom);
	}
	return EApartmentLifeRoomType::LivingRoom;
}

bool UApartmentLifeActivityLibrary::CanStartActivity(
	FName ActivityId,
	EApartmentLifeRoomType CurrentRoom,
	const UApartmentLifeActivityComponent* ActivityComponent)
{
	FApartmentLifeActivityDefinition Def;
	if (!TryGetDefinition(ActivityId, Def))
	{
		return true;
	}

	if (Def.RequiredRoom != EApartmentLifeActivityRoom::Any
		&& RoomTypeToActivityRoom(CurrentRoom) != Def.RequiredRoom)
	{
		return false;
	}

	if (ActivityComponent && !ActivityComponent->IsCooldownReady(ActivityId))
	{
		return false;
	}

	return true;
}

FApartmentLifeActivityCompletionResult UApartmentLifeActivityLibrary::ApplyActivityCompletion(
	UApartmentLifeNPCSimulationComponent* Simulation,
	FName ActivityId)
{
	FApartmentLifeActivityCompletionResult Result;
	Result.ActivityId = ActivityId;

	FApartmentLifeActivityDefinition Def;
	if (!TryGetDefinition(ActivityId, Def))
	{
		UApartmentLifeGirlLifeLibrary::ApplyActivityCompletion(Simulation, ActivityId);
		Result.bSuccess = Simulation != nullptr;
		return Result;
	}

	if (!Simulation)
	{
		return Result;
	}

	const FApartmentLifeActivityStatEffect& S = Def.StatEffects;
	Simulation->Mood.OverallMood = FMath::Clamp(Simulation->Mood.OverallMood + S.MoodDelta, 0.f, 100.f);
	Simulation->Mood.Energy = FMath::Clamp(Simulation->Mood.Energy + S.EnergyDelta, 0.f, 100.f);
	Simulation->Needs.Hygiene = FMath::Clamp(Simulation->Needs.Hygiene + S.HygieneDelta, 0.f, 100.f);
	Simulation->Needs.Hunger = FMath::Clamp(Simulation->Needs.Hunger + S.HungerDelta, 0.f, 100.f);
	Simulation->Mood.Comfort = FMath::Clamp(Simulation->Mood.Comfort + S.ComfortDelta, 0.f, 100.f);
	Simulation->Mood.Confidence = FMath::Clamp(Simulation->Mood.Confidence + S.ConfidenceDelta, 0.f, 100.f);
	Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress + S.StressDelta, 0.f, 100.f);
	Simulation->AffectionTowardPlayer = FMath::Clamp(Simulation->AffectionTowardPlayer + S.AffectionDelta, 0.f, 100.f);

	if (S.FitnessXPDelta > 0.f) Simulation->Skills.GainSkill(EApartmentLifeSkill::Fitness, S.FitnessXPDelta);
	if (S.OrganizationXPDelta > 0.f) Simulation->Skills.GainSkill(EApartmentLifeSkill::Organization, S.OrganizationXPDelta);
	if (S.CreativityXPDelta > 0.f) Simulation->Skills.GainSkill(EApartmentLifeSkill::Creativity, S.CreativityXPDelta);

	if (Def.bUsesIncomeFormula && UApartmentLifeWorkLibrary::IsComputerWorkActivity(ActivityId))
	{
		UApartmentLifeProgressionComponent* Progression = nullptr;
		if (AActor* Owner = Simulation->GetOwner())
		{
			Progression = Owner->FindComponentByClass<UApartmentLifeProgressionComponent>();
		}

		const FApartmentLifeWorkSessionResult WorkResult = UApartmentLifeWorkLibrary::ComputeWorkSessionResult(Simulation, Progression, ActivityId);
		UApartmentLifeWorkLibrary::ApplyWorkSessionResult(Simulation, Progression, WorkResult);
		Result.MoneyEarned = WorkResult.TotalIncome;
	}
	else if (Def.MoneyEffect != 0.f)
	{
		Result.MoneyEarned = Def.MoneyEffect;
		Simulation->Finance.Savings += Def.MoneyEffect;
	}

	if (ActivityId.ToString().Contains(TEXT("clean")))
	{
		Simulation->Apartment.Cleanliness = FMath::Clamp(Simulation->Apartment.Cleanliness + 8.f, 0.f, 100.f);
	}

	if (ActivityId.ToString().Contains(TEXT("social")) || ActivityId.ToString().Contains(TEXT("talk")))
	{
		Simulation->ApplySocialFulfillment(12.f);
	}

	Result.AppliedEffects = S;
	Result.NextActivityId = Def.NextActivityInChain;
	Result.bSuccess = true;

	Simulation->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(
		Simulation->Mood, Simulation->MoodInfluences, Simulation->Personality);

	return Result;
}

void UApartmentLifeActivityLibrary::ApplyActivitySkillGain(FApartmentLifeSkillSet& Skills, FName ActivityId)
{
	UApartmentLifeGirlLifeLibrary::ApplyActivitySkillGain(Skills, ActivityId);

	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		if (Def.StatEffects.FitnessXPDelta > 0.f) Skills.GainSkill(EApartmentLifeSkill::Fitness, Def.StatEffects.FitnessXPDelta * 0.1f);
		if (Def.StatEffects.OrganizationXPDelta > 0.f) Skills.GainSkill(EApartmentLifeSkill::Organization, Def.StatEffects.OrganizationXPDelta * 0.1f);
		if (Def.StatEffects.CreativityXPDelta > 0.f) Skills.GainSkill(EApartmentLifeSkill::Creativity, Def.StatEffects.CreativityXPDelta * 0.1f);
	}
}

FApartmentLifeActivityCameraFrame UApartmentLifeActivityLibrary::GetCameraFrameForActivity(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def) && Def.CameraFrame.ArmLength > 0.f)
	{
		return Def.CameraFrame;
	}
	return UApartmentLifeActivityCameraLibrary::GetFrameForActivity(ActivityId);
}

EApartmentLifePrimaryCameraMode UApartmentLifeActivityLibrary::GetCameraModeForActivity(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		return Def.CameraMode;
	}
	return UApartmentLifeActivityCameraLibrary::GetPrimaryModeForActivity(ActivityId);
}

EApartmentLifeAnimationGroup UApartmentLifeActivityLibrary::GetAnimationGroupForActivity(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (TryGetDefinition(ActivityId, Def))
	{
		return Def.AnimationGroup;
	}
	return UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(ActivityId);
}

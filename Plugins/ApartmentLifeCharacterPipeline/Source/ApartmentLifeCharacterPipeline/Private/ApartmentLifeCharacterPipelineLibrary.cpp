// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeClothingItemData.h"

float UApartmentLifeCharacterPipelineLibrary::ScoreClothingForNPCStyle(const UApartmentLifeClothingItemData* Item, const FApartmentLifeNPCStyleContext& Context, const FApartmentLifeWardrobeStyleProfile& StyleProfile)
{
	if (!Item || Item->Price > StyleProfile.MonthlyClothingBudget * StyleProfile.IncomeTier * 2.f)
	{
		return 0.f;
	}

	float Score = 1.f;

	if (Context.CareerId == StyleProfile.CareerId)
	{
		for (const FName& Tag : Item->StyleTags)
		{
			if (StyleProfile.PreferredStyleTags.Contains(Tag)) Score += 0.4f;
		}
	}

	if (Context.Income > 5000.f && Item->Price < 100.f) Score *= 0.5f;
	if (Context.Income < 2000.f && Item->Price > 200.f) Score *= 0.2f;

	if (Context.Weather.TemperatureCelsius < 12.f) Score += Item->WarmthRating;
	if (Context.Mood < 40.f && Item->Category == EApartmentLifeClothingCategory::AthleticWear) Score += 0.3f;

	for (const FLinearColor& Fav : StyleProfile.FavoriteColors)
	{
		Score += 0.1f;
	}

	return FMath::Clamp(Score, 0.f, 5.f);
}

FApartmentLifeYogaSessionState UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(const FApartmentLifeYogaSessionState& Current, const UApartmentLifeYogaPoseData* Pose, float PlayerInputAccuracy, float DeltaTime)
{
	FApartmentLifeYogaSessionState Updated = Current;
	if (!Pose || !Updated.bOnYogaMat) return Updated;

	Updated.PoseMatchScore = FMath::FInterpTo(Updated.PoseMatchScore, PlayerInputAccuracy * 100.f, DeltaTime, 3.f);
	Updated.BalanceMeter = FMath::Clamp(Updated.BalanceMeter + (PlayerInputAccuracy - 0.5f) * DeltaTime * 20.f, 0.f, 100.f);
	Updated.BreathingPhase += DeltaTime * 2.f;
	Updated.BreathingRhythm = FMath::Clamp(50.f + FMath::Sin(Updated.BreathingPhase) * 30.f, 0.f, 100.f);
	Updated.SessionDurationMinutes += DeltaTime / 60.f;

	if (Updated.PoseMatchScore > 75.f)
	{
		Updated.PoseStreak = FMath::Min(Updated.PoseStreak + 1, 99);
		Updated.FlexibilityXP += Pose->FlexibilityReward * DeltaTime;
		Updated.FitnessXP += Pose->Difficulty * DeltaTime * 2.f;
	}
	else if (Updated.PoseMatchScore < 40.f)
	{
		Updated.PoseStreak = 0;
	}

	return Updated;
}

FApartmentLifeGroomingRoutineState UApartmentLifeCharacterPipelineLibrary::AdvanceGroomingStep(const FApartmentLifeGroomingRoutineState& State, const UApartmentLifeGroomingRoutineData* Routine)
{
	FApartmentLifeGroomingRoutineState Updated = State;
	if (!Routine || Routine->Steps.Num() == 0) return Updated;

	int32 CurrentIndex = Routine->Steps.Find(Updated.CurrentStep);
	if (CurrentIndex == INDEX_NONE) CurrentIndex = 0;
	else CurrentIndex = (CurrentIndex + 1) % Routine->Steps.Num();

	Updated.CurrentStep = Routine->Steps[CurrentIndex];
	Updated.Hygiene = FMath::Min(Updated.Hygiene + Routine->HygieneGainPerStep, 100.f);
	Updated.Confidence = FMath::Min(Updated.Confidence + Routine->ConfidenceGainPerStep, 100.f);
	return Updated;
}

FName UApartmentLifeCharacterPipelineLibrary::GetAnimationIdForGroup(EApartmentLifeAnimationGroup Group)
{
	return FName(*FString::Printf(TEXT("anim.%s"), *UEnum::GetValueAsString(Group)));
}

EApartmentLifeAnimationGroup UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();

	if (Id.Contains(TEXT("yoga"))) return EApartmentLifeAnimationGroup::Yoga;
	if (Id.Contains(TEXT("stretch"))) return EApartmentLifeAnimationGroup::Stretching;
	if (Id.Contains(TEXT("exercise")) || Id.Contains(TEXT("workout"))) return EApartmentLifeAnimationGroup::Exercising;
	if (Id.Contains(TEXT("cook"))) return EApartmentLifeAnimationGroup::Cooking;
	if (Id.Contains(TEXT("eat")) || Id.Contains(TEXT("meal"))) return EApartmentLifeAnimationGroup::Eating;
	if (Id.Contains(TEXT("clean"))) return EApartmentLifeAnimationGroup::Cleaning;
	if (Id.Contains(TEXT("laundry"))) return EApartmentLifeAnimationGroup::Laundry;
	if (Id.Contains(TEXT("read"))) return EApartmentLifeAnimationGroup::Reading;
	if (Id.Contains(TEXT("game"))) return EApartmentLifeAnimationGroup::Gaming;
	if (Id.Contains(TEXT("computer")) || Id.Contains(TEXT("work"))) return EApartmentLifeAnimationGroup::ComputerUse;
	if (Id.Contains(TEXT("tv")) || Id.Contains(TEXT("watch"))) return EApartmentLifeAnimationGroup::WatchingTV;
	if (Id.Contains(TEXT("dance"))) return EApartmentLifeAnimationGroup::Dancing;
	if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("outfit"))) return EApartmentLifeAnimationGroup::Dressing;
	if (Id.Contains(TEXT("groom")) || Id.Contains(TEXT("makeup"))) return EApartmentLifeAnimationGroup::Grooming;
	if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("hygiene"))) return EApartmentLifeAnimationGroup::ShowerRoutine;
	if (Id.Contains(TEXT("mirror"))) return EApartmentLifeAnimationGroup::MirrorRoutine;
	if (Id.Contains(TEXT("sleep"))) return EApartmentLifeAnimationGroup::Sleeping;
	if (Id.Contains(TEXT("wake"))) return EApartmentLifeAnimationGroup::WakeUp;
	if (Id.Contains(TEXT("stand"))) return EApartmentLifeAnimationGroup::Stand;
	if (Id.Contains(TEXT("couch"))) return EApartmentLifeAnimationGroup::CouchSitting;
	if (Id.Contains(TEXT("chair"))) return EApartmentLifeAnimationGroup::ChairSitting;
	if (Id.Contains(TEXT("floor"))) return EApartmentLifeAnimationGroup::FloorSitting;
	if (Id.Contains(TEXT("sit"))) return EApartmentLifeAnimationGroup::Sitting;
	if (Id.Contains(TEXT("lie")) || Id.Contains(TEXT("lay"))) return EApartmentLifeAnimationGroup::LyingDown;
	if (Id.Contains(TEXT("run"))) return EApartmentLifeAnimationGroup::Running;
	if (Id.Contains(TEXT("walk"))) return EApartmentLifeAnimationGroup::Walking;
	if (Id.Contains(TEXT("turn"))) return EApartmentLifeAnimationGroup::TurnInPlace;
	if (Id.Contains(TEXT("talk")) || Id.Contains(TEXT("social"))) return EApartmentLifeAnimationGroup::Conversation;

	return EApartmentLifeAnimationGroup::Idle;
}

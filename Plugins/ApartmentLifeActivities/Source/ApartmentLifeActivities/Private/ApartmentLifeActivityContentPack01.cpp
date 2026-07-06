// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityContentPack01.h"
#include "ApartmentLifeCameraTypes.h"

namespace
{
	FApartmentLifeActivityStatEffect Stats(float Mood = 0, float Energy = 0, float Hygiene = 0, float Hunger = 0, float Comfort = 0, float Confidence = 0, float Stress = 0)
	{
		FApartmentLifeActivityStatEffect S;
		S.MoodDelta = Mood; S.EnergyDelta = Energy; S.HygieneDelta = Hygiene;
		S.HungerDelta = Hunger; S.ComfortDelta = Comfort; S.ConfidenceDelta = Confidence; S.StressDelta = Stress;
		return S;
	}

	FApartmentLifeActivityDefinition MakeActivity(
		FName Id, const TCHAR* Name, EApartmentLifeActivityCategory Category, EApartmentLifeActivityRoom Room,
		EApartmentLifeAnimationGroup Anim, int32 Duration, const FApartmentLifeActivityStatEffect& S,
		EApartmentLifeOutfitContext Outfit = EApartmentLifeOutfitContext::Everyday)
	{
		FApartmentLifeActivityDefinition Def;
		Def.ActivityId = Id;
		Def.DisplayName = FText::FromString(Name);
		Def.Category = Category;
		Def.RequiredRoom = Room;
		Def.AnimationGroup = Anim;
		Def.DurationMinutes = Duration;
		Def.StatEffects = S;
		Def.RequiredOutfitContext = Outfit;
		Def.bRequiresOutfitMatch = Outfit == EApartmentLifeOutfitContext::Athletic || Outfit == EApartmentLifeOutfitContext::Sleep;
		if (Id.ToString().Contains(TEXT("work.")))
		{
			Def.bUsesIncomeFormula = true;
		}
		if (Id.ToString().Contains(TEXT("dress")) || Id.ToString().Contains(TEXT("wardrobe")))
		{
			Def.CameraMode = EApartmentLifePrimaryCameraMode::Wardrobe;
		}
		return Def;
	}

	FApartmentLifeRoutineChainDefinition MakeChain(FName Id, const TCHAR* Name, EApartmentLifeRoutineChainType Type, std::initializer_list<FName> Steps)
	{
		FApartmentLifeRoutineChainDefinition Chain;
		Chain.ChainId = Id;
		Chain.DisplayName = FText::FromString(Name);
		Chain.ChainType = Type;
		Chain.ActivityIds = Steps;
		return Chain;
	}
}

TArray<FApartmentLifeActivityDefinition> ApartmentLifeActivityContentPack01::GetExpansionActivities()
{
	return {
		MakeActivity(TEXT("activity.dress.preview"), TEXT("Outfit Preview"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::WalkInCloset, EApartmentLifeAnimationGroup::Dressing, 8, Stats(2, 0, 0, 0, 2, 5, 0)),
		MakeActivity(TEXT("activity.groom.hair_adjust"), TEXT("Hair Adjustment"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 5, Stats(1, 0, 2, 0, 1, 3, 0)),
		MakeActivity(TEXT("activity.sleep.wake"), TEXT("Wake Up"), EApartmentLifeActivityCategory::Sleep, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::WakeUp, 10, Stats(2, 15, 0, 0, 3, 1, -5), EApartmentLifeOutfitContext::Sleep),
		MakeActivity(TEXT("activity.relax.journal"), TEXT("Journal Writing"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Reading, 20, Stats(4, -1, 0, 0, 5, 2, -3)),
		MakeActivity(TEXT("activity.work.focus"), TEXT("Focus Work Session"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 90, Stats(1, -9, 0, 4, -1, 3, 4), EApartmentLifeOutfitContext::Work),
		MakeActivity(TEXT("activity.work.creative"), TEXT("Creative Work Session"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 75, Stats(4, -7, 0, 3, 2, 4, 2), EApartmentLifeOutfitContext::Work),
		MakeActivity(TEXT("activity.fitness.yoga.warrior"), TEXT("Warrior Pose"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 20, Stats(4, -4, 1, 0, 4, 3, -7), EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.fitness.yoga.tree"), TEXT("Tree Pose"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 15, Stats(3, -3, 1, 0, 4, 3, -6), EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.fitness.yoga.cobra"), TEXT("Cobra Pose"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 15, Stats(3, -3, 1, 0, 3, 2, -7), EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.fitness.yoga.child"), TEXT("Child's Pose"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 12, Stats(5, 2, 0, 0, 6, 2, -12), EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.clean.tidy"), TEXT("Tidy Up"), EApartmentLifeActivityCategory::Cleaning, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Cleaning, 25, Stats(1, -4, 0, 0, 6, 2, -3))
	};
}

TArray<FApartmentLifeRoutineChainDefinition> ApartmentLifeActivityContentPack01::GetExpansionRoutineChains()
{
	return {
		// Morning variants
		MakeChain(TEXT("routine.morning.quick"), TEXT("Quick Morning"), EApartmentLifeRoutineChainType::Morning, {
			FName(TEXT("activity.hygiene.brush_teeth")), FName(TEXT("activity.dress.change")), FName(TEXT("activity.cook.breakfast"))
		}),
		MakeChain(TEXT("routine.morning.relaxed"), TEXT("Relaxed Morning"), EApartmentLifeRoutineChainType::Morning, {
			FName(TEXT("activity.sleep.wake")), FName(TEXT("activity.groom.wash_face")), FName(TEXT("activity.cook.breakfast")), FName(TEXT("activity.relax.sofa"))
		}),
		MakeChain(TEXT("routine.morning.productive"), TEXT("Productive Morning"), EApartmentLifeRoutineChainType::Morning, {
			FName(TEXT("activity.hygiene.shower")), FName(TEXT("activity.groom.makeup")), FName(TEXT("activity.dress.wardrobe")), FName(TEXT("activity.work.focus"))
		}),
		// Work variants
		MakeChain(TEXT("routine.work.focus"), TEXT("Focus Work Session"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.work.focus")), FName(TEXT("activity.cook.snack")), FName(TEXT("activity.fitness.stretch"))
		}),
		MakeChain(TEXT("routine.work.creative"), TEXT("Creative Work Session"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.work.creative")), FName(TEXT("activity.cook.drink")), FName(TEXT("activity.relax.sofa"))
		}),
		MakeChain(TEXT("routine.work.freelance_short"), TEXT("Short Freelance Task"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.work.freelance")), FName(TEXT("activity.finance.budget"))
		}),
		MakeChain(TEXT("routine.work.project_long"), TEXT("Long Project Session"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.work.programming")), FName(TEXT("activity.cook.lunch")), FName(TEXT("activity.work.focus")), FName(TEXT("activity.fitness.stretch"))
		}),
		// Self-care
		MakeChain(TEXT("routine.selfcare.quick"), TEXT("Quick Grooming"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.groom.wash_face")), FName(TEXT("activity.groom.hair_adjust"))
		}),
		MakeChain(TEXT("routine.selfcare.full"), TEXT("Full Grooming"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.hygiene.shower")), FName(TEXT("activity.groom.hair")), FName(TEXT("activity.groom.makeup")), FName(TEXT("activity.groom.mirror_check"))
		}),
		MakeChain(TEXT("routine.selfcare.skincare"), TEXT("Skin Care Routine"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.groom.wash_face")), FName(TEXT("activity.groom.skincare")), FName(TEXT("activity.groom.mirror_check"))
		}),
		MakeChain(TEXT("routine.selfcare.hair"), TEXT("Hair Care Routine"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.groom.hair")), FName(TEXT("activity.groom.hair_adjust")), FName(TEXT("activity.groom.mirror_check"))
		}),
		MakeChain(TEXT("routine.selfcare.mirror"), TEXT("Mirror Check"), EApartmentLifeRoutineChainType::Custom, {
			FName(TEXT("activity.groom.mirror_check")), FName(TEXT("activity.dress.preview"))
		}),
		// Evening variants
		MakeChain(TEXT("routine.evening.cozy"), TEXT("Cozy Evening"), EApartmentLifeRoutineChainType::Evening, {
			FName(TEXT("activity.cook.dinner")), FName(TEXT("activity.relax.sofa")), FName(TEXT("activity.entertainment.music")), FName(TEXT("activity.groom.skincare"))
		}),
		MakeChain(TEXT("routine.evening.reading"), TEXT("Reading Night"), EApartmentLifeRoutineChainType::Evening, {
			FName(TEXT("activity.cook.snack")), FName(TEXT("activity.read.book")), FName(TEXT("activity.relax.sit"))
		}),
		MakeChain(TEXT("routine.evening.tv"), TEXT("TV Night"), EApartmentLifeRoutineChainType::Evening, {
			FName(TEXT("activity.cook.dinner")), FName(TEXT("activity.entertainment.tv")), FName(TEXT("activity.relax.sofa"))
		}),
		MakeChain(TEXT("routine.evening.early_sleep"), TEXT("Early Sleep"), EApartmentLifeRoutineChainType::Evening, {
			FName(TEXT("activity.cook.dinner")), FName(TEXT("activity.groom.skincare")), FName(TEXT("activity.dress.change")), FName(TEXT("activity.sleep.bed"))
		}),
		MakeChain(TEXT("routine.evening.late_work"), TEXT("Late Work Night"), EApartmentLifeRoutineChainType::Evening, {
			FName(TEXT("activity.cook.snack")), FName(TEXT("activity.work.focus")), FName(TEXT("activity.cook.drink")), FName(TEXT("activity.sleep.nap"))
		})
	};
}

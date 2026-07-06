// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityCatalogLibrary.h"

namespace
{
	FApartmentLifeActivityDefinition MakeActivity(
		FName Id,
		const TCHAR* Name,
		EApartmentLifeActivityCategory Category,
		EApartmentLifeActivityRoom Room,
		EApartmentLifeAnimationGroup Anim,
		int32 Duration,
		const FApartmentLifeActivityStatEffect& Stats,
		float Money = 0.f,
		bool bIncome = false,
		bool bPrivacy = false,
		EApartmentLifeOutfitContext Outfit = EApartmentLifeOutfitContext::Everyday,
		int32 Cooldown = 0)
	{
		FApartmentLifeActivityDefinition Def;
		Def.ActivityId = Id;
		Def.DisplayName = FText::FromString(Name);
		Def.Category = Category;
		Def.RequiredRoom = Room;
		Def.AnimationGroup = Anim;
		Def.DurationMinutes = Duration;
		Def.StatEffects = Stats;
		Def.MoneyEffect = Money;
		Def.bUsesIncomeFormula = bIncome;
		Def.bPrivacyFraming = bPrivacy;
		Def.RequiredOutfitContext = Outfit;
		Def.bRequiresOutfitMatch = Outfit == EApartmentLifeOutfitContext::Athletic || Outfit == EApartmentLifeOutfitContext::Sleep;
		if (bPrivacy) Def.CameraFrame.bPrivacyFraming = true;
		if (Id.ToString().Contains(TEXT("dress")) || Id.ToString().Contains(TEXT("wardrobe")))
		{
			Def.CameraMode = EApartmentLifePrimaryCameraMode::Wardrobe;
		}
		Def.CooldownMinutes = Cooldown;
		return Def;
	}

	FApartmentLifeActivityStatEffect Stats(float Mood = 0, float Energy = 0, float Hygiene = 0, float Hunger = 0, float Comfort = 0, float Confidence = 0, float Stress = 0, float Affection = 0)
	{
		FApartmentLifeActivityStatEffect S;
		S.MoodDelta = Mood;
		S.EnergyDelta = Energy;
		S.HygieneDelta = Hygiene;
		S.HungerDelta = Hunger;
		S.ComfortDelta = Comfort;
		S.ConfidenceDelta = Confidence;
		S.StressDelta = Stress;
		S.AffectionDelta = Affection;
		return S;
	}
}

TArray<FApartmentLifeActivityDefinition> UApartmentLifeActivityCatalogLibrary::GetBuiltinCatalog()
{
	return {
		// Bedroom
		MakeActivity(TEXT("activity.sleep.bed"), TEXT("Sleep"), EApartmentLifeActivityCategory::Sleep, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Sleeping, 480, Stats(0, 60, 0, 0, 5, 0, -50), 0, false, false, EApartmentLifeOutfitContext::Sleep),
		MakeActivity(TEXT("activity.sleep.nap"), TEXT("Nap"), EApartmentLifeActivityCategory::Sleep, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Sleeping, 60, Stats(2, 25, 0, 0, 8, 0, -20), 0, false, false, EApartmentLifeOutfitContext::Sleep),
		MakeActivity(TEXT("activity.relax.sit"), TEXT("Sit on Bed"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Sitting, 15, Stats(2, 0, 0, 0, 5, 0, -2)),
		MakeActivity(TEXT("activity.read.book"), TEXT("Read"), EApartmentLifeActivityCategory::Reading, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Reading, 30, Stats(3, -2, 0, 0, 6, 1, -3), 0, false, false, EApartmentLifeOutfitContext::Everyday),
		MakeActivity(TEXT("activity.relax.sofa"), TEXT("Relax on Sofa"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::CouchSitting, 30, Stats(4, 0, 0, 0, 10, 0, -5)),
		MakeActivity(TEXT("activity.phone.browse"), TEXT("Phone Browsing"), EApartmentLifeActivityCategory::Entertainment, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Sitting, 20, Stats(2, -3, 0, 0, 3, 0, 1)),
		MakeActivity(TEXT("activity.plan.tomorrow"), TEXT("Plan Tomorrow"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Bedroom, EApartmentLifeAnimationGroup::Sitting, 15, Stats(1, -2, 0, 0, 2, 3, -2)),

		// Bathroom
		MakeActivity(TEXT("activity.hygiene.shower"), TEXT("Shower Routine"), EApartmentLifeActivityCategory::Hygiene, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::ShowerRoutine, 20, Stats(5, -3, 35, 0, 8, 2, -4), 0, false, true),
		MakeActivity(TEXT("activity.hygiene.brush_teeth"), TEXT("Brush Teeth"), EApartmentLifeActivityCategory::Hygiene, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 5, Stats(1, 0, 12, 0, 1, 1, 0)),
		MakeActivity(TEXT("activity.groom.wash_face"), TEXT("Wash Face"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 5, Stats(1, 0, 8, 0, 2, 2, 0)),
		MakeActivity(TEXT("activity.groom.hair"), TEXT("Hair Care"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 10, Stats(2, 0, 5, 0, 2, 4, -1)),
		MakeActivity(TEXT("activity.groom.skincare"), TEXT("Skin Care"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 10, Stats(2, 0, 5, 0, 3, 3, -1)),
		MakeActivity(TEXT("activity.groom.makeup"), TEXT("Makeup"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Grooming, 15, Stats(3, -2, 3, 0, 2, 6, 1)),
		MakeActivity(TEXT("activity.groom.mirror"), TEXT("Mirror Grooming"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::MirrorRoutine, 15, Stats(2, 0, 10, 0, 2, 6, 0)),
		MakeActivity(TEXT("activity.groom.mirror_check"), TEXT("Mirror Check"), EApartmentLifeActivityCategory::Grooming, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::MirrorRoutine, 5, Stats(1, 0, 2, 0, 1, 3, 0)),
		MakeActivity(TEXT("activity.chores.laundry_hamper"), TEXT("Laundry Hamper"), EApartmentLifeActivityCategory::Laundry, EApartmentLifeActivityRoom::Bathroom, EApartmentLifeAnimationGroup::Laundry, 10, Stats(0, -2, 0, 0, 2, 0, 0)),

		// Wardrobe / closet
		MakeActivity(TEXT("activity.dress.wardrobe"), TEXT("Open Wardrobe"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::WalkInCloset, EApartmentLifeAnimationGroup::Dressing, 10, Stats(2, 0, 0, 0, 3, 4, 0)),
		MakeActivity(TEXT("activity.dress.change"), TEXT("Change Outfit"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::WalkInCloset, EApartmentLifeAnimationGroup::Dressing, 8, Stats(1, 0, 0, 0, 2, 3, 0)),
		MakeActivity(TEXT("activity.dress.save_preset"), TEXT("Save Outfit Preset"), EApartmentLifeActivityCategory::Relaxing, EApartmentLifeActivityRoom::WalkInCloset, EApartmentLifeAnimationGroup::Dressing, 5, Stats(1, 0, 0, 0, 2, 2, 0)),
		MakeActivity(TEXT("activity.dress.organize"), TEXT("Organize Closet"), EApartmentLifeActivityCategory::Cleaning, EApartmentLifeActivityRoom::WalkInCloset, EApartmentLifeAnimationGroup::Dressing, 20, Stats(1, -3, 0, 0, 4, 2, -2)),
		MakeActivity(TEXT("activity.chores.laundry"), TEXT("Do Laundry"), EApartmentLifeActivityCategory::Laundry, EApartmentLifeActivityRoom::LaundryRoom, EApartmentLifeAnimationGroup::Laundry, 30, Stats(0, -5, 0, 0, 5, 1, 0)),

		// Desk / computer
		MakeActivity(TEXT("activity.work.computer"), TEXT("Remote Work"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 120, Stats(0, -10, 0, 5, -2, 2, 5), 0, true),
		MakeActivity(TEXT("activity.work.freelance"), TEXT("Freelance Task"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 90, Stats(1, -8, 0, 5, -1, 3, 4), 0, true),
		MakeActivity(TEXT("activity.work.digital_art"), TEXT("Digital Art"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 60, Stats(4, -6, 0, 3, 2, 4, 2), 0, true),
		MakeActivity(TEXT("activity.work.programming"), TEXT("Programming"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 90, Stats(1, -8, 0, 5, -1, 3, 3), 0, true),
		MakeActivity(TEXT("activity.work.tutoring"), TEXT("Online Tutoring"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 60, Stats(2, -6, 0, 3, 0, 4, 2), 0, true),
		MakeActivity(TEXT("activity.study.read"), TEXT("Study / Read"), EApartmentLifeActivityCategory::Reading, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 60, Stats(2, -5, 0, 2, 1, 2, 1)),
		MakeActivity(TEXT("activity.finance.budget"), TEXT("Budget Review"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 20, Stats(0, -2, 0, 0, 1, 3, 3)),
		MakeActivity(TEXT("activity.shop.online"), TEXT("Online Shopping"), EApartmentLifeActivityCategory::Shopping, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 25, Stats(3, -3, 0, 0, 2, 2, 2)),
		MakeActivity(TEXT("activity.skill.practice"), TEXT("Skill Practice"), EApartmentLifeActivityCategory::Work, EApartmentLifeActivityRoom::Office, EApartmentLifeAnimationGroup::ComputerUse, 45, Stats(2, -4, 0, 2, 1, 3, 1)),

		// Kitchen
		MakeActivity(TEXT("activity.cook.breakfast"), TEXT("Cook Breakfast"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 30, Stats(2, -3, 0, 0, 4, 1, -1)),
		MakeActivity(TEXT("activity.cook.lunch"), TEXT("Cook Lunch"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 35, Stats(2, -4, 0, 0, 4, 1, -1)),
		MakeActivity(TEXT("activity.cook.dinner"), TEXT("Cook Dinner"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 45, Stats(3, -5, 0, 0, 5, 2, -2)),
		MakeActivity(TEXT("activity.cook.prepare"), TEXT("Cook Meal"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 40, Stats(2, -4, 0, 0, 4, 1, -1)),
		MakeActivity(TEXT("activity.cook.snack"), TEXT("Make Snack"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 10, Stats(1, 2, 0, -15, 3, 0, -1)),
		MakeActivity(TEXT("activity.cook.drink"), TEXT("Make Drink"), EApartmentLifeActivityCategory::Cooking, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 5, Stats(1, 3, 0, -5, 2, 0, -1)),
		MakeActivity(TEXT("activity.eat.meal"), TEXT("Eat Meal"), EApartmentLifeActivityCategory::Eating, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Eating, 20, Stats(3, 5, 0, -35, 6, 0, -2)),
		MakeActivity(TEXT("activity.clean.dishes"), TEXT("Clean Dishes"), EApartmentLifeActivityCategory::Cleaning, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cleaning, 15, Stats(0, -3, 0, 0, 2, 1, -2)),
		MakeActivity(TEXT("activity.grocery.check"), TEXT("Grocery Check"), EApartmentLifeActivityCategory::Shopping, EApartmentLifeActivityRoom::Kitchen, EApartmentLifeAnimationGroup::Cooking, 10, Stats(0, -1, 0, 0, 1, 0, 1)),

		// Living room
		MakeActivity(TEXT("activity.entertainment.tv"), TEXT("Watch TV"), EApartmentLifeActivityCategory::Entertainment, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::WatchingTV, 30, Stats(4, -2, 0, 5, 8, 0, -5)),
		MakeActivity(TEXT("activity.entertainment.music"), TEXT("Listen to Music"), EApartmentLifeActivityCategory::Entertainment, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Sitting, 25, Stats(5, 0, 0, 0, 6, 1, -4)),
		MakeActivity(TEXT("activity.entertainment.game"), TEXT("Play Game"), EApartmentLifeActivityCategory::Entertainment, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Gaming, 45, Stats(6, -5, 0, 8, 5, 1, -3)),
		MakeActivity(TEXT("activity.social.talk"), TEXT("Talk"), EApartmentLifeActivityCategory::Social, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Conversation, 20, Stats(3, -2, 0, 0, 4, 2, -3, 2)),
		MakeActivity(TEXT("activity.fitness.stretch"), TEXT("Stretch"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Stretching, 15, Stats(2, 3, 0, 0, 4, 2, -4), 0, false, false, EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.decorate.room"), TEXT("Decorate"), EApartmentLifeActivityCategory::Decorating, EApartmentLifeActivityRoom::LivingRoom, EApartmentLifeAnimationGroup::Cleaning, 30, Stats(4, -4, 0, 0, 6, 3, -2)),

		// Yoga / fitness
		[]() {
			FApartmentLifeActivityDefinition Def = MakeActivity(TEXT("activity.fitness.yoga"), TEXT("Yoga Session"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 30, Stats(5, -5, 2, 0, 5, 4, -8), 0, false, false, EApartmentLifeOutfitContext::Athletic, 15);
			Def.StatEffects.FitnessXPDelta = 5.f;
			Def.StatEffects.FlexibilityXPDelta = 3.f;
			return Def;
		}(),
		MakeActivity(TEXT("activity.fitness.exercise"), TEXT("Exercise"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Exercising, 25, Stats(3, -8, 3, 0, 3, 3, -6), 0, false, false, EApartmentLifeOutfitContext::Athletic),
		MakeActivity(TEXT("activity.fitness.breathe"), TEXT("Breathing Exercise"), EApartmentLifeActivityCategory::Fitness, EApartmentLifeActivityRoom::YogaArea, EApartmentLifeAnimationGroup::Yoga, 10, Stats(4, 2, 0, 0, 5, 2, -10), 0, false, false, EApartmentLifeOutfitContext::Athletic, 5),

		// Dining
		MakeActivity(TEXT("activity.eat.dining"), TEXT("Dine"), EApartmentLifeActivityCategory::Eating, EApartmentLifeActivityRoom::DiningRoom, EApartmentLifeAnimationGroup::Eating, 25, Stats(3, 3, 0, -30, 7, 1, -2))
	};
}

bool UApartmentLifeActivityCatalogLibrary::TryGetActivityDefinition(FName ActivityId, FApartmentLifeActivityDefinition& OutDefinition)
{
	for (const FApartmentLifeActivityDefinition& Def : GetBuiltinCatalog())
	{
		if (Def.ActivityId == ActivityId)
		{
			OutDefinition = Def;
			return true;
		}
	}
	return false;
}

TArray<FApartmentLifeRoutineChainDefinition> UApartmentLifeActivityCatalogLibrary::GetBuiltinRoutineChains()
{
	FApartmentLifeRoutineChainDefinition Morning;
	Morning.ChainId = FName(TEXT("routine.morning"));
	Morning.DisplayName = FText::FromString(TEXT("Morning Routine"));
	Morning.ChainType = EApartmentLifeRoutineChainType::Morning;
	Morning.ActivityIds = {
		FName(TEXT("activity.hygiene.shower")),
		FName(TEXT("activity.hygiene.brush_teeth")),
		FName(TEXT("activity.dress.wardrobe")),
		FName(TEXT("activity.cook.breakfast")),
		FName(TEXT("activity.work.computer"))
	};

	FApartmentLifeRoutineChainDefinition Evening;
	Evening.ChainId = FName(TEXT("routine.evening"));
	Evening.DisplayName = FText::FromString(TEXT("Evening Routine"));
	Evening.ChainType = EApartmentLifeRoutineChainType::Evening;
	Evening.ActivityIds = {
		FName(TEXT("activity.cook.dinner")),
		FName(TEXT("activity.relax.sofa")),
		FName(TEXT("activity.dress.change")),
		FName(TEXT("activity.groom.skincare")),
		FName(TEXT("activity.read.book")),
		FName(TEXT("activity.sleep.bed"))
	};

	return { Morning, Evening };
}

bool UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(FName ChainId, FApartmentLifeRoutineChainDefinition& OutChain)
{
	for (const FApartmentLifeRoutineChainDefinition& Chain : GetBuiltinRoutineChains())
	{
		if (Chain.ChainId == ChainId)
		{
			OutChain = Chain;
			return true;
		}
	}
	return false;
}

TArray<FApartmentLifeActivityDefinition> UApartmentLifeActivityCatalogLibrary::GetActivitiesForRoom(EApartmentLifeActivityRoom Room)
{
	TArray<FApartmentLifeActivityDefinition> Results;
	for (const FApartmentLifeActivityDefinition& Def : GetBuiltinCatalog())
	{
		if (Def.RequiredRoom == Room || Def.RequiredRoom == EApartmentLifeActivityRoom::Any)
		{
			Results.Add(Def);
		}
	}
	return Results;
}

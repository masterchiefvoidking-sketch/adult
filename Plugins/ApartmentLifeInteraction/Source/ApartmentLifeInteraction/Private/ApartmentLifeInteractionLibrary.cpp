// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInteractionLibrary.h"
#include "ApartmentLifeActivityLibrary.h"

FName UApartmentLifeInteractionLibrary::GetDefaultActivityForInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	switch (Interaction)
	{
	case EApartmentLifeFurnitureInteraction::Sleep: return FName(TEXT("activity.sleep.bed"));
	case EApartmentLifeFurnitureInteraction::Nap: return FName(TEXT("activity.sleep.nap"));
	case EApartmentLifeFurnitureInteraction::Sit: return FName(TEXT("activity.relax.sit"));
	case EApartmentLifeFurnitureInteraction::Read: return FName(TEXT("activity.read.book"));
	case EApartmentLifeFurnitureInteraction::WatchTV: return FName(TEXT("activity.entertainment.tv"));
	case EApartmentLifeFurnitureInteraction::Talk: return FName(TEXT("activity.social.talk"));
	case EApartmentLifeFurnitureInteraction::Relax: return FName(TEXT("activity.relax.sofa"));
	case EApartmentLifeFurnitureInteraction::UseComputer: return FName(TEXT("activity.work.computer"));
	case EApartmentLifeFurnitureInteraction::Study: return FName(TEXT("activity.study.read"));
	case EApartmentLifeFurnitureInteraction::WorkFromHome: return FName(TEXT("activity.work.computer"));
	case EApartmentLifeFurnitureInteraction::Groom: return FName(TEXT("activity.groom.mirror"));
	case EApartmentLifeFurnitureInteraction::CheckOutfit: return FName(TEXT("activity.groom.mirror_check"));
	case EApartmentLifeFurnitureInteraction::ChangeOutfit: return FName(TEXT("activity.dress.change"));
	case EApartmentLifeFurnitureInteraction::ManageWardrobe: return FName(TEXT("activity.dress.wardrobe"));
	case EApartmentLifeFurnitureInteraction::Cook: return FName(TEXT("activity.cook.prepare"));
	case EApartmentLifeFurnitureInteraction::Eat: return FName(TEXT("activity.eat.meal"));
	case EApartmentLifeFurnitureInteraction::CleanDishes: return FName(TEXT("activity.clean.dishes"));
	case EApartmentLifeFurnitureInteraction::Shower: return FName(TEXT("activity.hygiene.shower"));
	case EApartmentLifeFurnitureInteraction::BrushTeeth: return FName(TEXT("activity.hygiene.brush_teeth"));
	case EApartmentLifeFurnitureInteraction::SkinCare: return FName(TEXT("activity.groom.skincare"));
	case EApartmentLifeFurnitureInteraction::HairCare: return FName(TEXT("activity.groom.hair"));
	case EApartmentLifeFurnitureInteraction::Yoga: return FName(TEXT("activity.fitness.yoga"));
	case EApartmentLifeFurnitureInteraction::Stretch: return FName(TEXT("activity.fitness.stretch"));
	case EApartmentLifeFurnitureInteraction::Exercise: return FName(TEXT("activity.fitness.exercise"));
	case EApartmentLifeFurnitureInteraction::DoLaundry: return FName(TEXT("activity.chores.laundry"));
	case EApartmentLifeFurnitureInteraction::PlayGame: return FName(TEXT("activity.entertainment.game"));
	case EApartmentLifeFurnitureInteraction::ListenMusic: return FName(TEXT("activity.entertainment.music"));
	case EApartmentLifeFurnitureInteraction::MakeSnack: return FName(TEXT("activity.cook.snack"));
	case EApartmentLifeFurnitureInteraction::PhoneBrowse: return FName(TEXT("activity.phone.browse"));
	case EApartmentLifeFurnitureInteraction::Freelance: return FName(TEXT("activity.work.freelance"));
	case EApartmentLifeFurnitureInteraction::BudgetReview: return FName(TEXT("activity.finance.budget"));
	case EApartmentLifeFurnitureInteraction::OrganizeCloset: return FName(TEXT("activity.dress.organize"));
	case EApartmentLifeFurnitureInteraction::Decorate: return FName(TEXT("activity.decorate.room"));
	case EApartmentLifeFurnitureInteraction::Breathe: return FName(TEXT("activity.fitness.breathe"));
	case EApartmentLifeFurnitureInteraction::GroceryCheck: return FName(TEXT("activity.grocery.check"));
	default: return FName(TEXT("activity.idle"));
	}
}

EApartmentLifeFurnitureInteraction UApartmentLifeInteractionLibrary::GetPrimaryInteraction(const TArray<EApartmentLifeFurnitureInteraction>& Interactions)
{
	return Interactions.Num() > 0 ? Interactions[0] : EApartmentLifeFurnitureInteraction::Sit;
}

FText UApartmentLifeInteractionLibrary::GetDisplayNameForInteraction(EApartmentLifeFurnitureInteraction Interaction)
{
	return BuildDescriptor(Interaction).DisplayName;
}

int32 UApartmentLifeInteractionLibrary::GetDurationMinutesForActivity(FName ActivityId)
{
	return UApartmentLifeActivityLibrary::GetDurationMinutes(ActivityId);
}

FApartmentLifeInteractionDescriptor UApartmentLifeInteractionLibrary::BuildDescriptor(EApartmentLifeFurnitureInteraction Interaction)
{
	FApartmentLifeInteractionDescriptor Desc;
	Desc.InteractionType = Interaction;
	Desc.InteractionId = FName(*FString::Printf(TEXT("interaction.%d"), static_cast<int32>(Interaction)));
	Desc.ActivityId = GetDefaultActivityForInteraction(Interaction);
	Desc.DurationMinutes = GetDurationMinutesForActivity(Desc.ActivityId);

	switch (Interaction)
	{
	case EApartmentLifeFurnitureInteraction::Shower:
		Desc.DisplayName = FText::FromString(TEXT("Shower Routine"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Hygiene +35, Mood +5"));
		Desc.FocusOffset = FVector(0.f, 0.f, 80.f);
		break;
	case EApartmentLifeFurnitureInteraction::UseComputer:
	case EApartmentLifeFurnitureInteraction::WorkFromHome:
		Desc.DisplayName = FText::FromString(TEXT("Remote Work"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Income +variable, Energy -10, Stress +5"));
		Desc.FocusOffset = FVector(0.f, 40.f, 60.f);
		break;
	case EApartmentLifeFurnitureInteraction::Yoga:
		Desc.DisplayName = FText::FromString(TEXT("Yoga"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Mood +10, Energy -5, Fitness XP"));
		break;
	case EApartmentLifeFurnitureInteraction::Sleep:
		Desc.DisplayName = FText::FromString(TEXT("Sleep"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Energy +60, Fatigue -50"));
		break;
	case EApartmentLifeFurnitureInteraction::Groom:
		Desc.DisplayName = FText::FromString(TEXT("Groom"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Hygiene +10, Confidence +6"));
		break;
	case EApartmentLifeFurnitureInteraction::ManageWardrobe:
		Desc.DisplayName = FText::FromString(TEXT("Open Wardrobe"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Confidence +4"));
		break;
	case EApartmentLifeFurnitureInteraction::PlayGame:
		Desc.DisplayName = FText::FromString(TEXT("Play Game"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Mood +6, Fun"));
		break;
	case EApartmentLifeFurnitureInteraction::Freelance:
		Desc.DisplayName = FText::FromString(TEXT("Freelance Task"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Income +variable"));
		break;
	case EApartmentLifeFurnitureInteraction::Breathe:
		Desc.DisplayName = FText::FromString(TEXT("Breathing Exercise"));
		Desc.StatEffectSummary = FText::FromString(TEXT("Stress -10, Mood +4"));
		break;
	default:
		Desc.DisplayName = FText::FromString(UEnum::GetDisplayValueAsText(Interaction).ToString());
		Desc.StatEffectSummary = FText::FromString(TEXT("Routine activity"));
		break;
	}

	return Desc;
}

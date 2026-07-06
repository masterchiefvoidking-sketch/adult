// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInteractionLibrary.h"

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
	case EApartmentLifeFurnitureInteraction::WorkFromHome: return FName(TEXT("activity.work.remote"));
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
	default: return FName(TEXT("activity.idle"));
	}
}

EApartmentLifeFurnitureInteraction UApartmentLifeInteractionLibrary::GetPrimaryInteraction(const TArray<EApartmentLifeFurnitureInteraction>& Interactions)
{
	return Interactions.Num() > 0 ? Interactions[0] : EApartmentLifeFurnitureInteraction::Sit;
}

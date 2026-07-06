// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDebugMenuComponent.h"
#include "ApartmentLifeDevToolsLibrary.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

UApartmentLifeDebugMenuComponent::UApartmentLifeDebugMenuComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UApartmentLifeDebugMenuComponent::InitializeContext(
	APlayerController* InController,
	AApartmentLifeApartmentUnit* InApartment,
	AActor* InGirlCharacter)
{
	OwnerController = InController;
	ApartmentUnit = InApartment;
	GirlCharacter = InGirlCharacter;
}

void UApartmentLifeDebugMenuComponent::ToggleMenu()
{
	bMenuOpen = !bMenuOpen;
	if (bMenuOpen)
	{
		ShowMenuOverlay();
	}
}

AActor* UApartmentLifeDebugMenuComponent::GetGirlActor() const
{
	return GirlCharacter.Get();
}

void UApartmentLifeDebugMenuComponent::ShowMenuOverlay() const
{
	if (!GEngine)
	{
		return;
	}

	const FString Summary = UApartmentLifeDevToolsLibrary::BuildCharacterDebugSummary(GetGirlActor());
	const FString Help = TEXT(
		"[F1] Close | M +$500 | H +1hr | Shift+H 8am | [/] Mood | ;' Energy | -= Hygiene\n"
		"O Outfit | 3 Bed | 4 Bath | 5 Living | 6 Kitchen | 7 Office | Y Yoga | S Save | L Load | R Reset");

	GEngine->AddOnScreenDebugMessage(9001, 8.f, FColor::Cyan, TEXT("=== Apartment Life Debug (F1) ==="));
	GEngine->AddOnScreenDebugMessage(9002, 8.f, FColor::White, Summary);
	GEngine->AddOnScreenDebugMessage(9003, 8.f, FColor::Yellow, Help);
}

void UApartmentLifeDebugMenuComponent::HandleDebugAction(FName ActionName)
{
	if (!bMenuOpen)
	{
		return;
	}

	AActor* Girl = GetGirlActor();
	APlayerController* Controller = OwnerController.Get();
	const FString Action = ActionName.ToString();

	if (Action == TEXT("AddMoney"))
	{
		UApartmentLifeDevToolsLibrary::AddMoney(Controller, Girl, 500.f);
	}
	else if (Action == TEXT("AdvanceHour"))
	{
		if (UWorld* World = GetWorld())
		{
			if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				FApartmentLifeGameTime Time = TimeSubsystem->GetCurrentTime();
				Time.Hour = (Time.Hour + 1) % 24;
				TimeSubsystem->SetCurrentTime(Time);
			}
		}
	}
	else if (Action == TEXT("SetMorning"))
	{
		UApartmentLifeDevToolsLibrary::SetGameHour(Controller, 8);
	}
	else if (Action == TEXT("MoodUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetMood(Girl, Sim->GetMood().OverallMood + 10.f);
		}
	}
	else if (Action == TEXT("MoodDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetMood(Girl, Sim->GetMood().OverallMood - 10.f);
		}
	}
	else if (Action == TEXT("EnergyUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetEnergy(Girl, Sim->GetMood().Energy + 10.f);
		}
	}
	else if (Action == TEXT("EnergyDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetEnergy(Girl, Sim->GetMood().Energy - 10.f);
		}
	}
	else if (Action == TEXT("HygieneUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetHygiene(Girl, Sim->Needs.Hygiene + 10.f);
		}
	}
	else if (Action == TEXT("HygieneDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetHygiene(Girl, Sim->Needs.Hygiene - 10.f);
		}
	}
	else if (Action == TEXT("CycleOutfit"))
	{
		static const EApartmentLifeOutfitContext Contexts[] = {
			EApartmentLifeOutfitContext::Everyday,
			EApartmentLifeOutfitContext::Work,
			EApartmentLifeOutfitContext::Lounge,
			EApartmentLifeOutfitContext::Sleep,
			EApartmentLifeOutfitContext::Athletic,
			EApartmentLifeOutfitContext::Formal
		};
		OutfitContextIndex = (OutfitContextIndex + 1) % UE_ARRAY_COUNT(Contexts);
		UApartmentLifeDevToolsLibrary::EquipOutfitContext(Girl, Contexts[OutfitContextIndex]);
	}
	else if (Action == TEXT("TeleportBedroom"))
	{
		UApartmentLifeDevToolsLibrary::TeleportToRoom(Girl, EApartmentLifeRoomType::Bedroom);
	}
	else if (Action == TEXT("TeleportBathroom"))
	{
		UApartmentLifeDevToolsLibrary::TeleportToRoom(Girl, EApartmentLifeRoomType::Bathroom);
	}
	else if (Action == TEXT("TeleportLivingRoom"))
	{
		UApartmentLifeDevToolsLibrary::TeleportToRoom(Girl, EApartmentLifeRoomType::LivingRoom);
	}
	else if (Action == TEXT("TeleportKitchen"))
	{
		UApartmentLifeDevToolsLibrary::TeleportToRoom(Girl, EApartmentLifeRoomType::Kitchen);
	}
	else if (Action == TEXT("TeleportOffice"))
	{
		UApartmentLifeDevToolsLibrary::TeleportToRoom(Girl, EApartmentLifeRoomType::Office);
	}
	else if (Action == TEXT("TriggerYoga"))
	{
		UApartmentLifeDevToolsLibrary::TriggerActivity(Girl, FName(TEXT("activity.fitness.yoga")));
	}
	else if (Action == TEXT("SaveNow"))
	{
		UApartmentLifeDevToolsLibrary::SaveTest(Controller, DebugSaveSlot);
	}
	else if (Action == TEXT("LoadNow"))
	{
		UApartmentLifeDevToolsLibrary::LoadTest(Controller, DebugSaveSlot);
	}
	else if (Action == TEXT("ResetApartment"))
	{
		UApartmentLifeDevToolsLibrary::ResetApartmentLayout(ApartmentUnit.Get());
	}

	ShowMenuOverlay();
}

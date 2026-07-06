// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDebugMenuComponent.h"
#include "ApartmentLifeDevToolsLibrary.h"
#include "ApartmentLifeDeveloperSubsystem.h"
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
		"[F1] Close | M +$500 | Shift+M -$200 | H +1hr | Shift+H 8am\n"
		"[/] Mood (creator: adjust slider) | ;' Energy | -= Hygiene | ,. Stress | <> Hunger | \\ / Comfort\n"
		"O Outfit | U Unlock clothes | I Unlock shop | X Complete activity | Y Yoga\n"
		"Shift+P Print creator | Shift+F Reset creator face | Shift+B Reset creator body\n"
		"Shift+A Reset animation | Shift+I Show IK targets | Shift+O Show furniture markers\n"
		"3-7 Teleport rooms | S Save | L Load | Shift+S Delete | Shift+L Clear saves\n"
		"R Reset apartment | Shift+R Reset character | F4 Spawn desk");

	GEngine->AddOnScreenDebugMessage(9001, 8.f, FColor::Cyan, TEXT("=== Apartment Life Debug (F1) ==="));
	GEngine->AddOnScreenDebugMessage(9002, 8.f, FColor::White, Summary);
	GEngine->AddOnScreenDebugMessage(9003, 8.f, FColor::Yellow, Help);
}

void UApartmentLifeDebugMenuComponent::HandleDebugAction(FName ActionName)
{
	bool bAllowActions = bMenuOpen;
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (const UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
			{
				bAllowActions = bAllowActions || Dev->IsDeveloperModeEnabled();
			}
		}
	}

	if (!bAllowActions)
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
	else if (Action == TEXT("RemoveMoney"))
	{
		UApartmentLifeDevToolsLibrary::RemoveMoney(Controller, Girl, 200.f);
	}
	else if (Action == TEXT("AdvanceHour"))
	{
		if (UWorld* World = GetWorld())
		{
			if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				TimeSubsystem->AdvanceMinutes(60);
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
	else if (Action == TEXT("StressUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetStress(Girl, Sim->GetMood().Stress + 10.f);
		}
	}
	else if (Action == TEXT("StressDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetStress(Girl, Sim->GetMood().Stress - 10.f);
		}
	}
	else if (Action == TEXT("HungerUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetHunger(Girl, Sim->Needs.Hunger + 10.f);
		}
	}
	else if (Action == TEXT("HungerDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetHunger(Girl, Sim->Needs.Hunger - 10.f);
		}
	}
	else if (Action == TEXT("ComfortUp"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetComfort(Girl, Sim->GetMood().Comfort + 10.f);
		}
	}
	else if (Action == TEXT("ComfortDown"))
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Girl))
		{
			UApartmentLifeDevToolsLibrary::SetComfort(Girl, Sim->GetMood().Comfort - 10.f);
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
	else if (Action == TEXT("UnlockAllClothing"))
	{
		UApartmentLifeDevToolsLibrary::UnlockAllClothing(Girl);
	}
	else if (Action == TEXT("UnlockAllShop"))
	{
		UApartmentLifeDevToolsLibrary::UnlockAllShopItems(Girl);
	}
	else if (Action == TEXT("CompleteActivity"))
	{
		UApartmentLifeDevToolsLibrary::CompleteCurrentActivity(Girl);
	}
	else if (Action == TEXT("SpawnDesk"))
	{
		UApartmentLifeDevToolsLibrary::SpawnFurnitureAtCursor(
			Controller, ApartmentUnit.Get(), FName(TEXT("furniture.desk.default")), EApartmentLifeRoomType::Office);
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
	else if (Action == TEXT("DeleteSave"))
	{
		UApartmentLifeDevToolsLibrary::DeleteSaveTest(Controller, DebugSaveSlot);
	}
	else if (Action == TEXT("ClearSaves"))
	{
		UApartmentLifeDevToolsLibrary::ClearAllSaveSlots(Controller, 8);
	}
	else if (Action == TEXT("ResetApartment"))
	{
		UApartmentLifeDevToolsLibrary::ResetApartmentLayout(ApartmentUnit.Get());
	}
	else if (Action == TEXT("ResetCharacter"))
	{
		UApartmentLifeDevToolsLibrary::ResetCharacterState(Girl);
	}
	else if (Action == TEXT("PrintCreatorData"))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(9010, 8.f, FColor::Green, UApartmentLifeDevToolsLibrary::ExportCreatorData(Girl));
		}
	}
	else if (Action == TEXT("ResetCreatorFace"))
	{
		UApartmentLifeDevToolsLibrary::ResetCreatorFace(Girl);
	}
	else if (Action == TEXT("ResetCreatorBody"))
	{
		UApartmentLifeDevToolsLibrary::ResetCreatorBody(Girl);
	}
	else if (Action == TEXT("ResetAnimation"))
	{
		UApartmentLifeDevToolsLibrary::ResetAnimationState(Girl);
	}
	else if (Action == TEXT("ShowIkTargets"))
	{
		UApartmentLifeDevToolsLibrary::ShowIkTargets(Girl, 8.f);
	}
	else if (Action == TEXT("PreviewMorningRoutine"))
	{
		UApartmentLifeDevToolsLibrary::PreviewRoutineChain(Girl, FName(TEXT("routine.morning")));
	}

	ShowMenuOverlay();
}

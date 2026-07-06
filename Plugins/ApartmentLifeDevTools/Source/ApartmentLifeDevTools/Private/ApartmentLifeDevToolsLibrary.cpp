// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDevToolsLibrary.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeUnlockLibrary.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeWorldSimLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

UApartmentLifeNPCSimulationComponent* UApartmentLifeDevToolsLibrary::GetSimulation(AActor* Character)
{
	return Character ? Character->FindComponentByClass<UApartmentLifeNPCSimulationComponent>() : nullptr;
}

bool UApartmentLifeDevToolsLibrary::AddMoney(UObject* WorldContextObject, AActor* Character, float Amount)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Finance.Savings += Amount;
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::RemoveMoney(UObject* WorldContextObject, AActor* Character, float Amount)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	(void)WorldContextObject;
	return AddMoney(nullptr, Character, -FMath::Abs(Amount));
#endif
}

bool UApartmentLifeDevToolsLibrary::SetGameHour(UObject* WorldContextObject, int32 Hour)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		FApartmentLifeGameTime Time = TimeSubsystem->GetCurrentTime();
		Time.Hour = FMath::Clamp(Hour, 0, 23);
		TimeSubsystem->SetCurrentTime(Time);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetGameTime(UObject* WorldContextObject, int32 Hour, int32 Minute)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		FApartmentLifeGameTime Time = TimeSubsystem->GetCurrentTime();
		Time.Hour = FMath::Clamp(Hour, 0, 23);
		Time.Minute = FMath::Clamp(Minute, 0, 59);
		TimeSubsystem->SetCurrentTime(Time);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetMood(AActor* Character, float Mood)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Mood.OverallMood = FMath::Clamp(Mood, 0.f, 100.f);
		Sim->Mood.Happiness = Sim->Mood.OverallMood;
		Sim->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Sim->Mood, Sim->MoodInfluences, Sim->Personality);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetEnergy(AActor* Character, float Energy)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Mood.Energy = FMath::Clamp(Energy, 0.f, 100.f);
		Sim->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Sim->Mood, Sim->MoodInfluences, Sim->Personality);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetHygiene(AActor* Character, float Hygiene)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Needs.Hygiene = FMath::Clamp(Hygiene, 0.f, 100.f);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetHunger(AActor* Character, float Hunger)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Needs.Hunger = FMath::Clamp(Hunger, 0.f, 100.f);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetComfort(AActor* Character, float Comfort)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Mood.Comfort = FMath::Clamp(Comfort, 0.f, 100.f);
		Sim->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Sim->Mood, Sim->MoodInfluences, Sim->Personality);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetConfidence(AActor* Character, float Confidence)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Mood.Confidence = FMath::Clamp(Confidence, 0.f, 100.f);
		Sim->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Sim->Mood, Sim->MoodInfluences, Sim->Personality);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetStress(AActor* Character, float Stress)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Mood.Stress = FMath::Clamp(Stress, 0.f, 100.f);
		Sim->Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Sim->Mood, Sim->MoodInfluences, Sim->Personality);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::SetAffection(AActor* Character, float Affection)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->AffectionTowardPlayer = FMath::Clamp(Affection, 0.f, 100.f);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::EquipOutfitContext(AActor* Character, EApartmentLifeOutfitContext Context)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeWardrobeComponent* Wardrobe = Character ? Character->FindComponentByClass<UApartmentLifeWardrobeComponent>() : nullptr)
	{
		FApartmentLifeWeatherState Weather;
		if (UWorld* World = Character->GetWorld())
		{
			if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				Weather = TimeSubsystem->GetCurrentWeather();
			}
		}
		Wardrobe->SelectOutfitForOutfitContext(Context, Weather);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::ResetEquippedOutfit(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	return EquipOutfitContext(Character, EApartmentLifeOutfitContext::Everyday);
#endif
}

bool UApartmentLifeDevToolsLibrary::UnlockAllClothing(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeWardrobeComponent* Wardrobe = Character ? Character->FindComponentByClass<UApartmentLifeWardrobeComponent>() : nullptr)
	{
		for (const FApartmentLifeBuiltinClothingItem& Item : UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog())
		{
			Wardrobe->AddOwnedClothing(Item.ItemId);
		}
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::UnlockAllShopItems(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeProgressionComponent* Progression = Character ? Character->FindComponentByClass<UApartmentLifeProgressionComponent>() : nullptr)
	{
		for (const FApartmentLifeBuiltinShopItem& Item : UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog())
		{
			Progression->UnlockShopItem(Item.ItemId);
		}
		for (uint8 Tier = 0; Tier <= static_cast<uint8>(EApartmentLifeFurnitureTier::Luxury); ++Tier)
		{
			Progression->UnlockShopTier(static_cast<EApartmentLifeFurnitureTier>(Tier));
		}
		for (const FApartmentLifeWorkTypeDefinition& Work : UApartmentLifeWorkLibrary::GetBuiltinWorkTypes())
		{
			Progression->UnlockWorkType(Work.WorkTypeId);
		}
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::TeleportToRoom(AActor* Character, EApartmentLifeRoomType Room)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	return TeleportCharacter(Character, GetRoomTeleportLocation(Room));
#endif
}

bool UApartmentLifeDevToolsLibrary::TriggerActivity(AActor* Character, FName ActivityId)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Character || ActivityId.IsNone())
	{
		return false;
	}

	if (UApartmentLifeActivityComponent* Activity = Character->FindComponentByClass<UApartmentLifeActivityComponent>())
	{
		return Activity->StartActivity(ActivityId);
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::CompleteCurrentActivity(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeActivityComponent* Activity = Character ? Character->FindComponentByClass<UApartmentLifeActivityComponent>() : nullptr)
	{
		Activity->ForceCompleteActivity();
		return true;
	}
	return false;
#endif
}

FVector UApartmentLifeDevToolsLibrary::GetRoomTeleportLocation(EApartmentLifeRoomType Room)
{
	switch (Room)
	{
	case EApartmentLifeRoomType::Bedroom: return FVector(-150.f, -80.f, 0.f);
	case EApartmentLifeRoomType::Bathroom: return FVector(-30.f, 180.f, 0.f);
	case EApartmentLifeRoomType::LivingRoom: return FVector(80.f, 60.f, 0.f);
	case EApartmentLifeRoomType::Kitchen: return FVector(140.f, 160.f, 0.f);
	case EApartmentLifeRoomType::Office: return FVector(-40.f, -120.f, 0.f);
	default: return FVector::ZeroVector;
	}
}

FString UApartmentLifeDevToolsLibrary::BuildCharacterDebugSummary(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return FString();
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		int32 Hour = 0;
		if (UWorld* World = Character ? Character->GetWorld() : nullptr)
		{
			if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				Hour = TimeSubsystem->GetCurrentTime().Hour;
			}
		}

		return FString::Printf(
			TEXT("Time %02d:00 | Mood %.0f | Energy %.0f | Hygiene %.0f | Stress %.0f | Savings $%.0f | Activity %s"),
			Hour,
			Sim->GetMood().OverallMood,
			Sim->GetMood().Energy,
			Sim->Needs.Hygiene,
			Sim->GetMood().Stress,
			Sim->GetFinance().Savings,
			*Sim->GetCurrentActivityId().ToString());
	}
	return TEXT("No simulation component");
#endif
}

bool UApartmentLifeDevToolsLibrary::SpawnFurnitureAtCursor(
	UObject* WorldContextObject,
	AApartmentLifeApartmentUnit* Apartment,
	FName FurnitureItemId,
	EApartmentLifeRoomType Room)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Apartment)
	{
		return false;
	}

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	FVector SpawnLocation = Apartment->GetActorLocation() + FVector(100.f, 0.f, 0.f);
	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		SpawnLocation = CamLoc + CamRot.Vector() * 200.f;
	}

	FApartmentLifePlacedFurnitureInstance Instance;
	Instance.FurnitureItemId = FurnitureItemId;
	Instance.Room = Room;
	Instance.Transform = FTransform(FRotator::ZeroRotator, SpawnLocation);

	if (UApartmentLifeBuildModeComponent* BuildMode = Apartment->GetBuildModeComponent())
	{
		return BuildMode->PlaceItem(FurnitureItemId, Instance.Transform, Room);
	}

	return Apartment->PlaceFurnitureInstance(Instance);
#endif
}

bool UApartmentLifeDevToolsLibrary::ResetApartmentLayout(AApartmentLifeApartmentUnit* Apartment)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Apartment)
	{
		return false;
	}

	TArray<FGuid> InstanceIds;
	for (const FApartmentLifePlacedFurnitureInstance& Instance : Apartment->GetPlacedFurniture())
	{
		InstanceIds.Add(Instance.InstanceId);
	}

	for (const FGuid& Id : InstanceIds)
	{
		Apartment->RemoveFurnitureInstance(Id, false);
	}
	return true;
#endif
}

bool UApartmentLifeDevToolsLibrary::ResetCharacterState(AActor* Character)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = GetSimulation(Character))
	{
		Sim->Finance = FApartmentLifeFinancialLedger();
		Sim->Finance.Savings = 1500.f;
		Sim->Mood = FApartmentLifeMoodState();
		Sim->Needs = FApartmentLifeNPCNeeds();
		Sim->Skills = FApartmentLifeSkillSet();
		Sim->AffectionTowardPlayer = 55.f;
		Sim->TrustTowardPlayer = 50.f;
	}

	if (UApartmentLifeProgressionComponent* Progression = Character ? Character->FindComponentByClass<UApartmentLifeProgressionComponent>() : nullptr)
	{
		Progression->SeedStarterUnlocks();
	}

	return ResetEquippedOutfit(Character);
#endif
}

bool UApartmentLifeDevToolsLibrary::TeleportCharacter(AActor* Character, const FVector& Location, const FRotator& Rotation)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Character)
	{
		return false;
	}

	Character->SetActorLocationAndRotation(Location, Rotation);
	return true;
#endif
}

bool UApartmentLifeDevToolsLibrary::PreviewAnimation(AActor* Character, FName AnimationGroupName)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Character)
	{
		return false;
	}

	if (UApartmentLifeAnimationComponent* Animation = Character->FindComponentByClass<UApartmentLifeAnimationComponent>())
	{
		const UEnum* Enum = StaticEnum<EApartmentLifeAnimationGroup>();
		if (Enum)
		{
			const int64 Value = Enum->GetValueByName(AnimationGroupName);
			if (Value != INDEX_NONE)
			{
				Animation->SetAnimationGroup(static_cast<EApartmentLifeAnimationGroup>(Value));
				return true;
			}
		}
		Animation->SetAnimationGroup(EApartmentLifeAnimationGroup::Idle);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::PreviewDialogue(AActor* Speaker, AActor* Listener)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Speaker || !Listener)
	{
		return false;
	}

	UApartmentLifeConversationComponent* Conversation = Speaker->FindComponentByClass<UApartmentLifeConversationComponent>();
	UApartmentLifeNPCSimulationComponent* SpeakerSim = GetSimulation(Speaker);
	UApartmentLifeNPCSimulationComponent* ListenerSim = GetSimulation(Listener);
	if (!Conversation || !SpeakerSim || !ListenerSim)
	{
		return false;
	}

	FApartmentLifeGameTime Time;
	FApartmentLifeWeatherState Weather;
	if (UWorld* World = Speaker->GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Time = TimeSubsystem->GetCurrentTime();
			Weather = TimeSubsystem->GetCurrentWeather();
		}
	}

	const FApartmentLifeDialogueContext Context =
		UApartmentLifeSocialLibrary::BuildDialogueContext(SpeakerSim, ListenerSim, Time, Weather, NAME_None);
	return Conversation->StartConversation(Listener, Context);
#endif
}

bool UApartmentLifeDevToolsLibrary::SaveTest(UObject* WorldContextObject, int32 SlotIndex)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromWorldContext(WorldContextObject) : nullptr)
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			return SaveSubsystem->SaveToSlot(SlotIndex, TEXT("DevSaveTest"));
		}
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::LoadTest(UObject* WorldContextObject, int32 SlotIndex)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromWorldContext(WorldContextObject) : nullptr)
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			return SaveSubsystem->LoadFromSlot(SlotIndex);
		}
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::DeleteSaveTest(UObject* WorldContextObject, int32 SlotIndex)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromWorldContext(WorldContextObject) : nullptr)
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			return SaveSubsystem->DeleteSaveSlot(SlotIndex);
		}
	}
	return false;
#endif
}

bool UApartmentLifeDevToolsLibrary::ClearAllSaveSlots(UObject* WorldContextObject, int32 MaxSlots)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromWorldContext(WorldContextObject) : nullptr)
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			bool bAnyDeleted = false;
			for (int32 Slot = 0; Slot < MaxSlots; ++Slot)
			{
				if (SaveSubsystem->DeleteSaveSlot(Slot))
				{
					bAnyDeleted = true;
				}
			}
			return bAnyDeleted;
		}
	}
	return false;
#endif
}

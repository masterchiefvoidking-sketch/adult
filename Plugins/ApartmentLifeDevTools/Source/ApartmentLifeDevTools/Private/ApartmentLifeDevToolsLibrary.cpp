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

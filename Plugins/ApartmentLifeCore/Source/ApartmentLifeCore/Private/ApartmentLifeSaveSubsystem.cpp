// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

FString UApartmentLifeSaveSubsystem::BuildSlotName(int32 SlotIndex) const
{
	return FString::Printf(TEXT("ApartmentLifeSave_%d"), SlotIndex);
}

UWorld* UApartmentLifeSaveSubsystem::GetActiveWorld() const
{
	if (UGameInstance* GI = GetGameInstance())
	{
		return GI->GetWorld();
	}
	return nullptr;
}

bool UApartmentLifeSaveSubsystem::SaveToSlot(int32 SlotIndex, const FString& SlotName)
{
	UWorld* World = GetActiveWorld();
	if (!World)
	{
		OnSaveCompleted.Broadcast(false);
		return false;
	}

	UApartmentLifeSaveGame* SaveGame = Cast<UApartmentLifeSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UApartmentLifeSaveGame::StaticClass()));

	if (!SaveGame)
	{
		OnSaveCompleted.Broadcast(false);
		return false;
	}

	SaveGame->SaveVersion = UApartmentLifeSaveGame::CurrentSaveVersion;

	if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		SaveGame->WorldTime = TimeSubsystem->GetCurrentTime();
		SaveGame->Weather = TimeSubsystem->GetCurrentWeather();
		SaveGame->Season = TimeSubsystem->GetCurrentSeason();
	}

	CollectSaveables(SaveGame);

	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGame, SlotName.IsEmpty() ? BuildSlotName(SlotIndex) : SlotName, SlotIndex);
	OnSaveCompleted.Broadcast(bSuccess);
	return bSuccess;
}

bool UApartmentLifeSaveSubsystem::LoadFromSlot(int32 SlotIndex)
{
	const FString SlotName = BuildSlotName(SlotIndex);
	if (!DoesSaveExist(SlotIndex))
	{
		OnLoadCompleted.Broadcast(false);
		return false;
	}

	UApartmentLifeSaveGame* SaveGame = Cast<UApartmentLifeSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));

	if (!SaveGame)
	{
		OnLoadCompleted.Broadcast(false);
		return false;
	}

	UWorld* World = GetActiveWorld();
	if (World)
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->SetCurrentTime(SaveGame->WorldTime);
			TimeSubsystem->SetWeather(SaveGame->Weather);
		}
	}

	ApplySaveables(SaveGame);
	OnLoadCompleted.Broadcast(true);
	return true;
}

bool UApartmentLifeSaveSubsystem::DoesSaveExist(int32 SlotIndex) const
{
	return UGameplayStatics::DoesSaveGameExist(BuildSlotName(SlotIndex), SlotIndex);
}

void UApartmentLifeSaveSubsystem::CollectSaveables(UApartmentLifeSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	UWorld* World = GetActiveWorld();
	if (!World)
	{
		return;
	}

	SaveGame->SerializedActors.Empty();

	for (UObject* Registered : FApartmentLifeSaveableRegistry::GetRegistered())
	{
		if (!Registered || !Registered->Implements<UApartmentLifeSaveable>())
		{
			continue;
		}

		FApartmentLifeSerializedActor Entry;
		Entry.SaveId = IApartmentLifeSaveable::Execute_GetSaveId(Registered);
		IApartmentLifeSaveable::Execute_CaptureSaveData(Registered, Entry.Data);
		SaveGame->SerializedActors.Add(Entry);
	}

	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor || !Actor->Implements<UApartmentLifeSaveable>())
		{
			continue;
		}

		FApartmentLifeSerializedActor Entry;
		Entry.SaveId = IApartmentLifeSaveable::Execute_GetSaveId(Actor);
		IApartmentLifeSaveable::Execute_CaptureSaveData(Actor, Entry.Data);
		SaveGame->SerializedActors.Add(Entry);

		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (Component && Component->Implements<UApartmentLifeSaveable>())
			{
				FApartmentLifeSerializedActor CompEntry;
				CompEntry.SaveId = IApartmentLifeSaveable::Execute_GetSaveId(Component);
				IApartmentLifeSaveable::Execute_CaptureSaveData(Component, CompEntry.Data);
				SaveGame->SerializedActors.Add(CompEntry);
			}
		}
	}
}

void UApartmentLifeSaveSubsystem::ApplySaveables(const UApartmentLifeSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return;
	}

	UWorld* World = GetActiveWorld();
	if (!World)
	{
		return;
	}

	for (const FApartmentLifeSerializedActor& Entry : SaveGame->SerializedActors)
	{
		bool bRestored = false;

		for (UObject* Registered : FApartmentLifeSaveableRegistry::GetRegistered())
		{
			if (Registered && Registered->Implements<UApartmentLifeSaveable>())
			{
				if (IApartmentLifeSaveable::Execute_GetSaveId(Registered) == Entry.SaveId)
				{
					IApartmentLifeSaveable::Execute_RestoreSaveData(Registered, Entry.Data);
					bRestored = true;
					break;
				}
			}
		}

		if (bRestored)
		{
			continue;
		}

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && Actor->Implements<UApartmentLifeSaveable>())
			{
				if (IApartmentLifeSaveable::Execute_GetSaveId(Actor) == Entry.SaveId)
				{
					IApartmentLifeSaveable::Execute_RestoreSaveData(Actor, Entry.Data);
					bRestored = true;
					break;
				}
			}

			for (UActorComponent* Component : Actor->GetComponents())
			{
				if (Component && Component->Implements<UApartmentLifeSaveable>())
				{
					if (IApartmentLifeSaveable::Execute_GetSaveId(Component) == Entry.SaveId)
					{
						IApartmentLifeSaveable::Execute_RestoreSaveData(Component, Entry.Data);
						bRestored = true;
						break;
					}
				}
			}

			if (bRestored)
			{
				break;
			}
		}
	}
}

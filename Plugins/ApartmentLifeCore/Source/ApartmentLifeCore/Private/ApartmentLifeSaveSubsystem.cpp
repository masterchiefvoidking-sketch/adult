// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Misc/App.h"

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

bool UApartmentLifeSaveSubsystem::CanLoadSaveVersion(int32 Version) const
{
	return Version > 0 && Version <= UApartmentLifeSaveGame::CurrentSaveVersion;
}

bool UApartmentLifeSaveSubsystem::MigrateSaveGame(UApartmentLifeSaveGame* SaveGame) const
{
	if (!SaveGame)
	{
		return false;
	}

	if (SaveGame->SaveVersion >= UApartmentLifeSaveGame::CurrentSaveVersion)
	{
		return true;
	}

	if (SaveGame->SaveVersion < 2)
	{
		SaveGame->SaveVersion = 2;
	}

	if (SaveGame->SaveVersion < 3)
	{
		if (SaveGame->Metadata.SlotDisplayName.IsEmpty())
		{
			SaveGame->Metadata.SlotDisplayName = TEXT("Legacy Save");
		}
		if (SaveGame->Metadata.GameVersion.IsEmpty())
		{
			SaveGame->Metadata.GameVersion = FApp::GetProjectName();
		}
		if (SaveGame->Metadata.CreatedTimestamp.GetTicks() == 0)
		{
			SaveGame->Metadata.CreatedTimestamp = FDateTime::UtcNow();
		}
		SaveGame->SaveVersion = 3;
	}

	return SaveGame->SaveVersion == UApartmentLifeSaveGame::CurrentSaveVersion;
}

void UApartmentLifeSaveSubsystem::PopulateMetadata(UApartmentLifeSaveGame* SaveGame, const FString& SlotName, bool bAutosave) const
{
	if (!SaveGame)
	{
		return;
	}

	if (SaveGame->Metadata.CreatedTimestamp.GetTicks() == 0)
	{
		SaveGame->Metadata.CreatedTimestamp = FDateTime::UtcNow();
	}

	SaveGame->Metadata.LastPlayedTimestamp = FDateTime::UtcNow();
	SaveGame->Metadata.GameVersion = FApp::GetProjectName();

	if (SlotName.IsEmpty())
	{
		SaveGame->Metadata.SlotDisplayName = bAutosave ? TEXT("Autosave") : TEXT("Manual Save");
	}
	else
	{
		SaveGame->Metadata.SlotDisplayName = SlotName;
	}

	UWorld* World = GetActiveWorld();
	if (!World)
	{
		return;
	}

	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor)
		{
			continue;
		}

		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component || !Component->Implements<UApartmentLifeSaveable>())
			{
				continue;
			}

			const FString SaveId = IApartmentLifeSaveable::Execute_GetSaveId(Component);
			if (SaveId.StartsWith(TEXT("npc_sim_")))
			{
				SaveGame->Metadata.CharacterId = FName(*SaveId.RightChop(8));
			}
			else if (SaveId.StartsWith(TEXT("apartment_")))
			{
				SaveGame->Metadata.ApartmentId = FName(*SaveId.RightChop(10));
			}
		}

		if (Actor->Implements<UApartmentLifeSaveable>())
		{
			const FString SaveId = IApartmentLifeSaveable::Execute_GetSaveId(Actor);
			if (SaveId.StartsWith(TEXT("apartment_")))
			{
				SaveGame->Metadata.ApartmentId = FName(*SaveId.RightChop(10));
			}
		}
	}
}

bool UApartmentLifeSaveSubsystem::SaveToSlot(int32 SlotIndex, const FString& SlotName, bool bAutosave)
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

	const FString EffectiveSlotName = SlotName.IsEmpty() ? BuildSlotName(SlotIndex) : SlotName;

	if (DoesSaveExist(SlotIndex))
	{
		if (UApartmentLifeSaveGame* Existing = Cast<UApartmentLifeSaveGame>(
			UGameplayStatics::LoadGameFromSlot(BuildSlotName(SlotIndex), SlotIndex)))
		{
			SaveGame->Metadata.CreatedTimestamp = Existing->Metadata.CreatedTimestamp;
			if (SaveGame->Metadata.CreatedTimestamp.GetTicks() == 0)
			{
				SaveGame->Metadata.CreatedTimestamp = FDateTime::UtcNow();
			}
		}
	}

	SaveGame->SaveVersion = UApartmentLifeSaveGame::CurrentSaveVersion;

	if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		SaveGame->WorldTime = TimeSubsystem->GetCurrentTime();
		SaveGame->Weather = TimeSubsystem->GetCurrentWeather();
		SaveGame->Season = TimeSubsystem->GetCurrentSeason();
		SaveGame->Holidays = TimeSubsystem->GetRegisteredHolidays();
		SaveGame->Birthdays = TimeSubsystem->GetRegisteredBirthdays();
	}

	CollectSaveables(SaveGame);
	PopulateMetadata(SaveGame, EffectiveSlotName, bAutosave);

	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGame, BuildSlotName(SlotIndex), SlotIndex);
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

	if (!CanLoadSaveVersion(SaveGame->SaveVersion))
	{
		OnLoadCompleted.Broadcast(false);
		return false;
	}

	if (!MigrateSaveGame(SaveGame))
	{
		OnLoadCompleted.Broadcast(false);
		return false;
	}

	UWorld* World = GetActiveWorld();
	if (World)
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->RestoreFromSave(
				SaveGame->WorldTime,
				SaveGame->Weather,
				SaveGame->Season,
				SaveGame->Holidays,
				SaveGame->Birthdays);
		}
	}

	ApplySaveables(SaveGame);
	ApplyPostLoadRefresh();

	SaveGame->Metadata.LastPlayedTimestamp = FDateTime::UtcNow();
	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, SlotIndex);

	OnLoadCompleted.Broadcast(true);
	return true;
}

bool UApartmentLifeSaveSubsystem::DoesSaveExist(int32 SlotIndex) const
{
	return UGameplayStatics::DoesSaveGameExist(BuildSlotName(SlotIndex), SlotIndex);
}

bool UApartmentLifeSaveSubsystem::DeleteSaveSlot(int32 SlotIndex)
{
	if (!DoesSaveExist(SlotIndex))
	{
		return false;
	}
	return UGameplayStatics::DeleteGameInSlot(BuildSlotName(SlotIndex), SlotIndex);
}

bool UApartmentLifeSaveSubsystem::GetSlotMetadata(int32 SlotIndex, FApartmentLifeSaveSlotMetadata& OutMetadata, int32& OutSaveVersion) const
{
	if (!DoesSaveExist(SlotIndex))
	{
		return false;
	}

	if (const UApartmentLifeSaveGame* SaveGame = Cast<UApartmentLifeSaveGame>(
		UGameplayStatics::LoadGameFromSlot(BuildSlotName(SlotIndex), SlotIndex)))
	{
		OutMetadata = SaveGame->Metadata;
		OutSaveVersion = SaveGame->SaveVersion;
		return true;
	}
	return false;
}

void UApartmentLifeSaveSubsystem::RequestAutosave(int32 SlotIndex, FName Reason)
{
	(void)Reason;
	if (!bAutosaveEnabled)
	{
		return;
	}

	UWorld* World = GetActiveWorld();
	if (!World)
	{
		return;
	}

	const double Now = World->GetTimeSeconds();
	if (Now - LastAutosaveWorldTime < AutosaveCooldownSeconds)
	{
		return;
	}

	LastAutosaveWorldTime = Now;
	SaveToSlot(SlotIndex >= 0 ? SlotIndex : DefaultAutosaveSlot, TEXT("Autosave"), true);
}

void UApartmentLifeSaveSubsystem::ApplyPostLoadRefresh()
{
	// Post-load hooks are implemented by game-specific modules listening to OnLoadCompleted.
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

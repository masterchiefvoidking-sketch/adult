// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeSaveGame.h"
#include "ApartmentLifeSaveSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeSaveVersionTest,
	"ApartmentLife.Save.VersionAndMigration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeSaveVersionTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Current save version is 3"), UApartmentLifeSaveGame::CurrentSaveVersion, 3);

	UApartmentLifeSaveGame* LegacySave = NewObject<UApartmentLifeSaveGame>();
	LegacySave->SaveVersion = 1;

	UApartmentLifeSaveSubsystem* Subsystem = NewObject<UApartmentLifeSaveSubsystem>();
	TestTrue(TEXT("Legacy save migrates to current version"), Subsystem->MigrateSaveGame(LegacySave));
	TestEqual(TEXT("Migrated version"), LegacySave->SaveVersion, 3);
	TestFalse(TEXT("Metadata slot name populated"), LegacySave->Metadata.SlotDisplayName.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeSaveMetadataTest,
	"ApartmentLife.Save.MetadataFields",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeSaveMetadataTest::RunTest(const FString& Parameters)
{
	UApartmentLifeSaveGame* SaveGame = NewObject<UApartmentLifeSaveGame>();
	SaveGame->Metadata.CharacterId = FName(TEXT("character.main"));
	SaveGame->Metadata.ApartmentId = FName(TEXT("apartment.main"));
	SaveGame->Metadata.CreatedTimestamp = FDateTime(2026, 7, 6, 8, 0, 0);
	SaveGame->SaveVersion = UApartmentLifeSaveGame::CurrentSaveVersion;

	TestEqual(TEXT("Character ID stored"), SaveGame->Metadata.CharacterId, FName(TEXT("character.main")));
	TestEqual(TEXT("Apartment ID stored"), SaveGame->Metadata.ApartmentId, FName(TEXT("apartment.main")));
	TestTrue(TEXT("Timestamp stored"), SaveGame->Metadata.CreatedTimestamp.GetYear() == 2026);
	return true;
}

#endif

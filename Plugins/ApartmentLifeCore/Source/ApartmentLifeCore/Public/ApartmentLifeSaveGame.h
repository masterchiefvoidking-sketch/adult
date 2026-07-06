// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCalendarTypes.h"
#include "ApartmentLifeSaveGame.generated.h"

USTRUCT()
struct FApartmentLifeSerializedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FString SaveId;

	UPROPERTY()
	TMap<FString, FString> Data;
};

USTRUCT(BlueprintType)
struct FApartmentLifeSaveSlotMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString SlotDisplayName;

	UPROPERTY()
	FString GameVersion;

	UPROPERTY()
	FDateTime CreatedTimestamp;

	UPROPERTY()
	FDateTime LastPlayedTimestamp;

	UPROPERTY()
	FName CharacterId;

	UPROPERTY()
	FName ApartmentId;
};

/**
 * Versioned save game container. Aggregates all IApartmentLifeSaveable state plus world time.
 */
UCLASS()
class APARTMENTLIFECORE_API UApartmentLifeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static constexpr int32 CurrentSaveVersion = 3;

	UPROPERTY()
	int32 SaveVersion = CurrentSaveVersion;

	UPROPERTY()
	FApartmentLifeSaveSlotMetadata Metadata;

	UPROPERTY()
	FApartmentLifeGameTime WorldTime;

	UPROPERTY()
	FApartmentLifeWeatherState Weather;

	UPROPERTY()
	EApartmentLifeSeason Season = EApartmentLifeSeason::Summer;

	UPROPERTY()
	TArray<FApartmentLifeHolidayDefinition> Holidays;

	UPROPERTY()
	TArray<FApartmentLifeBirthdayRecord> Birthdays;

	UPROPERTY()
	TArray<FApartmentLifeSerializedActor> SerializedActors;
};

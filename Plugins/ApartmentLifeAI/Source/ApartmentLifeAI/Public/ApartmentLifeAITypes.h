// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeAITypes.generated.h"

USTRUCT(BlueprintType)
struct APARTMENTLIFEAI_API FApartmentLifeScheduleEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	int32 StartHour = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	int32 EndHour = 17;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	EApartmentLifeOccasion Occasion = EApartmentLifeOccasion::Work;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAI_API FApartmentLifeEconomyState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float MonthlyIncome = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float Savings = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float MonthlyBills = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float FoodBudget = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	float EntertainmentBudget = 150.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAI_API FApartmentLifeRelationshipState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	FName OtherCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "-100", ClampMax = "100"))
	float Affinity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Trust = 50.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAI_API FApartmentLifeMemoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FName EventId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FApartmentLifeGameTime Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	float EmotionalWeight = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAI_API FApartmentLifePersonalityProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Extroversion = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Conscientiousness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Openness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preferences")
	TArray<FName> HobbyIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preferences")
	TArray<FName> FoodPreferences;
};

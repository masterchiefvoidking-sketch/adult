// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ApartmentLifeContentSettings.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeMaturityRating : uint8
{
	Teen		UMETA(DisplayName = "Teen"),
	Mature		UMETA(DisplayName = "Mature"),
	AdultsOnly	UMETA(DisplayName = "Adults Only")
};

/**
 * Project-wide content gating. Explicit activities, nudity, and bodily function
 * simulations are data-driven but only activate when permitted here.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Apartment Life Content Settings"))
class APARTMENTLIFECORE_API UApartmentLifeContentSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UApartmentLifeContentSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Content")
	EApartmentLifeMaturityRating MaturityRating = EApartmentLifeMaturityRating::AdultsOnly;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Content")
	bool bEnableExplicitActivities = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Content")
	bool bEnableNudity = true;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Content")
	bool bEnableBodilyFunctions = true;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Content")
	static const UApartmentLifeContentSettings* Get();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Content")
	static bool IsExplicitContentAllowed();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Content")
	static bool IsNudityAllowed();
};

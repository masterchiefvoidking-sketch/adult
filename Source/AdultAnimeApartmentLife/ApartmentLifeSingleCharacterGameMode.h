// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AdultAnimeApartmentLifeGameModeBase.h"
#include "ApartmentLifeSingleCharacterGameMode.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;

/**
 * Bootstraps the private single-character build: one apartment and one customizable girl.
 */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeSingleCharacterGameMode : public AAdultAnimeApartmentLifeGameModeBase
{
	GENERATED_BODY()

public:
	AApartmentLifeSingleCharacterGameMode();

protected:
	virtual void BeginPlay() override;

	void BootstrapApartment();
	void SeedStarterFurniture(AApartmentLifeApartmentUnit* Apartment);
	void ConfigureGirlCharacter(AApartmentLifeSimCharacter* Character);
	void LinkPlayerController();

	UPROPERTY(EditAnywhere, Category = "Single Character")
	bool bAutoLoadOnStart = true;

	UPROPERTY(EditAnywhere, Category = "Single Character")
	int32 AutoLoadSlot = 0;

	UPROPERTY(EditAnywhere, Category = "Single Character")
	FName RemoteWorkTypeId = FName(TEXT("career.remote_programmer"));

	UPROPERTY()
	TObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TObjectPtr<AApartmentLifeSimCharacter> GirlCharacter;
};

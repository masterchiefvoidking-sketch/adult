// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AdultAnimeApartmentLifeGameModeBase.h"
#include "ApartmentLifeVerticalSliceGameMode.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;

/**
 * Bootstraps the private vertical slice: one apartment, player, partner NPC,
 * starter furniture, and optional auto-load from the quick-save slot.
 */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeVerticalSliceGameMode : public AAdultAnimeApartmentLifeGameModeBase
{
	GENERATED_BODY()

public:
	AApartmentLifeVerticalSliceGameMode();

protected:
	virtual void BeginPlay() override;

	void BootstrapSlice();
	void SeedStarterFurniture(AApartmentLifeApartmentUnit* Apartment);
	void ConfigureCharacter(AApartmentLifeSimCharacter* Character, FName CharacterId, const FVector& Location);
	void LinkPlayerController();

	UPROPERTY(EditAnywhere, Category = "Slice")
	bool bAutoLoadOnStart = true;

	UPROPERTY(EditAnywhere, Category = "Slice")
	int32 AutoLoadSlot = 0;

	UPROPERTY()
	TObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TObjectPtr<AApartmentLifeSimCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<AApartmentLifeSimCharacter> PartnerCharacter;
};

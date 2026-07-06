// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePlayerController.h"
#include "ApartmentLifeSingleCharacterPlayerController.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;
class UApartmentLifeInteractionComponent;
class UApartmentLifeBuildModeComponent;
class UApartmentLifeDebugMenuComponent;

/**
 * Player camera controller for the single-girl private build.
 * The player directs the girl's interactions, wardrobe, build mode, and conversations.
 */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeSingleCharacterPlayerController : public AApartmentLifePlayerController
{
	GENERATED_BODY()

public:
	AApartmentLifeSingleCharacterPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Single Character")
	void SetSingleCharacterContext(AApartmentLifeApartmentUnit* InApartment, AApartmentLifeSimCharacter* InGirlCharacter);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Single Character")
	AApartmentLifeSimCharacter* GetGirlCharacter() const { return GirlCharacter.Get(); }

protected:
	virtual void SetupInputComponent() override;

	void OnInteract();
	void OnToggleBuildMode();
	void OnBuildTopDown();
	void OnOpenWardrobe();
	void OnBuildUndo();
	void OnBuildRedo();
	void OnQuickSave();
	void OnQuickLoad();
	void OnTalkWithGirl();
	void OnFocusGirlFace();
	void OnFocusGirlOutfit();
	void OnToggleDebugMenu();
	void OnCharacterCreatorCamera();
	void OnDebugAddMoney();
	void OnDebugAdvanceHour();
	void OnDebugSetMorning();
	void OnDebugMoodUp();
	void OnDebugMoodDown();
	void OnDebugEnergyUp();
	void OnDebugEnergyDown();
	void OnDebugHygieneUp();
	void OnDebugHygieneDown();
	void OnDebugCycleOutfit();
	void OnDebugTeleportBedroom();
	void OnDebugTeleportBathroom();
	void OnDebugTeleportLivingRoom();
	void OnDebugTeleportKitchen();
	void OnDebugTeleportOffice();
	void OnDebugTriggerYoga();
	void OnDebugSaveNow();
	void OnDebugLoadNow();
	void OnDebugResetApartment();

	UApartmentLifeBuildModeComponent* GetBuildMode() const;
	UApartmentLifeInteractionComponent* GetGirlInteraction() const;

	void FocusCameraOnInteractable(AActor* Target);

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeSimCharacter> GirlCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Debug")
	TObjectPtr<UApartmentLifeDebugMenuComponent> DebugMenuComponent;

	UPROPERTY(EditAnywhere, Category = "Single Character|Save")
	int32 QuickSaveSlot = 0;

	bool bBuildModeActive = false;
};

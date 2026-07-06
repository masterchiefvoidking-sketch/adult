// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePlayerController.h"
#include "ApartmentLifeSingleCharacterPlayerController.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;
class UApartmentLifeInteractionComponent;
class UApartmentLifeInteractionSelectionComponent;
class UApartmentLifeInteractionHudComponent;
class UApartmentLifeBuildModeComponent;
class UApartmentLifeDebugMenuComponent;
class UApartmentLifeWardrobeUiController;
class UApartmentLifeWardrobeShoppingComponent;
class UApartmentLifeWorkUiController;
class UApartmentLifeFinanceUiController;
class UApartmentLifeCharacterCreatorUiController;
class UApartmentLifeDeveloperUiController;
class UApartmentLifeUiBridgeComponent;

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
	void SetSingleCharacterContext(
		AApartmentLifeApartmentUnit* InApartment,
		AApartmentLifeSimCharacter* InGirlCharacter,
		bool bInEnterGameplayDirectly = true);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Single Character")
	AApartmentLifeSimCharacter* GetGirlCharacter() const { return GirlCharacter.Get(); }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Single Character")
	void ApplyPostLoadState();

	virtual void CloseWardrobeSession() override;
	virtual void CloseBuildModeSession() override;
	virtual void CloseConversationSession() override;

protected:
	virtual void SetupInputComponent() override;

	void OnInteract();
	void OnSelectInteractable();
	void OnFocusDoubleClick();
	void OnExecuteAction1();
	void OnExecuteAction2();
	void OnExecuteAction3();
	void OnExecuteAction4();
	void OnFocusGirlUpperBody();
	void OnFocusGirlHair();
	void OnFocusGirlShoes();
	void OnPhotoFovUp();
	void OnPhotoFovDown();
	void OnPhotoApertureUp();
	void OnPhotoApertureDown();
	void OnPhotoScreenshot();
	void OnPhotoSaveBookmark();
	void OnToggleBuildMode();
	void OnBuildTopDown();
	void OnOpenWardrobe();
	void OpenWardrobeSession();
	bool IsWardrobeActivityId(FName ActivityId) const;
	void OnOpenWorkMenu();
	void OnToggleBudget();
	void OnOpenApartmentShop();
	void OnStartSelectedWork();
	void OnSelectWork1();
	void OnSelectWork2();
	void OnSelectWork3();
	void OnSelectWork4();
	void OnBuildUndo();
	void OnBuildRedo();
	void OnQuickSave();
	void OnQuickLoad();
	void OnTalkWithGirl();
	void OnFocusGirlFace();
	void OnFocusGirlOutfit();
	void OnToggleDebugMenu();
	void OnTogglePerfOverlay();
	void OnUiBack();
	void OnToggleGameHud();
	void OnOpenSaveLoadScreen();
	void OnOpenProfileScreen();
	void OnOpenRoutinesScreen();
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
	void OnDebugResetCharacter();
	void OnDebugPrintCreatorData();
	void OnDebugResetCreatorFace();
	void OnDebugResetCreatorBody();
	void OnDebugRemoveMoney();
	void OnDebugStressUp();
	void OnDebugStressDown();
	void OnDebugHungerUp();
	void OnDebugHungerDown();
	void OnDebugComfortUp();
	void OnDebugComfortDown();
	void OnDebugUnlockClothing();
	void OnDebugUnlockShop();
	void OnDebugCompleteActivity();
	void OnDebugSpawnDesk();
	void OnDebugDeleteSave();
	void OnDebugClearSaves();

	UApartmentLifeBuildModeComponent* GetBuildMode() const;
	UApartmentLifeInteractionComponent* GetGirlInteraction() const;
	UApartmentLifeInteractionSelectionComponent* GetGirlSelection() const;

	void FocusCameraOnInteractable(AActor* Target, const FVector& FocusOffset = FVector::ZeroVector);
	void FocusCameraForActivity(FName ActivityId, AActor* ContextActor = nullptr);
	void SelectWorkByIndex(int32 Index);

	UFUNCTION()
	void HandleGirlActivityStarted(FName ActivityId);

	UFUNCTION()
	void HandleGirlActivityCompleted(FName ActivityId);

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Interaction")
	TObjectPtr<UApartmentLifeInteractionHudComponent> InteractionHudComponent;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Wardrobe")
	TObjectPtr<UApartmentLifeWardrobeUiController> WardrobeUiController;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Wardrobe")
	TObjectPtr<UApartmentLifeWardrobeShoppingComponent> WardrobeShoppingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Progression")
	TObjectPtr<UApartmentLifeWorkUiController> WorkUiController;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Progression")
	TObjectPtr<UApartmentLifeFinanceUiController> FinanceUiController;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeSimCharacter> GirlCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Debug")
	TObjectPtr<UApartmentLifeDebugMenuComponent> DebugMenuComponent;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|UI")
	TObjectPtr<UApartmentLifeUiBridgeComponent> UiBridgeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Creator")
	TObjectPtr<UApartmentLifeCharacterCreatorUiController> CreatorUiController;

	UPROPERTY(VisibleAnywhere, Category = "Single Character|Developer")
	TObjectPtr<UApartmentLifeDeveloperUiController> DeveloperUiController;

	UPROPERTY(EditAnywhere, Category = "Single Character|Save")
	int32 QuickSaveSlot = 0;

	bool bBuildModeActive = false;
	bool bEnterGameplayDirectly = true;
};

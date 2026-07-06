// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeUiBridgeComponent.generated.h"

class AApartmentLifeApartmentUnit;
class APlayerController;
class UApartmentLifeBuildModeComponent;
class UApartmentLifeFinanceUiController;
class UApartmentLifeInteractionHudComponent;
class UApartmentLifeInteractionSelectionComponent;
class UApartmentLifeUiSubsystem;
class UApartmentLifeWardrobeUiController;
class UApartmentLifeWorkUiController;
class UApartmentLifeCharacterCreatorUiController;
class UApartmentLifeDeveloperUiController;
class UApartmentLifeImmersionSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApartmentLifeUiBridgePostLoadRequested);

UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEUI_API UApartmentLifeUiBridgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UApartmentLifeUiBridgeComponent();

	void InitializeContext(
		APlayerController* InController,
		AApartmentLifeApartmentUnit* InApartment,
		AActor* InGirl,
		UApartmentLifeWardrobeUiController* InWardrobeUi,
		UApartmentLifeWorkUiController* InWorkUi,
		UApartmentLifeFinanceUiController* InFinanceUi,
		UApartmentLifeInteractionHudComponent* InInteractionHud,
		UApartmentLifeCharacterCreatorUiController* InCreatorUi,
		UApartmentLifeDeveloperUiController* InDeveloperUi = nullptr,
		int32 InQuickSaveSlot = 0,
		bool bInEnterGameplayDirectly = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void OpenSaveLoadScreen();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void OpenProfileScreen();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void OpenCharacterCreatorScreen();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void OpenRoutinesScreen();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void HandleUiBack();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void ToggleHud();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SyncImmersionSettingsFromSubsystem();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void RefreshAfterLoad();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiBridgePostLoadRequested OnPostLoadRequested;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindUiSubsystem();
	void RefreshMainMenuSlots();
	void RefreshHud();
	void RefreshActiveScreen();
	void PushWardrobePanel();
	void PushWorkPanel();
	void PushWorkResultPanel();
	void PushFinancePanel();
	void PushShoppingPanel();
	void PushContextMenuPanel();
	void PushBuildModePanel();
	void PushRoutinesPanel();
	void PushProfilePanel();
	void PushSaveLoadPanel();
	void PushSettingsPanel();
	void PushYogaPanel();
	void PushCharacterCreatorPanel();
	void PushDeveloperHubPanel();
	void HandleDeveloperHubActivation(int32 Index);
	void SyncSettingsFromCamera();
	void ApplySettingsToCamera();
	void ApplyImmersionSettings();
	void BindImmersionDelegates();
	void RefreshImmersionFromGameTime();
	void ApplyMusicForScreen(EApartmentLifeUiScreen Screen);
	void ApplyMicroAnimationFromSimulation();

	UFUNCTION()
	void HandleGameHourAdvanced(const FApartmentLifeGameTime& NewTime);

	UFUNCTION()
	void HandlePhotoModeUiVisibilityChanged(bool bHideUI);

	UApartmentLifeImmersionSubsystem* GetImmersionSubsystem() const;

	UFUNCTION()
	void HandleScreenChanged(EApartmentLifeUiScreen NewScreen);

	UFUNCTION()
	void HandleListItemActivated(EApartmentLifeUiScreen Screen, int32 Index);

	UFUNCTION()
	void HandleListItemSelected(EApartmentLifeUiScreen Screen, int32 Index);

	UFUNCTION()
	void HandleMainMenuNewGame();

	UFUNCTION()
	void HandleMainMenuContinue(int32 SlotIndex);

	UFUNCTION()
	void HandleWardrobeUiStateChanged();

	UFUNCTION()
	void HandleCreatorUiStateChanged();

	UFUNCTION()
	void HandleDeveloperUiStateChanged();

	UFUNCTION()
	void HandleInteractableSelected(AActor* Target, const TArray<struct FApartmentLifeInteractionDescriptor>& Actions);

	UFUNCTION()
	void HandleInteractableCleared();

	UFUNCTION()
	void HandleYogaSessionUpdated(const struct FApartmentLifeYogaSessionState& State);

	UFUNCTION()
	void HandleRoutineStepChanged(int32 StepIndex);

	UFUNCTION()
	void HandleBuildSelectionChanged(FGuid InstanceId);

	UApartmentLifeUiSubsystem* GetUiSubsystem() const;
	UApartmentLifeInteractionSelectionComponent* GetSelection() const;
	UApartmentLifeBuildModeComponent* GetBuildMode() const;
	class UApartmentLifeNPCSimulationComponent* GetGirlSimulation() const;
	class UApartmentLifeProgressionComponent* GetGirlProgression() const;
	class UApartmentLifeActivityComponent* GetGirlActivity() const;
	class UApartmentLifeWardrobeComponent* GetGirlWardrobe() const;
	class UApartmentLifeRoutineChainComponent* GetGirlRoutine() const;
	class UApartmentLifeYogaMinigameComponent* GetGirlYoga() const;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwnerController;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AActor> GirlCharacter;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeWardrobeUiController> WardrobeUi;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeWorkUiController> WorkUi;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeFinanceUiController> FinanceUi;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeInteractionHudComponent> InteractionHud;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeCharacterCreatorUiController> CreatorUi;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeDeveloperUiController> DeveloperUi;

	int32 QuickSaveSlot = 0;
	bool bEnterGameplayDirectly = true;
	bool bBindingsComplete = false;
	EApartmentLifeShopCategory ActiveShopCategory = EApartmentLifeShopCategory::Furniture;
	float HudRefreshAccumulator = 0.f;
	float MicroAnimationAccumulator = 0.f;
	int32 ActiveRoutineOffset = 0;
	bool bImmersionBindingsComplete = false;

	static constexpr int32 MaxSaveSlots = 3;
	static constexpr int32 SaveLoadSlotStride = 10;
};

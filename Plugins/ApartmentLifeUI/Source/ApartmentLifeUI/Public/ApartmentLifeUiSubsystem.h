// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeUiTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeUiSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeUiScreenChanged, EApartmentLifeUiScreen, NewScreen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnApartmentLifeUiListItemActivated, EApartmentLifeUiScreen, Screen, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApartmentLifeUiMainMenuNewGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApartmentLifeUiMainMenuContinue, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApartmentLifeUiBackRequested);

class SApartmentLifeRootWidget;

UCLASS()
class APARTMENTLIFEUI_API UApartmentLifeUiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|UI")
	bool IsSlateUiActive() const { return RootWidget.IsValid(); }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	bool TickToast(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void EnsureViewportUi();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void ShowScreen(EApartmentLifeUiScreen Screen);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void CloseScreen();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void ToggleHud();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SetHudSuppressed(bool bSuppressed);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|UI")
	bool IsHudVisible() const { return Settings.bHudVisible && !bHudSuppressed; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|UI")
	EApartmentLifeUiScreen GetActiveScreen() const { return ActiveScreen; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SetHudState(const FApartmentLifeHudViewModel& State);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SetPanelState(const FApartmentLifeUiPanelState& State);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SetMainMenuSlots(const TArray<FApartmentLifeMainMenuSlotViewModel>& Slots);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void SetYogaState(const FApartmentLifeYogaUiState& State);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void ShowToast(const FText& Message, float DurationSeconds = 3.f);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void RefreshView();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	FApartmentLifeUiSettingsState& GetMutableSettings() { return Settings; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|UI")
	const FApartmentLifeUiSettingsState& GetSettings() const { return Settings; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void HandleListItemClicked(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void HandleListItemConfirmed(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void HandleBack();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|UI")
	void ShowMainMenu();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiScreenChanged OnScreenChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiListItemActivated OnListItemActivated;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiMainMenuNewGame OnMainMenuNewGame;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiMainMenuContinue OnMainMenuContinue;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|UI")
	FOnApartmentLifeUiBackRequested OnBackRequested;

protected:
	void PushViewToWidget();

	UPROPERTY()
	FApartmentLifeHudViewModel HudState;

	UPROPERTY()
	FApartmentLifeUiPanelState PanelState;

	UPROPERTY()
	TArray<FApartmentLifeMainMenuSlotViewModel> MainMenuSlots;

	UPROPERTY()
	FApartmentLifeYogaUiState YogaState;

	UPROPERTY()
	FApartmentLifeUiSettingsState Settings;

	UPROPERTY()
	FApartmentLifeUiToast ActiveToast;

	EApartmentLifeUiScreen ActiveScreen = EApartmentLifeUiScreen::Gameplay;
	bool bHudSuppressed = false;
	TSharedPtr<SApartmentLifeRootWidget> RootWidget;
};

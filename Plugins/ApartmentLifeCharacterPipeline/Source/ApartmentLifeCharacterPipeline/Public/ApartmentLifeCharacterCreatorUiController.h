// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorUiController.generated.h"

class UApartmentLifeCharacterCreatorComponent;
class UApartmentLifeWardrobeComponent;
class UApartmentLifeAnimationComponent;
class AApartmentLifeCameraPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterCreatorUiStateChanged);

UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeCharacterCreatorUiController : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		UApartmentLifeCharacterCreatorComponent* InCreator,
		UApartmentLifeWardrobeComponent* InWardrobe,
		UApartmentLifeAnimationComponent* InAnimation,
		AApartmentLifeCameraPawn* InCameraPawn);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void OpenCreator(bool bEditExisting = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void CloseCreator(bool bConfirm = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void ToggleCreator();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	bool IsCreatorOpen() const { return bIsOpen; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void SetCategoryTab(EApartmentLifeCreatorCategoryTab Tab);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	EApartmentLifeCreatorCategoryTab GetCategoryTab() const { return ActiveTab; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void SelectEntryIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void ActivateSelectedEntry();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void AdjustSelectedSlider(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void Randomize(EApartmentLifeCreatorRandomizeScope Scope);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void ResetCurrentCategory();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void ApplyPresetByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void SavePresetByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void SetLightingMode(EApartmentLifeCreatorLightingMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void SetCameraFocus(FName FocusMode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void PreviewAnimationByIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void ConfirmCharacter();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|UI")
	void RefreshEntryList();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	const TArray<FText>& GetVisibleEntryLabels() const { return VisibleEntryLabels; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	const TArray<FText>& GetVisibleEntryDetails() const { return VisibleEntryDetails; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	int32 GetSelectedEntryIndex() const { return SelectedEntryIndex; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	FApartmentLifeCreatorCompatibilityReport GetCompatibilityReport() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator|UI")
	const TArray<FApartmentLifeCharacterCreatorPreset>& GetVisiblePresets() const { return VisiblePresets; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Creator|UI")
	FOnCharacterCreatorUiStateChanged OnCreatorUiStateChanged;

protected:
	void BroadcastStateChanged();
	void RebuildListForActiveTab();
	FApartmentLifeCharacterCreatorState GetMutableStateCopy() const;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeCharacterCreatorComponent> Creator;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeWardrobeComponent> Wardrobe;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeAnimationComponent> Animation;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeCameraPawn> CameraPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creator|UI")
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creator|UI")
	EApartmentLifeCreatorCategoryTab ActiveTab = EApartmentLifeCreatorCategoryTab::Face;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creator|UI")
	EApartmentLifeCreatorLightingMode LightingMode = EApartmentLifeCreatorLightingMode::WardrobeStudio;

	TArray<FText> VisibleEntryLabels;
	TArray<FText> VisibleEntryDetails;
	TArray<FName> VisibleEntryIds;
	TArray<FApartmentLifeCharacterCreatorPreset> VisiblePresets;
	TArray<FName> PreviewAnimationGroups;
	int32 SelectedEntryIndex = 0;
	FApartmentLifeCharacterCreatorState SessionBackup;
};

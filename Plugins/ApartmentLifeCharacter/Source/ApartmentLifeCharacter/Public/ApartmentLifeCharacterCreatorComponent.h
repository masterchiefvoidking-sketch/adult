// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCharacterAppearanceComponent.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorComponent.generated.h"

class UApartmentLifeBodyCustomizationComponent;
class UApartmentLifeMorphCatalogData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreatorStateUpdated, const FApartmentLifeCharacterCreatorState&, State);

/**
 * Premium character creator — face, body, hair, makeup, accessories, voice, personality.
 * All options driven by data assets.
 */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTER_API UApartmentLifeCharacterCreatorComponent : public UApartmentLifeCharacterAppearanceComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void InitializeFromCatalog(UApartmentLifeMorphCatalogData* Catalog);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void InitializeFromBuiltinCatalog();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void ApplyCreatorState(const FApartmentLifeCharacterCreatorState& NewState);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	const FApartmentLifeCharacterCreatorState& GetCreatorState() const { return CreatorState; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetFaceMorph(EApartmentLifeFaceRegion Region, FName MorphName, float Value);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetBodySlider(FName MorphName, float Value);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetHairstyle(FName HairstyleId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetVoicePreset(FName VoicePresetId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetPersonalityPreset(FName PersonalityPresetId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetSkinTone(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetHairColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetEyeColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetMakeupIntensity(float Intensity);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeFace();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeBody();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeHair();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeColors();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeAppearance(EApartmentLifeCreatorRandomizeScope Scope);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void ResetFace();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void ResetBody();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void ResetAppearance();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	bool ApplyPreset(const FApartmentLifeCharacterCreatorPreset& Preset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	bool SaveCurrentAsPreset(FName PresetId, const FText& DisplayName, EApartmentLifeCreatorPresetType PresetType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	FString ExportCreatorStateJson() const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Creator")
	FOnCreatorStateUpdated OnCreatorStateUpdated;

	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void SyncBodyComponent();
	void ApplyFullAppearance();
	void BroadcastAndApply();
	FApartmentLifeCharacterCreatorState GetDefaultState() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creator")
	FApartmentLifeCharacterCreatorState CreatorState;

	UPROPERTY()
	TObjectPtr<UApartmentLifeMorphCatalogData> MorphCatalog;
};

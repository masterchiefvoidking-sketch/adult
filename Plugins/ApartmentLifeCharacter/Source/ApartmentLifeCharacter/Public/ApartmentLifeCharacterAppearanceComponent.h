// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterCustomizationData.h"
#include "ApartmentLifeCharacterAppearanceComponent.generated.h"

class UApartmentLifeCharacterCustomizationData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAppearanceUpdated);

/**
 * Applies data-driven appearance to modular character meshes.
 * Supports morphs, materials, hair, makeup, accessories, and tattoos.
 */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTER_API UApartmentLifeCharacterAppearanceComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UApartmentLifeCharacterAppearanceComponent();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Character")
	void InitializeFromData(UApartmentLifeCharacterCustomizationData* CustomizationData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Character")
	void ApplyAppearanceState(const FApartmentLifeCharacterAppearanceState& NewState);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character")
	const FApartmentLifeCharacterAppearanceState& GetAppearanceState() const { return AppearanceState; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Character")
	FOnAppearanceUpdated OnAppearanceUpdated;

	// IApartmentLifeSaveable
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeCharacterCustomizationData> CustomizationDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FApartmentLifeCharacterAppearanceState AppearanceState;

	void ApplyMorphs(USkeletalMeshComponent* Mesh, const TArray<FApartmentLifeMorphSlider>& Morphs) const;
	void ApplyColorParameter(UMeshComponent* Mesh, const FApartmentLifeColorCustomization& ColorData) const;
};

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
	void ApplyCreatorState(const FApartmentLifeCharacterCreatorState& NewState);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	const FApartmentLifeCharacterCreatorState& GetCreatorState() const { return CreatorState; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void SetFaceMorph(EApartmentLifeFaceRegion Region, FName MorphName, float Value);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	void RandomizeFace();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Creator")
	FOnCreatorStateUpdated OnCreatorStateUpdated;

	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void SyncBodyComponent();
	void ApplyFullAppearance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creator")
	FApartmentLifeCharacterCreatorState CreatorState;

	UPROPERTY()
	TObjectPtr<UApartmentLifeMorphCatalogData> MorphCatalog;
};

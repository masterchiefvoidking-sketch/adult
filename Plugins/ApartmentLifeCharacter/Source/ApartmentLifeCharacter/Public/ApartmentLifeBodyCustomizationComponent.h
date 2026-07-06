// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeBodyCustomizationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBodyCustomizationUpdated, const FApartmentLifeBodyFitProfile&, FitProfile);

/**
 * Modular body customization. Updates clothing fit, collision, and animation style hooks.
 */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTER_API UApartmentLifeBodyCustomizationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Body")
	void ApplyBodyCustomization(const FApartmentLifeBodyCustomization& Body);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Body")
	const FApartmentLifeBodyCustomization& GetBodyCustomization() const { return BodyState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Body")
	const FApartmentLifeBodyFitProfile& GetFitProfile() const { return FitProfile; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Body")
	FName GetWalkStyleId() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Body")
	FName GetIdleStyleId() const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Body")
	FOnBodyCustomizationUpdated OnBodyCustomizationUpdated;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void UpdateMeshAndCollision();
	void RecalculateFitProfile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	FApartmentLifeBodyCustomization BodyState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body")
	FApartmentLifeBodyFitProfile FitProfile;
};

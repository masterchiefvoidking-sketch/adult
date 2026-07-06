// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ApartmentLifeCharacterBase.generated.h"

class UApartmentLifeCharacterCreatorComponent;
class UApartmentLifeBodyCustomizationComponent;
class UApartmentLifeCharacterAppearanceComponent;
struct FApartmentLifeBodyFitProfile;

/**
 * Base class for all sim characters. Composes creator, body, and appearance.
 * Animation (motion matching, IK, facial) is driven by AnimBP + data assets on derived Blueprints.
 */
UCLASS(Abstract)
class APARTMENTLIFECHARACTER_API AApartmentLifeCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AApartmentLifeCharacterBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character")
	UApartmentLifeCharacterCreatorComponent* GetCreatorComponent() const { return CreatorComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character")
	UApartmentLifeCharacterAppearanceComponent* GetAppearanceComponent() const { return CreatorComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character")
	UApartmentLifeBodyCustomizationComponent* GetBodyCustomizationComponent() const { return BodyCustomizationComponent; }

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Apartment Life|Character")
	void OnBodyFitProfileUpdated(const FApartmentLifeBodyFitProfile& FitProfile);

	UFUNCTION()
	void HandleBodyCustomizationUpdated(const FApartmentLifeBodyFitProfile& FitProfile);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeCharacterCreatorComponent> CreatorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeBodyCustomizationComponent> BodyCustomizationComponent;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ApartmentLifeCharacterBase.generated.h"

class UApartmentLifeCharacterAppearanceComponent;

/**
 * Base class for all sim characters. Composes appearance, wardrobe, AI life sim, and activities.
 * Animation (motion matching, IK, facial) is driven by AnimBP + data assets on derived Blueprints.
 */
UCLASS(Abstract)
class APARTMENTLIFECHARACTER_API AApartmentLifeCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AApartmentLifeCharacterBase();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character")
	UApartmentLifeCharacterAppearanceComponent* GetAppearanceComponent() const { return AppearanceComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeCharacterAppearanceComponent> AppearanceComponent;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCharacterBase.h"
#include "ApartmentLifeSimCharacter.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeLifeSimulationComponent;
class UApartmentLifeActivityComponent;

/** Fully composed sim character with appearance, wardrobe, autonomous AI, and activities. */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeSimCharacter : public AApartmentLifeCharacterBase
{
	GENERATED_BODY()

public:
	AApartmentLifeSimCharacter();

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeWardrobeComponent* GetWardrobeComponent() const { return WardrobeComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeLifeSimulationComponent* GetLifeSimulationComponent() const { return LifeSimulationComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeActivityComponent* GetActivityComponent() const { return ActivityComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeWardrobeComponent> WardrobeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeLifeSimulationComponent> LifeSimulationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeActivityComponent> ActivityComponent;
};

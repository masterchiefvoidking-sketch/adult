// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"

/** Fully composed sim character with world simulation, wardrobe, and activities. */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeSimCharacter : public AApartmentLifeCharacterBase
{
	GENERATED_BODY()

public:
	AApartmentLifeSimCharacter();

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeNPCSimulationComponent* GetSimulationComponent() const { return SimulationComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeWardrobeComponent* GetWardrobeComponent() const { return WardrobeComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeActivityComponent* GetActivityComponent() const { return ActivityComponent; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleActivityChanged(FName ActivityId);

	UFUNCTION()
	void HandleScheduleOccasion(const FApartmentLifeGameTime& NewTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeNPCSimulationComponent> SimulationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeWardrobeComponent> WardrobeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeActivityComponent> ActivityComponent;
};

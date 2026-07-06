// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeCleaningComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCleaningStateChanged);

/** Simulates dirt, wear, dishes, trash, and maintenance on apartment objects. */
UCLASS(ClassGroup = Apartment, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeCleaningComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Cleaning")
	void TickCleaningSimulation(float DeltaHours, float OccupantTidiness);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Cleaning")
	void CleanInstance(FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Cleaning")
	bool RequestMaintenance(FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Cleaning")
	void AutoCleanByTidiness(TArray<FApartmentLifePlacedFurnitureInstance>& Instances, float Tidiness, float Mood);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Cleaning")
	float GetApartmentCleanliness(const TArray<FApartmentLifePlacedFurnitureInstance>& Instances) const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Cleaning")
	FOnCleaningStateChanged OnCleaningStateChanged;
};

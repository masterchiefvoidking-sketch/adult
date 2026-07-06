// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeWorldSimDataAssets.h"
#include "ApartmentLifeWorldEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldEventTriggered, FName, EventId);

/**
 * Rolls and dispatches random world events (power outage, sales, promotions, festivals).
 * Events are data-driven via UApartmentLifeRandomEventData assets.
 */
UCLASS()
class APARTMENTLIFEWORLDSIM_API UApartmentLifeWorldEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Events")
	void RegisterEventData(UApartmentLifeRandomEventData* EventData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Events")
	void EvaluateDailyEvents();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Events")
	const UApartmentLifeRandomEventData* FindEventData(FName EventId) const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Events")
	FOnWorldEventTriggered OnWorldEventTriggered;

protected:
	UFUNCTION()
	void HandleDayAdvanced(const struct FApartmentLifeGameTime& NewTime);

	float ComputeEventProbability(const UApartmentLifeRandomEventData* EventData) const;

	UPROPERTY()
	TArray<TObjectPtr<UApartmentLifeRandomEventData>> RegisteredEvents;
};

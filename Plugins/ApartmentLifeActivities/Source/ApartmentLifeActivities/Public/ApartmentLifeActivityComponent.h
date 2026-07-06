// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeActivityData.h"
#include "ApartmentLifeActivityComponent.generated.h"

class UApartmentLifeDataRegistrySubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivityStarted, FName, ActivityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivityCompleted, FName, ActivityId);

/**
 * Executes data-driven daily activities. Animation playback (motion matching, IK, facial)
 * is triggered via Blueprint/AnimBP using MontageId from the activity data asset.
 */
UCLASS(ClassGroup = Activity, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEACTIVITIES_API UApartmentLifeActivityComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	bool StartActivity(FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	void CancelCurrentActivity();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	bool IsActivityActive() const { return bActivityActive; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	FName GetCurrentActivityId() const { return CurrentActivityId; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Activity")
	FOnActivityStarted OnActivityStarted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Activity")
	FOnActivityCompleted OnActivityCompleted;

protected:
	UFUNCTION()
	void HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime);

	void CompleteActivity();
	UApartmentLifeActivityData* ResolveActivity(FName ActivityId) const;
	bool CanStartActivity(const UApartmentLifeActivityData* Activity) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	bool bActivityActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	FName CurrentActivityId;

	int32 RemainingMinutes = 0;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeActivityData.h"
#include "ApartmentLifeActivityTypes.h"
#include "ApartmentLifeActivityComponent.generated.h"

class UApartmentLifeDataRegistrySubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivityStarted, FName, ActivityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivityCompleted, FName, ActivityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActivityCompletedWithResult, FName, ActivityId, const FApartmentLifeActivityCompletionResult&, Result);

/**
 * Executes data-driven daily activities with catalog-backed definitions,
 * cooldowns, and completion results.
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
	bool StartActivity(FName ActivityId, EApartmentLifeRoomType CurrentRoom = EApartmentLifeRoomType::LivingRoom);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	void CancelCurrentActivity();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	void ForceCompleteActivity();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	bool IsActivityActive() const { return bActivityActive; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	FName GetCurrentActivityId() const { return CurrentActivityId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	int32 GetRemainingMinutes() const { return RemainingMinutes; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	bool IsCooldownReady(FName ActivityId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	const FApartmentLifeActivityCompletionResult& GetLastCompletionResult() const { return LastCompletionResult; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Activity")
	FOnActivityStarted OnActivityStarted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Activity")
	FOnActivityCompleted OnActivityCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Activity")
	FOnActivityCompletedWithResult OnActivityCompletedWithResult;

protected:
	UFUNCTION()
	void HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime);

	void CompleteActivity();
	UApartmentLifeActivityData* ResolveActivity(FName ActivityId) const;
	bool CanStartActivity(FName ActivityId, EApartmentLifeRoomType CurrentRoom) const;
	void RecordCooldown(FName ActivityId);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	bool bActivityActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	FName CurrentActivityId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	FApartmentLifeActivityCompletionResult LastCompletionResult;

	int32 RemainingMinutes = 0;

	TMap<FName, int32> CooldownRemainingMinutes;
};

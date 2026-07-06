// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeActivityTypes.h"
#include "ApartmentLifeRoutineChainComponent.generated.h"

class UApartmentLifeActivityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoutineChainStepChanged, int32, StepIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoutineChainCompleted);

/** Runs ordered activity chains (morning/evening routines). */
UCLASS(ClassGroup = Activity, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEACTIVITIES_API UApartmentLifeRoutineChainComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Routine")
	bool StartRoutineChain(FName ChainId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Routine")
	void AdvanceRoutineChain();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Routine")
	void CancelRoutineChain();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Routine")
	bool IsRoutineActive() const { return bRoutineActive; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Routine")
	FName GetCurrentChainId() const { return CurrentChainId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Routine")
	int32 GetCurrentStepIndex() const { return CurrentStepIndex; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Routine")
	FName GetCurrentStepActivityId() const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Routine")
	FOnRoutineChainStepChanged OnRoutineChainStepChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Routine")
	FOnRoutineChainCompleted OnRoutineChainCompleted;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void StartCurrentStep();
	UApartmentLifeActivityComponent* GetActivityComponent() const;

	UFUNCTION()
	void HandleActivityCompleted(FName ActivityId);

	UPROPERTY()
	FApartmentLifeRoutineChainDefinition ActiveChain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Routine")
	bool bRoutineActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Routine")
	FName CurrentChainId;

	int32 CurrentStepIndex = INDEX_NONE;
};

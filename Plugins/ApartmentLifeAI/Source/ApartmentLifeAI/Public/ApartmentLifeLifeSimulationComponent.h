// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeAITypes.h"
#include "ApartmentLifeLifeSimulationComponent.generated.h"

class UApartmentLifeGameTimeSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScheduleActivityChanged, FName, ActivityId);

/**
 * Autonomous life simulation for NPCs. Manages work, income, bills, shopping, maintenance,
 * relationships, hobbies, and memories. Nothing waits for the player.
 */
UCLASS(ClassGroup = AI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAI_API UApartmentLifeLifeSimulationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UApartmentLifeLifeSimulationComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|AI")
	void TickLifeSimulation(const FApartmentLifeGameTime& CurrentTime);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|AI")
	FName GetCurrentScheduledActivity() const { return CurrentActivityId; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|AI")
	void PayMonthlyBills();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|AI")
	void AddMemory(const FApartmentLifeMemoryEntry& Memory);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|AI")
	void AdjustRelationship(FName OtherCharacterId, float AffinityDelta, float TrustDelta);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FApartmentLifeScheduleEntry> WeeklySchedule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FApartmentLifeEconomyState Economy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FApartmentLifePersonalityProfile Personality;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|AI")
	FOnScheduleActivityChanged OnScheduleActivityChanged;

	// IApartmentLifeSaveable
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UFUNCTION()
	void HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime);

	void EvaluateSchedule(const FApartmentLifeGameTime& CurrentTime);
	void SimulateDailyEconomy(const FApartmentLifeGameTime& CurrentTime);
	float ComputeLifestyleBudgetTier() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	FName CurrentActivityId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<FApartmentLifeRelationshipState> Relationships;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<FApartmentLifeMemoryEntry> Memories;

	int32 LastProcessedDay = -1;
};

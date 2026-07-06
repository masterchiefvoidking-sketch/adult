// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeGroomingRoutineComponent.generated.h"

class UApartmentLifeGroomingRoutineData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroomingStepChanged, EApartmentLifeGroomingStep, Step);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeGroomingRoutineComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Grooming")
	void StartRoutine(UApartmentLifeGroomingRoutineData* RoutineData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Grooming")
	void AdvanceStep();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Grooming")
	const FApartmentLifeGroomingRoutineState& GetRoutineState() const { return RoutineState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Grooming")
	float GetScheduleReadinessBonus() const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Grooming")
	FOnGroomingStepChanged OnGroomingStepChanged;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY()
	TObjectPtr<UApartmentLifeGroomingRoutineData> ActiveRoutine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grooming")
	FApartmentLifeGroomingRoutineState RoutineState;
};

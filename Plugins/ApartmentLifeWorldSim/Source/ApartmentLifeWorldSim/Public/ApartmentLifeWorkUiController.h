// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeWorkUiController.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeProgressionComponent;
class UApartmentLifeActivityComponent;

UCLASS(ClassGroup = UI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWORLDSIM_API UApartmentLifeWorkUiController : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		UApartmentLifeNPCSimulationComponent* InSimulation,
		UApartmentLifeProgressionComponent* InProgression,
		UApartmentLifeActivityComponent* InActivity);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	void OpenWorkSelection();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	void CloseWorkSelection();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	bool StartSelectedWork();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	void SelectWorkIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	void ShowWorkResult(const FApartmentLifeWorkSessionResult& Result);

	UFUNCTION()
	void HandleWorkSessionCompleted(const FApartmentLifeWorkSessionResult& Result);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Work|UI")
	void RefreshOverlay() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Work|UI")
	bool IsWorkMenuOpen() const { return bWorkMenuOpen; }

protected:
	UFUNCTION()
	void HandleActivityCompletedWithResult(FName ActivityId, const FApartmentLifeActivityCompletionResult& Result);

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeNPCSimulationComponent> Simulation;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeProgressionComponent> Progression;

	UPROPERTY()
	TWeakObjectPtr<UApartmentLifeActivityComponent> Activity;

	TArray<FApartmentLifeWorkTypeDefinition> VisibleWorkTypes;
	int32 SelectedWorkIndex = 0;
	bool bWorkMenuOpen = false;
	bool bShowingResult = false;
	FApartmentLifeWorkSessionResult LastResult;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCharacterBase.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeSimCharacter.generated.h"

class UApartmentLifeNPCSimulationComponent;
class UApartmentLifeWardrobeComponent;
class UApartmentLifeActivityComponent;
class UApartmentLifeInteractionComponent;
class UApartmentLifeInteractionSelectionComponent;
class UApartmentLifeAnimationComponent;
class UApartmentLifeYogaMinigameComponent;
class UApartmentLifeGroomingRoutineComponent;
class UApartmentLifeNPCStyleComponent;
class UApartmentLifeClothingFitComponent;
class UApartmentLifeBedroomRoutineComponent;
struct FApartmentLifeGameTime;
struct FApartmentLifeBodyFitProfile;

/** Fully composed sim character with world simulation, wardrobe, activities, and character pipeline. */
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

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeAnimationComponent* GetAnimationComponent() const { return AnimationComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeYogaMinigameComponent* GetYogaComponent() const { return YogaComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeGroomingRoutineComponent* GetGroomingComponent() const { return GroomingComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeNPCStyleComponent* GetNPCStyleComponent() const { return NPCStyleComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeClothingFitComponent* GetClothingFitComponent() const { return ClothingFitComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeConversationComponent* GetConversationComponent() const { return ConversationComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeInteractionSelectionComponent* GetInteractionSelectionComponent() const { return InteractionSelectionComponent; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	UApartmentLifeBedroomRoutineComponent* GetBedroomRoutineComponent() const { return BedroomRoutineComponent; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleActivityChanged(FName ActivityId);

	UFUNCTION()
	void HandleActivityStarted(FName ActivityId);

	UFUNCTION()
	void HandleActivityCompleted(FName ActivityId);

	UFUNCTION()
	void HandleScheduleOccasion(const FApartmentLifeGameTime& NewTime);

	UFUNCTION()
	void HandleCreatorStateUpdated(const struct FApartmentLifeCharacterCreatorState& State);

	UFUNCTION()
	void HandleBodyFitProfileUpdated(const FApartmentLifeBodyFitProfile& FitProfile);

	UFUNCTION()
	void HandlePurchasedItem(FName ItemId);

	UFUNCTION()
	void HandleWardrobeUpdated();

	void RefreshClothingFitFromBody();
	void RefreshNPCStyleFromSimulation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeNPCSimulationComponent> SimulationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeWardrobeComponent> WardrobeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeActivityComponent> ActivityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeAnimationComponent> AnimationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeYogaMinigameComponent> YogaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeGroomingRoutineComponent> GroomingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeNPCStyleComponent> NPCStyleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeClothingFitComponent> ClothingFitComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeConversationComponent> ConversationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeInteractionSelectionComponent> InteractionSelectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UApartmentLifeBedroomRoutineComponent> BedroomRoutineComponent;
};

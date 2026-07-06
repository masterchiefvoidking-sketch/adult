// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeAnimationLibrary.generated.h"

class UApartmentLifeAnimationComponent;
class UApartmentLifeBodyCustomizationComponent;
class UApartmentLifeNPCSimulationComponent;
class AApartmentLifeFurnitureActor;

UCLASS()
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeAnimationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static FName GetMontageIdForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static EApartmentLifeAnimationGroup GetAnimationGroupForGroomingStep(EApartmentLifeGroomingStep Step);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static EApartmentLifeFacialExpression GetFacialExpressionForActivity(FName ActivityId, float Mood, float Energy);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static EApartmentLifeMovementPersonality GetMovementPersonalityFromWalkStyle(EApartmentLifeWalkStyle WalkStyle);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static EApartmentLifeAnimationTransitionKind GetTransitionBetweenGroups(
		EApartmentLifeAnimationGroup FromGroup,
		EApartmentLifeAnimationGroup ToGroup);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static FName GetTransitionMontageId(
		EApartmentLifeAnimationGroup FromGroup,
		EApartmentLifeAnimationGroup ToGroup);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static bool ResolveAlignmentFromFurniture(
		AActor* FurnitureActor,
		FName ActivityId,
		FApartmentLifeInteractionAlignmentSet& OutAlignment);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static FApartmentLifeAnimationIkTargets BuildIkTargetsFromAlignment(const FApartmentLifeInteractionAlignmentSet& Alignment);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static bool AlignCharacterToInteraction(AActor* Character, const FApartmentLifeInteractionAlignmentSet& Alignment);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static void ApplyActivityAnimation(
		UApartmentLifeAnimationComponent* Animation,
		FName ActivityId,
		AActor* ContextActor,
		UApartmentLifeBodyCustomizationComponent* Body,
		UApartmentLifeNPCSimulationComponent* Simulation);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static void ApplyGroomingStepAnimation(
		UApartmentLifeAnimationComponent* Animation,
		EApartmentLifeGroomingStep Step);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	static void ApplyYogaPoseAnimation(
		UApartmentLifeAnimationComponent* Animation,
		FName PoseId,
		FName PoseMontageId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	static bool ShouldEnablePhysicsForGroup(EApartmentLifeAnimationGroup Group);
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeAnimationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationGroupChanged, EApartmentLifeAnimationGroup, NewGroup);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationStateChanged, const FApartmentLifeAnimationState&, State);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeAnimationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void SetAnimationGroup(EApartmentLifeAnimationGroup Group);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void TransitionToGroup(EApartmentLifeAnimationGroup Group, EApartmentLifeAnimationTransitionKind TransitionKind);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void PlayActivityAnimation(
		FName ActivityId,
		EApartmentLifeAnimationGroup Group,
		const FApartmentLifeInteractionAlignmentSet& Alignment,
		float Mood,
		float Energy);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void PlayYogaPose(FName PoseId, FName PoseMontageId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void SetFacialExpression(EApartmentLifeFacialExpression Expression);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void ApplyMovementStyles(
		EApartmentLifeMovementPersonality Personality,
		FName WalkStyleId,
		FName IdleStyleId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void ResetToIdle();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void SetMontageId(FName MontageId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void MarkCompletedAnimationEvent(FName EventId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void RestoreAnimationAfterLoad();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	const FApartmentLifeAnimationState& GetAnimationState() const { return AnimationState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	const FApartmentLifeFitnessProgress& GetFitnessProgress() const { return FitnessProgress; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Animation")
	FOnAnimationGroupChanged OnAnimationGroupChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Animation")
	FOnAnimationStateChanged OnAnimationStateChanged;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void BroadcastState();
	void ApplyAlignment(const FApartmentLifeInteractionAlignmentSet& Alignment);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FApartmentLifeAnimationState AnimationState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FApartmentLifeFitnessProgress FitnessProgress;

	EApartmentLifeAnimationGroup PreviousGroup = EApartmentLifeAnimationGroup::Idle;
};

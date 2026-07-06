// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeAnimationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimationGroupChanged, EApartmentLifeAnimationGroup, NewGroup);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeAnimationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Animation")
	void SetAnimationGroup(EApartmentLifeAnimationGroup Group);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	const FApartmentLifeAnimationState& GetAnimationState() const { return AnimationState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Animation")
	const FApartmentLifeFitnessProgress& GetFitnessProgress() const { return FitnessProgress; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Animation")
	FOnAnimationGroupChanged OnAnimationGroupChanged;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FApartmentLifeAnimationState AnimationState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FApartmentLifeFitnessProgress FitnessProgress;
};

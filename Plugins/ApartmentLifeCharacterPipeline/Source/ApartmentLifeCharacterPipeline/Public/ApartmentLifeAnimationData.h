// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeAnimationData.generated.h"

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeAnimationSetData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("AnimationSet")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EApartmentLifeAnimationGroup Group = EApartmentLifeAnimationGroup::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName MontageId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName BlendSpaceId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName MotionMatchingDatabaseId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bRequiresFullBodyIK = false;
};

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeYogaPoseData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("YogaPose")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yoga")
	FName PoseMontageId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yoga")
	float TargetHoldSeconds = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yoga")
	float Difficulty = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yoga")
	float FlexibilityReward = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Yoga")
	float StressReduction = 8.f;
};

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeGroomingRoutineData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("GroomingRoutine")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grooming")
	TArray<EApartmentLifeGroomingStep> Steps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grooming")
	TMap<EApartmentLifeGroomingStep, FName> StepActivityIds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grooming")
	float HygieneGainPerStep = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grooming")
	float MoodGainPerStep = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grooming")
	float ConfidenceGainPerStep = 2.f;
};

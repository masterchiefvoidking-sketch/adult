// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeCharacterPipelineTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeAnimationGroup : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walking			UMETA(DisplayName = "Walking"),
	Running			UMETA(DisplayName = "Running"),
	Sitting			UMETA(DisplayName = "Sitting"),
	CouchSitting	UMETA(DisplayName = "Couch Sitting"),
	ChairSitting	UMETA(DisplayName = "Chair Sitting"),
	FloorSitting	UMETA(DisplayName = "Floor Sitting"),
	LyingDown		UMETA(DisplayName = "Lying Down"),
	Sleeping		UMETA(DisplayName = "Sleeping"),
	Cooking			UMETA(DisplayName = "Cooking"),
	Eating			UMETA(DisplayName = "Eating"),
	Cleaning		UMETA(DisplayName = "Cleaning"),
	Laundry			UMETA(DisplayName = "Laundry"),
	Reading			UMETA(DisplayName = "Reading"),
	Gaming			UMETA(DisplayName = "Gaming"),
	ComputerUse		UMETA(DisplayName = "Computer Use"),
	WatchingTV		UMETA(DisplayName = "Watching TV"),
	Yoga			UMETA(DisplayName = "Yoga"),
	Stretching		UMETA(DisplayName = "Stretching"),
	Exercising		UMETA(DisplayName = "Exercising"),
	Dancing			UMETA(DisplayName = "Dancing"),
	Dressing		UMETA(DisplayName = "Dressing"),
	Grooming		UMETA(DisplayName = "Grooming"),
	ShowerRoutine	UMETA(DisplayName = "Shower Routine"),
	MirrorRoutine	UMETA(DisplayName = "Mirror Routine"),
	Conversation	UMETA(DisplayName = "Conversation")
};

UENUM(BlueprintType)
enum class EApartmentLifeGroomingStep : uint8
{
	Shower		UMETA(DisplayName = "Shower"),
	BrushTeeth	UMETA(DisplayName = "Brush Teeth"),
	WashFace	UMETA(DisplayName = "Wash Face"),
	HairCare	UMETA(DisplayName = "Hair Care"),
	Makeup		UMETA(DisplayName = "Makeup"),
	SkinCare	UMETA(DisplayName = "Skin Care"),
	ChangeClothes UMETA(DisplayName = "Change Clothes"),
	MirrorCheck	UMETA(DisplayName = "Mirror Check"),
	Laundry		UMETA(DisplayName = "Laundry"),
	ClosetOrg	UMETA(DisplayName = "Closet Organization")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeAnimationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	EApartmentLifeAnimationGroup CurrentGroup = EApartmentLifeAnimationGroup::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName MontageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName MotionMatchingDatabaseId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bUseFullBodyIK = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bEnableFacialAnimation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bEnableEyeTracking = true;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeYogaSessionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	FName CurrentPoseId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga", meta = (ClampMin = "0", ClampMax = "100"))
	float PoseMatchScore = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga", meta = (ClampMin = "0", ClampMax = "100"))
	float BalanceMeter = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga", meta = (ClampMin = "0", ClampMax = "100"))
	float BreathingRhythm = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float FlexibilityXP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float FitnessXP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	bool bOnYogaMat = false;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeGroomingRoutineState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grooming")
	TArray<EApartmentLifeGroomingStep> PreferredStepOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grooming", meta = (ClampMin = "0", ClampMax = "100"))
	float Hygiene = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grooming", meta = (ClampMin = "0", ClampMax = "100"))
	float Confidence = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grooming")
	EApartmentLifeGroomingStep CurrentStep = EApartmentLifeGroomingStep::Shower;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeFitnessProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fitness")
	float FlexibilityLevel = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fitness")
	float CardioLevel = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fitness")
	float TotalFitnessXP = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeNPCStyleContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	float Income = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FName CareerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	float Mood = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	bool bInRelationship = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	EApartmentLifeOccasion Occasion = EApartmentLifeOccasion::Everyday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FApartmentLifeWeatherState Weather;
};

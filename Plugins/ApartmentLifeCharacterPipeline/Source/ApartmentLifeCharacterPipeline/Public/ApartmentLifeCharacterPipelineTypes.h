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
	TurnInPlace		UMETA(DisplayName = "Turn In Place"),
	Running			UMETA(DisplayName = "Running"),
	Stand			UMETA(DisplayName = "Stand"),
	Sitting			UMETA(DisplayName = "Sitting"),
	CouchSitting	UMETA(DisplayName = "Couch Sitting"),
	ChairSitting	UMETA(DisplayName = "Chair Sitting"),
	FloorSitting	UMETA(DisplayName = "Floor Sitting"),
	LyingDown		UMETA(DisplayName = "Lying Down"),
	Sleeping		UMETA(DisplayName = "Sleeping"),
	WakeUp			UMETA(DisplayName = "Wake Up"),
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
enum class EApartmentLifeFacialExpression : uint8
{
	Neutral		UMETA(DisplayName = "Neutral"),
	Happy		UMETA(DisplayName = "Happy"),
	Tired		UMETA(DisplayName = "Tired"),
	Focused		UMETA(DisplayName = "Focused"),
	Relaxed		UMETA(DisplayName = "Relaxed"),
	Stressed	UMETA(DisplayName = "Stressed"),
	Embarrassed UMETA(DisplayName = "Embarrassed"),
	Confident	UMETA(DisplayName = "Confident"),
	Sleepy		UMETA(DisplayName = "Sleepy")
};

UENUM(BlueprintType)
enum class EApartmentLifeMovementPersonality : uint8
{
	Soft		UMETA(DisplayName = "Soft"),
	Confident	UMETA(DisplayName = "Confident"),
	Energetic	UMETA(DisplayName = "Energetic"),
	Relaxed		UMETA(DisplayName = "Relaxed"),
	Shy			UMETA(DisplayName = "Shy"),
	Elegant		UMETA(DisplayName = "Elegant"),
	Sporty		UMETA(DisplayName = "Sporty")
};

UENUM(BlueprintType)
enum class EApartmentLifeAnimationTransitionKind : uint8
{
	Instant			UMETA(DisplayName = "Instant"),
	Blend			UMETA(DisplayName = "Blend"),
	MontageBridge	UMETA(DisplayName = "Montage Bridge")
};

UENUM(BlueprintType)
enum class EApartmentLifeInteractionMarkerType : uint8
{
	Entry			UMETA(DisplayName = "Entry"),
	Facing			UMETA(DisplayName = "Facing"),
	HandsTarget		UMETA(DisplayName = "Hands Target"),
	FeetTarget		UMETA(DisplayName = "Feet Target"),
	SeatTarget		UMETA(DisplayName = "Seat Target"),
	CameraFocus		UMETA(DisplayName = "Camera Focus"),
	Exit			UMETA(DisplayName = "Exit")
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
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeAnimationIkTargets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	bool bEnableFootIk = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	bool bEnableHandIk = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	bool bEnableHeadLookAt = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	bool bEnableEyeTracking = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector LeftFootTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector RightFootTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector LeftHandTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector RightHandTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FVector HeadLookAtTarget = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeInteractionAlignmentSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	bool bHasAlignment = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FTransform EntryTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FTransform ExitTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FVector FacingDirection = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FVector HandsTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FVector FeetTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FVector SeatTarget = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FVector CameraFocusPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FName FurnitureActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alignment")
	FName InteractionSocketName;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	EApartmentLifeFacialExpression FacialExpression = EApartmentLifeFacialExpression::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	EApartmentLifeMovementPersonality MovementPersonality = EApartmentLifeMovementPersonality::Soft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName WalkStyleId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName IdleStyleId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName ActiveActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName TransitionMontageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	EApartmentLifeAnimationTransitionKind TransitionKind = EApartmentLifeAnimationTransitionKind::Blend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FApartmentLifeInteractionAlignmentSet Alignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FApartmentLifeAnimationIkTargets IkTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bPhysicsHairEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bPhysicsClothEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName LastCompletedAnimationEvent;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	int32 PoseStreak = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float SessionDurationMinutes = 0.f;
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

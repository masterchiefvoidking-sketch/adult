// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeActivityData.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeActivityCategory : uint8
{
	Cooking			UMETA(DisplayName = "Cooking"),
	Eating			UMETA(DisplayName = "Eating"),
	Reading			UMETA(DisplayName = "Reading"),
	Entertainment	UMETA(DisplayName = "Entertainment"),
	Cleaning		UMETA(DisplayName = "Cleaning"),
	Laundry			UMETA(DisplayName = "Laundry"),
	Fitness			UMETA(DisplayName = "Fitness"),
	Shopping		UMETA(DisplayName = "Shopping"),
	Work			UMETA(DisplayName = "Work"),
	Decorating		UMETA(DisplayName = "Decorating"),
	Gardening		UMETA(DisplayName = "Gardening"),
	Relaxing		UMETA(DisplayName = "Relaxing"),
	Grooming		UMETA(DisplayName = "Grooming"),
	Hygiene			UMETA(DisplayName = "Hygiene"),
	BodilyFunction	UMETA(DisplayName = "Bodily Function"),
	Intimate		UMETA(DisplayName = "Intimate"),
	Social			UMETA(DisplayName = "Social"),
	Sleep			UMETA(DisplayName = "Sleep")
};

/** Data asset defining a daily activity: duration, animations, interactions, and requirements. */
UCLASS(BlueprintType)
class APARTMENTLIFEACTIVITIES_API UApartmentLifeActivityData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("Activity")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	EApartmentLifeActivityCategory Category = EApartmentLifeActivityCategory::Relaxing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	int32 DurationMinutes = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	FName MontageId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	FName RequiredFurnitureTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	bool bRequiresPrivacy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	bool bRequiresExplicitContent = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	TArray<FName> NeedModifiers;
};

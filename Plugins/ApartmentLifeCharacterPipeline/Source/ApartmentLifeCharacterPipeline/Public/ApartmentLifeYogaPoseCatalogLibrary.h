// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeYogaPoseCatalogLibrary.generated.h"

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTERPIPELINE_API FApartmentLifeBuiltinYogaPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	FName PoseId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	FName PoseMontageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float Difficulty = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float FlexibilityReward = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yoga")
	float StressReduction = 8.f;
};

UCLASS()
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeYogaPoseCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Yoga")
	static TArray<FApartmentLifeBuiltinYogaPose> GetBuiltinPoses();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Yoga")
	static bool TryGetBuiltinPose(FName PoseId, FApartmentLifeBuiltinYogaPose& OutPose);
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeAnimationData.h"
#include "ApartmentLifeCharacterPipelineLibrary.generated.h"

class UApartmentLifeClothingItemData;

UCLASS()
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeCharacterPipelineLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Character Pipeline")
	static float ScoreClothingForNPCStyle(const UApartmentLifeClothingItemData* Item, const FApartmentLifeNPCStyleContext& Context, const FApartmentLifeWardrobeStyleProfile& StyleProfile);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Character Pipeline")
	static FApartmentLifeYogaSessionState UpdateYogaPoseMatch(const FApartmentLifeYogaSessionState& Current, const UApartmentLifeYogaPoseData* Pose, float PlayerInputAccuracy, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Character Pipeline")
	static FApartmentLifeGroomingRoutineState AdvanceGroomingStep(const FApartmentLifeGroomingRoutineState& State, const UApartmentLifeGroomingRoutineData* Routine);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character Pipeline")
	static FName GetAnimationIdForGroup(EApartmentLifeAnimationGroup Group);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Character Pipeline")
	static EApartmentLifeAnimationGroup GetAnimationGroupForActivity(FName ActivityId);
};

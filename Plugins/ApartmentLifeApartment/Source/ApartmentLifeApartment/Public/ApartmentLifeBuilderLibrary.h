// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeFurnitureData.h"
#include "ApartmentLifeBuilderLibrary.generated.h"

/** Stateless builder math: placement, scoring, decoration AI. */
UCLASS()
class APARTMENTLIFEAPARTMENT_API UApartmentLifeBuilderLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static FTransform SnapTransformToGrid(const FTransform& Transform, float GridSize);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static bool ValidatePlacement(
		const UApartmentLifeFurnitureItemData* ItemData,
		const FTransform& Transform,
		EApartmentLifeRoomType Room,
		const FBox& RoomBounds,
		const TArray<FApartmentLifePlacedFurnitureInstance>& ExistingPlacements);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Builder")
	static FApartmentLifeRoomScore CalculateRoomScore(
		EApartmentLifeRoomType Room,
		const TArray<FApartmentLifePlacedFurnitureInstance>& Placements,
		const TArray<UApartmentLifeFurnitureItemData*>& ResolvedItems,
		const TArray<EApartmentLifeStyleTag>& PreferredStyles,
		float BaseLighting,
		float ApartmentNaturalLight);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Builder")
	static float ScoreFurnitureForDecorator(
		const UApartmentLifeFurnitureItemData* Item,
		const FApartmentLifeDecorationProfile& Profile,
		float AvailableBudget,
		float RemainingFloorSpace,
		EApartmentLifeRoomType TargetRoom);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Builder")
	static TArray<FApartmentLifePlacedFurnitureInstance> RecommendDecorations(
		const FApartmentLifeDecorationProfile& Profile,
		float AvailableBudget,
		float RemainingFloorSpace,
		EApartmentLifeRoomType TargetRoom,
		const TArray<UApartmentLifeFurnitureItemData*>& Catalog);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Builder")
	static void ApplyWearAndDirt(FApartmentLifeObjectCondition& Condition, float DeltaHours, float TidinessFactor);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static float GetSellPrice(const UApartmentLifeFurnitureItemData* Item, const FApartmentLifeObjectCondition& Condition);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static TArray<EApartmentLifeFurnitureInteraction> GetInteractionsForCategory(EApartmentLifeFurnitureCategory Category);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static FName GetActivityIdForInteraction(EApartmentLifeFurnitureInteraction Interaction);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static FName GetDefaultSocketForInteraction(EApartmentLifeFurnitureInteraction Interaction);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Builder")
	static FText GetDisplayNameForInteraction(EApartmentLifeFurnitureInteraction Interaction);
};

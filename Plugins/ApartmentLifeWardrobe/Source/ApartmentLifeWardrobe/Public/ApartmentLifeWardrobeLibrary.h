// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeLibrary.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeNPCSimulationComponent;

UCLASS()
class APARTMENTLIFEWARDROBE_API UApartmentLifeWardrobeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	static TArray<FName> FilterAndSortCloset(
		const UApartmentLifeWardrobeComponent* Wardrobe,
		const FApartmentLifeWardrobeFilterQuery& Query);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	static void RecommendOutfitForActivity(UApartmentLifeWardrobeComponent* Wardrobe, FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	static bool ShouldAutoRecommendOutfitForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	static FApartmentLifeOutfitMoodEffect ComputeOutfitMoodEffect(
		const UApartmentLifeWardrobeComponent* Wardrobe,
		FName ActivityId = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	static void ApplyOutfitMoodEffect(UApartmentLifeNPCSimulationComponent* Simulation, const FApartmentLifeOutfitMoodEffect& Effect);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	static bool CanAffordItem(const UApartmentLifeNPCSimulationComponent* Simulation, FName ItemId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	static float ScoreItemForStyleTags(FName ItemId, const TArray<FName>& PreferredTags);
};

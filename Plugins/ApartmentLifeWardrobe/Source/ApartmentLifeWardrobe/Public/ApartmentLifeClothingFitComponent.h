// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeClothingFitComponent.generated.h"

class UApartmentLifeWardrobeComponent;
class UApartmentLifeClothingItemData;

/** Applies body-driven clothing fit offsets to equipped meshes. */
UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWARDROBE_API UApartmentLifeClothingFitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Wardrobe")
	void RefreshClothingFit(const FApartmentLifeBodyFitProfile& BodyFit);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Wardrobe")
	bool DoesClothingFit(const UApartmentLifeClothingItemData* Item, const FApartmentLifeBodyFitProfile& BodyFit) const;
};

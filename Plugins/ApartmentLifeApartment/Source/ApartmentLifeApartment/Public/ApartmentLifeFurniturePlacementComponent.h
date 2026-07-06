// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ApartmentLifeFurnitureData.h"
#include "ApartmentLifeFurniturePlacementComponent.generated.h"

class UApartmentLifeFurnitureItemData;
class UApartmentLifeDataRegistrySubsystem;

UCLASS(ClassGroup = Apartment, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeFurniturePlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	AActor* SpawnFurnitureInstance(const FApartmentLifeFurniturePlacement& Placement);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void DestroyFurnitureInstance(const FGuid& PlacementGuid);

protected:
	UPROPERTY()
	TMap<FGuid, TObjectPtr<AActor>> SpawnedFurniture;

	UApartmentLifeFurnitureItemData* ResolveFurnitureData(FName FurnitureItemId) const;
};

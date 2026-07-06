// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeFurniturePlacementComponent.generated.h"

class AApartmentLifeFurnitureActor;
class UApartmentLifeFurnitureItemData;
class UApartmentLifeDataRegistrySubsystem;

UCLASS(ClassGroup = Apartment, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeFurniturePlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	AApartmentLifeFurnitureActor* SpawnFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void UpdateFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void DestroyFurnitureInstance(const FGuid& InstanceId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	AApartmentLifeFurnitureActor* GetSpawnedFurnitureActor(const FGuid& InstanceId) const;

protected:
	UPROPERTY()
	TMap<FGuid, TObjectPtr<AApartmentLifeFurnitureActor>> SpawnedFurniture;

	UApartmentLifeFurnitureItemData* ResolveFurnitureData(FName FurnitureItemId) const;
};

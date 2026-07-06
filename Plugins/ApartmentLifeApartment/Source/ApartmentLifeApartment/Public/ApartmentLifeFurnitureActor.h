// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeFurnitureActor.generated.h"

class UStaticMeshComponent;
class UApartmentLifeFurnitureItemData;

/** Runtime spawned furniture with interaction points. */
UCLASS()
class APARTMENTLIFEAPARTMENT_API AApartmentLifeFurnitureActor : public AActor
{
	GENERATED_BODY()

public:
	AApartmentLifeFurnitureActor();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Furniture")
	void InitializeFromInstance(const FApartmentLifePlacedFurnitureInstance& Instance, UApartmentLifeFurnitureItemData* ItemData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Furniture")
	bool TryInteract(EApartmentLifeFurnitureInteraction Interaction, FName& OutActivityId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Furniture")
	FGuid GetInstanceId() const { return InstanceId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Furniture")
	TArray<EApartmentLifeFurnitureInteraction> GetAvailableInteractions() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Furniture")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Furniture")
	FGuid InstanceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Furniture")
	TArray<FApartmentLifeInteractionPoint> InteractionPoints;
};

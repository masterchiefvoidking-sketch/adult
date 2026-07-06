// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeFurnitureData.h"
#include "ApartmentLifeApartmentUnit.generated.h"

class UApartmentLifeFurniturePlacementComponent;

/**
 * Root actor for a customizable apartment unit. Owns room volumes and furniture layout state.
 */
UCLASS()
class APARTMENTLIFEAPARTMENT_API AApartmentLifeApartmentUnit : public AActor, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	AApartmentLifeApartmentUnit();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	void PlaceFurniture(const FApartmentLifeFurniturePlacement& Placement);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Apartment")
	bool RemoveFurniture(const FGuid& PlacementGuid);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Apartment")
	const TArray<FApartmentLifeFurniturePlacement>& GetLayout() const { return FurnitureLayout; }

	// IApartmentLifeSaveable
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TObjectPtr<UApartmentLifeFurniturePlacementComponent> PlacementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Apartment")
	TArray<FApartmentLifeFurniturePlacement> FurnitureLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	FName ApartmentId;
};

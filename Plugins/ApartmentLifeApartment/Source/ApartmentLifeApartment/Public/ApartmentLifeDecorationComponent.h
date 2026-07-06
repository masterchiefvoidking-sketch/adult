// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeDecorationComponent.generated.h"

class AApartmentLifeApartmentUnit;
class UApartmentLifeFurnitureItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecorationPlaced, FGuid, InstanceId);

/** NPC decoration AI — purchases and places furniture based on personality, income, and space. */
UCLASS(ClassGroup = Apartment, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeDecorationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Decoration")
	void RunDecorationPass(float AvailableBudget);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Decoration")
	bool ApplyUpgrade(FName UpgradeAssetId, float AvailableBudget);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Decoration")
	void SetDecorationProfile(const FApartmentLifeDecorationProfile& Profile);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Decoration")
	const FApartmentLifeDecorationProfile& GetDecorationProfile() const { return Profile; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Decoration")
	FOnDecorationPlaced OnDecorationPlaced;

protected:
	AApartmentLifeApartmentUnit* GetApartmentUnit() const;
	TArray<UApartmentLifeFurnitureItemData*> GetFurnitureCatalog() const;
	float GetRemainingFloorSpace() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	FApartmentLifeDecorationProfile Profile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decoration")
	TArray<FName> AppliedUpgradeHistory;
};

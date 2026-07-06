// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ApartmentLifeDataDriven.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UApartmentLifeDataDriven : public UInterface
{
	GENERATED_BODY()
};

/** Marks objects whose runtime behavior is fully driven by primary data assets. */
class APARTMENTLIFECORE_API IApartmentLifeDataDriven
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Data")
	FName GetDataAssetId() const;
};

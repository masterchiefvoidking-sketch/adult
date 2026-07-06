// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ApartmentLifeSaveable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UApartmentLifeSaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by actors and components that participate in save/load.
 * All gameplay state should serialize through this interface — never hardcode persistence.
 */
class APARTMENTLIFECORE_API IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Save")
	FString GetSaveId() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Save")
	void CaptureSaveData(TMap<FString, FString>& OutData) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Apartment Life|Save")
	void RestoreSaveData(const TMap<FString, FString>& InData);
};

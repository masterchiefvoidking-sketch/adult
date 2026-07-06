// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeActivityFallbackLibrary.generated.h"

UCLASS()
class APARTMENTLIFEACTIVITIES_API UApartmentLifeActivityFallbackLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static int32 GetBuiltinDurationMinutes(FName ActivityId);
};

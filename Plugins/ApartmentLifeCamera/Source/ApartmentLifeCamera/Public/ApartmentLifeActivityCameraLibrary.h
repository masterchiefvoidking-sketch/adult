// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeCameraTypes.h"
#include "ApartmentLifeActivityCameraLibrary.generated.h"

UCLASS()
class APARTMENTLIFECAMERA_API UApartmentLifeActivityCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	static FApartmentLifeActivityCameraFrame GetFrameForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	static EApartmentLifePrimaryCameraMode GetPrimaryModeForActivity(FName ActivityId);
};

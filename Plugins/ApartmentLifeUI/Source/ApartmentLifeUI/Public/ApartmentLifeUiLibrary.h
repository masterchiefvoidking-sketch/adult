// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeUiLibrary.generated.h"

UCLASS()
class APARTMENTLIFEUI_API UApartmentLifeUiLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** When Slate UI is active, feature controllers should skip on-screen debug overlays. */
	UFUNCTION(BlueprintPure, Category = "Apartment Life|UI", meta = (WorldContext = "WorldContextObject"))
	static bool ShouldSuppressDebugOverlay(const UObject* WorldContextObject);
};

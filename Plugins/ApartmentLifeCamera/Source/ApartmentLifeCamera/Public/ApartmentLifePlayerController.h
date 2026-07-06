// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ApartmentLifePlayerController.generated.h"

class AApartmentLifeCameraPawn;

UCLASS()
class APARTMENTLIFECAMERA_API AApartmentLifePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AApartmentLifePlayerController();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	AApartmentLifeCameraPawn* GetCameraPawn() const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetFocusTarget(AActor* NewTarget);
};

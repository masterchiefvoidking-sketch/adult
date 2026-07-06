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

	/** Tear down wardrobe camera, activity, and UI when supported by the game controller. */
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Session")
	virtual void CloseWardrobeSession();

	/** Tear down build mode camera state when supported by the game controller. */
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Session")
	virtual void CloseBuildModeSession();

	/** End an active player conversation and restore camera when supported. */
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Session")
	virtual void CloseConversationSession();
};

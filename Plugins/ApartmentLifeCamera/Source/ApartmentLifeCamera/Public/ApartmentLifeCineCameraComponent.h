// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraComponent.h"
#include "ApartmentLifeCineCameraComponent.generated.h"

/** Cine camera with adjustable FOV and photo-mode depth of field controls. */
UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECAMERA_API UApartmentLifeCineCameraComponent : public UCineCameraComponent
{
	GENERATED_BODY()

public:
	UApartmentLifeCineCameraComponent();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetFieldOfViewSmooth(float NewFOV, float InterpSpeed = 6.f);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetPhotoModeEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetDepthOfFieldSettings(float FocalDistance, float Aperture, float BlurAmount);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	float DesiredFOV = 75.f;
	float FOVInterpSpeed = 6.f;
	bool bPhotoModeActive = false;
};

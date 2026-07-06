// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCineCameraComponent.h"

UApartmentLifeCineCameraComponent::UApartmentLifeCineCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	FieldOfView = 75.f;
	DesiredFOV = FieldOfView;
}

void UApartmentLifeCineCameraComponent::SetFieldOfViewSmooth(float NewFOV, float InterpSpeed)
{
	DesiredFOV = FMath::Clamp(NewFOV, 20.f, 120.f);
	FOVInterpSpeed = FMath::Max(InterpSpeed, 0.1f);
}

void UApartmentLifeCineCameraComponent::SetPhotoModeEnabled(bool bEnabled)
{
	bPhotoModeActive = bEnabled;
	CurrentFocusMethod = bEnabled ? ECameraFocusMethod::Manual : ECameraFocusMethod::DoNotOverride;
}

void UApartmentLifeCineCameraComponent::SetDepthOfFieldSettings(float FocalDistance, float Aperture, float BlurAmount)
{
	CurrentAperture = FMath::Clamp(Aperture, 0.1f, 32.f);
	ManualFocusDistance = FMath::Max(FocalDistance, 1.f);
	CurrentFocalLength = FMath::Lerp(24.f, 135.f, FMath::Clamp(BlurAmount, 0.f, 1.f));
}

void UApartmentLifeCineCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FieldOfView = FMath::FInterpTo(FieldOfView, DesiredFOV, DeltaTime, FOVInterpSpeed);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

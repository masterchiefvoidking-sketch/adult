// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeOrbitSpringArmComponent.h"

UApartmentLifeOrbitSpringArmComponent::UApartmentLifeOrbitSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bDoCollisionTest = true;
	ProbeSize = 12.f;
	TargetArmLength = 400.f;
	DesiredArmLength = TargetArmLength;
	bEnableCameraLag = true;
	CameraLagSpeed = 10.f;
	bEnableCameraRotationLag = true;
	CameraRotationLagSpeed = 12.f;
}

void UApartmentLifeOrbitSpringArmComponent::SetTargetArmLengthSmooth(float NewLength, float InterpSpeed)
{
	DesiredArmLength = FMath::Max(NewLength, 50.f);
	ArmLengthInterpSpeed = FMath::Max(InterpSpeed, 0.1f);
}

void UApartmentLifeOrbitSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TargetArmLength = FMath::FInterpTo(TargetArmLength, DesiredArmLength, DeltaTime, ArmLengthInterpSpeed);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

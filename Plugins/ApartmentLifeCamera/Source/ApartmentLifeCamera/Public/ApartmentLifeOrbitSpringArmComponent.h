// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ApartmentLifeOrbitSpringArmComponent.generated.h"

/** Spring arm with smooth interpolation and configurable collision probe for interior inspection. */
UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECAMERA_API UApartmentLifeOrbitSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UApartmentLifeOrbitSpringArmComponent();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetTargetArmLengthSmooth(float NewLength, float InterpSpeed = 8.f);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	float DesiredArmLength = 400.f;
	float ArmLengthInterpSpeed = 8.f;
};

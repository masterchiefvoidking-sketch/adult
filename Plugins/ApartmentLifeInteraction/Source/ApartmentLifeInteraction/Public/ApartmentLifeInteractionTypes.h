// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeInteractionTypes.generated.h"

/** Data-driven interaction descriptor for the vertical slice loop. */
USTRUCT(BlueprintType)
struct APARTMENTLIFEINTERACTION_API FApartmentLifeInteractionDescriptor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FName InteractionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	int32 DurationMinutes = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText StatEffectSummary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FVector FocusOffset = FVector::ZeroVector;
};

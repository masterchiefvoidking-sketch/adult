// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorLibrary.generated.h"

class UApartmentLifeCharacterCreatorComponent;
class UApartmentLifeWardrobeComponent;
class UApartmentLifeClothingFitComponent;
class UApartmentLifeBodyCustomizationComponent;

UCLASS()
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeCharacterCreatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	static FApartmentLifeCreatorCompatibilityReport EvaluateWardrobeCompatibility(AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator")
	static void PreviewAnimationGroup(AActor* Character, FName AnimationGroupName);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FName> GetCreatorPreviewAnimationGroups();
};

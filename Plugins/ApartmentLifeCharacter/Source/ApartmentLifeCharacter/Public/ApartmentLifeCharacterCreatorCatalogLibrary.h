// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.generated.h"

UCLASS()
class APARTMENTLIFECHARACTER_API UApartmentLifeCharacterCreatorCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static constexpr int32 CreatorCompatibilityVersion = 1;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static FApartmentLifeCharacterCreatorState GetDefaultCreatorState();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FName> GetBuiltinHairstyleIds();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FApartmentLifeColorPalette> GetBuiltinColorPalettes();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FName> GetBuiltinVoicePresetIds();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FName> GetBuiltinPersonalityPresetIds();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FApartmentLifeCharacterCreatorPreset> GetBuiltinPresets();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static TArray<FApartmentLifeMorphSlider> GetDefaultFaceMorphsForRegion(EApartmentLifeFaceRegion Region);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static FLinearColor GetRandomBelievableSkinTone();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static FLinearColor GetRandomBelievableHairColor();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Creator")
	static FLinearColor GetRandomBelievableEyeColor();
};

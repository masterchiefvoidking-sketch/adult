// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorPresetLibrary.generated.h"

UCLASS()
class APARTMENTLIFECHARACTER_API UApartmentLifeCharacterCreatorPresetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|Presets")
	static bool SavePresetToDisk(const FApartmentLifeCharacterCreatorPreset& Preset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|Presets")
	static bool LoadPresetFromDisk(FName PresetId, FApartmentLifeCharacterCreatorPreset& OutPreset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|Presets")
	static bool DeletePresetFromDisk(FName PresetId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|Presets")
	static TArray<FApartmentLifeCharacterCreatorPreset> GetAllPresets(bool bIncludeBuiltin = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Creator|Presets")
	static bool ApplyPresetToState(
		const FApartmentLifeCharacterCreatorPreset& Preset,
		FApartmentLifeCharacterCreatorState& InOutState,
		EApartmentLifeCreatorPresetType OverrideType = EApartmentLifeCreatorPresetType::FullCharacter);
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorCatalogData.generated.h"

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTER_API UApartmentLifeHairstyleData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("Hairstyle")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hair")
	USkeletalMesh* HairMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hair")
	bool bEnableHairPhysics = true;
};

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTER_API UApartmentLifeVoicePresetData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("VoicePreset")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	float Pitch = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	float Speed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	FName VoiceBankId;
};

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTER_API UApartmentLifePersonalityPresetData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("PersonalityPreset")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Personality")
	FName DefaultWalkStyleId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Personality")
	FName DefaultIdleStyleId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Personality")
	TArray<FName> StylePreferenceIds;
};

UCLASS(BlueprintType)
class APARTMENTLIFECHARACTER_API UApartmentLifeMorphCatalogData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("MorphCatalog")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Morphs")
	TArray<FApartmentLifeMorphSlider> DefaultFaceMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Morphs")
	TArray<FApartmentLifeMorphSlider> DefaultBodyMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	FApartmentLifeCharacterCreatorState DefaultCreatorState;
};

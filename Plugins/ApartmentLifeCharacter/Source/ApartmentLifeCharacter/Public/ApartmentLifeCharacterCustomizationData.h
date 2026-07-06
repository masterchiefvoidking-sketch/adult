// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeCharacterCustomizationData.generated.h"

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeMorphSlider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Morph")
	FName MorphTargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Morph", meta = (ClampMin = "0", ClampMax = "1"))
	float Value = 0.5f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeColorCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FName MaterialParameterName = NAME_None;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeCharacterAppearanceState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float HeightCm = 165.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	TArray<FApartmentLifeMorphSlider> BodyProportions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> FaceMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skin")
	FApartmentLifeColorCustomization SkinTone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
	FApartmentLifeColorCustomization EyeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hair")
	FName HairstyleId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hair")
	FApartmentLifeColorCustomization HairColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Makeup")
	TArray<FApartmentLifeColorCustomization> MakeupLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessories")
	TArray<FName> AccessoryIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tattoos")
	TArray<FName> TattooIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voice")
	FName VoiceProfileId;
};

/** Data asset defining available customization options for a character archetype. */
UCLASS(BlueprintType)
class APARTMENTLIFECHARACTER_API UApartmentLifeCharacterCustomizationData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("CharacterCustomization")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	USkeletalMesh* BaseBodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	USkeletalMesh* BaseFaceMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options")
	TArray<FName> AvailableHairstyles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options")
	TArray<FName> AvailableVoices;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	FApartmentLifeCharacterAppearanceState DefaultAppearance;
};

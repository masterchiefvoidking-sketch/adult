// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCharacterCustomizationData.h"
#include "ApartmentLifeCharacterCreatorTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeFaceRegion : uint8
{
	FaceShape	UMETA(DisplayName = "Face Shape"),
	Eyes		UMETA(DisplayName = "Eyes"),
	Eyebrows	UMETA(DisplayName = "Eyebrows"),
	Nose		UMETA(DisplayName = "Nose"),
	Lips		UMETA(DisplayName = "Lips"),
	Jaw			UMETA(DisplayName = "Jaw"),
	Cheeks		UMETA(DisplayName = "Cheeks"),
	Ears		UMETA(DisplayName = "Ears")
};

UENUM(BlueprintType)
enum class EApartmentLifeWalkStyle : uint8
{
	Default		UMETA(DisplayName = "Default"),
	Confident	UMETA(DisplayName = "Confident"),
	Relaxed		UMETA(DisplayName = "Relaxed"),
	Energetic	UMETA(DisplayName = "Energetic"),
	Reserved	UMETA(DisplayName = "Reserved")
};

UENUM(BlueprintType)
enum class EApartmentLifeIdleStyle : uint8
{
	Neutral		UMETA(DisplayName = "Neutral"),
	Shy			UMETA(DisplayName = "Shy"),
	Bubbly		UMETA(DisplayName = "Bubbly"),
	Composed	UMETA(DisplayName = "Composed"),
	Athletic	UMETA(DisplayName = "Athletic")
};

UENUM(BlueprintType)
enum class EApartmentLifePostureStyle : uint8
{
	Neutral		UMETA(DisplayName = "Neutral"),
	Upright		UMETA(DisplayName = "Upright"),
	Relaxed		UMETA(DisplayName = "Relaxed"),
	Athletic	UMETA(DisplayName = "Athletic")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeFaceCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> FaceShapeMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> EyeMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> EyebrowMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> NoseMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> LipMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> JawMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> CheekMorphs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	TArray<FApartmentLifeMorphSlider> EarMorphs;

	APARTMENTLIFECHARACTER_API TArray<FApartmentLifeMorphSlider> GetAllFaceMorphs() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeBodyCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "140", ClampMax = "200"))
	float HeightCm = 165.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float ShoulderWidth = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float Waist = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float Hips = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float ArmLength = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float LegLength = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float TorsoLength = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body", meta = (ClampMin = "0", ClampMax = "1"))
	float MuscleTone = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	EApartmentLifePostureStyle Posture = EApartmentLifePostureStyle::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	EApartmentLifeWalkStyle WalkStyle = EApartmentLifeWalkStyle::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	EApartmentLifeIdleStyle IdleStyle = EApartmentLifeIdleStyle::Neutral;

	APARTMENTLIFECHARACTER_API TArray<FApartmentLifeMorphSlider> ToMorphSliders() const;
	APARTMENTLIFECHARACTER_API FVector GetScaleVector() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeMakeupCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Makeup")
	TArray<FApartmentLifeColorCustomization> FaceMakeup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Makeup")
	FApartmentLifeColorCustomization LipColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Makeup")
	FApartmentLifeColorCustomization NailColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Makeup", meta = (ClampMin = "0", ClampMax = "1"))
	float MakeupIntensity = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeCharacterCreatorState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeBodyCustomization Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeFaceCustomization Face;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeColorCustomization SkinTone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeColorCustomization EyeColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FName HairstyleId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeColorCustomization HairColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FApartmentLifeMakeupCustomization Makeup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	TArray<FName> GlassesIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	TArray<FName> JewelryIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	TArray<FName> AccessoryIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	TArray<FName> TattooIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FName VoicePresetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	FName PersonalityPresetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creator")
	TArray<FLinearColor> FavoriteColors;

	/** Convert to legacy appearance state for backward compatibility. */
	APARTMENTLIFECHARACTER_API FApartmentLifeCharacterAppearanceState ToLegacyAppearanceState() const;

	/** Populate from legacy appearance state. */
	APARTMENTLIFECHARACTER_API void FromLegacyAppearanceState(const FApartmentLifeCharacterAppearanceState& Legacy);
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECHARACTER_API FApartmentLifeBodyFitProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fit")
	FVector ScaleAdjustment = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fit")
	float SleeveLengthOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fit")
	float PantLengthOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fit")
	float CollisionRadius = 34.f;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeCameraTypes.generated.h"

/** Top-level camera modes for MP10 polish. */
UENUM(BlueprintType)
enum class EApartmentLifePrimaryCameraMode : uint8
{
	Apartment		UMETA(DisplayName = "Apartment"),
	CharacterFocus	UMETA(DisplayName = "Character Focus"),
	Wardrobe		UMETA(DisplayName = "Wardrobe"),
	BuildMode		UMETA(DisplayName = "Build Mode"),
	Activity		UMETA(DisplayName = "Activity"),
	Photo			UMETA(DisplayName = "Photo Mode")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECAMERA_API FApartmentLifePhotoModeSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float FieldOfView = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float Aperture = 2.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float FocusDistance = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float BlurAmount = 0.65f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	float TimeOfDayHour = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photo")
	bool bHideUI = true;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECAMERA_API FApartmentLifeCameraUserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float OrbitSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float PanSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bInvertYAxis = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	EApartmentLifePrimaryCameraMode LastPrimaryMode = EApartmentLifePrimaryCameraMode::Apartment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float PreferredWardrobeZoom = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bPreferBuildTopDown = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FApartmentLifePhotoModeSettings PhotoSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float PreferredCreatorZoom = 240.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	uint8 CreatorLightingMode = 3;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFECAMERA_API FApartmentLifeActivityCameraFrame
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLength = 380.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float Pitch = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector FocusOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bPrivacyFraming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bAllowManualOrbit = true;
};

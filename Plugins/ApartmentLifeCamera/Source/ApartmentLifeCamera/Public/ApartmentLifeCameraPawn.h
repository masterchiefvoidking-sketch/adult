// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ApartmentLifeCameraTypes.h"
#include "ApartmentLifeCameraPawn.generated.h"

class UApartmentLifeOrbitSpringArmComponent;
class UApartmentLifeCineCameraComponent;
class USceneComponent;

UENUM(BlueprintType)
enum class EApartmentLifeCameraMode : uint8
{
	Orbit		UMETA(DisplayName = "Orbit"),
	Free		UMETA(DisplayName = "Free Camera"),
	Photo		UMETA(DisplayName = "Photo Mode"),
	TopDown		UMETA(DisplayName = "Top Down"),
	RoomFocus	UMETA(DisplayName = "Room Focus"),
	CharacterFace		UMETA(DisplayName = "Character Face"),
	CharacterOutfit		UMETA(DisplayName = "Character Outfit"),
	CharacterFullBody	UMETA(DisplayName = "Character Full Body"),
	CharacterUpperBody	UMETA(DisplayName = "Character Upper Body"),
	CharacterHair		UMETA(DisplayName = "Character Hair"),
	CharacterShoes		UMETA(DisplayName = "Character Shoes"),
	PosePreview			UMETA(DisplayName = "Pose Preview"),
	AnimationPreview	UMETA(DisplayName = "Animation Preview"),
	FurnitureFocus		UMETA(DisplayName = "Furniture Focus"),
	CharacterCreator	UMETA(DisplayName = "Character Creator"),
	BuildMode			UMETA(DisplayName = "Build Mode"),
	Activity			UMETA(DisplayName = "Activity"),
	Wardrobe			UMETA(DisplayName = "Wardrobe")
};

UENUM(BlueprintType)
enum class EApartmentLifeCharacterFocusMode : uint8
{
	FullBody	UMETA(DisplayName = "Full Body"),
	UpperBody	UMETA(DisplayName = "Upper Body"),
	Face		UMETA(DisplayName = "Face"),
	Outfit		UMETA(DisplayName = "Outfit"),
	Hair		UMETA(DisplayName = "Hair"),
	Shoes		UMETA(DisplayName = "Shoes"),
	PosePreview	UMETA(DisplayName = "Pose Preview"),
	AnimationPreview UMETA(DisplayName = "Animation Preview"),
	Activity	UMETA(DisplayName = "Current Activity"),
	InteractionObject UMETA(DisplayName = "Interaction Object")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhotoModeUIVisibilityChanged, bool, bHideUI);

/**
 * Premium inspection camera for rooms, furniture, outfits, activities, and photo mode.
 */
UCLASS()
class APARTMENTLIFECAMERA_API AApartmentLifeCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AApartmentLifeCameraPawn();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetFocusTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ToggleFocusLock();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ToggleFreeCameraMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void TogglePhotoMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterPhotoMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ExitPhotoMode();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	EApartmentLifeCameraMode GetCameraMode() const { return CameraMode; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	EApartmentLifePrimaryCameraMode GetPrimaryCameraMode() const { return PrimaryCameraMode; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetPrimaryCameraMode(EApartmentLifePrimaryCameraMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ReturnToApartmentCamera();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ResetView();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetApartmentBounds(const FBox& Bounds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ApplyUserSettings(const FApartmentLifeCameraUserSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	FApartmentLifeCameraUserSettings BuildUserSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AddYawInput(float YawDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AddPitchInput(float PitchDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AddZoomInput(float ZoomDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AddPanInputX(float PanDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AddPanInputY(float PanDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusFurniture(AActor* Furniture, const FVector& FocusOffset = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterCharacterCreatorMode(AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ExitCharacterCreatorMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetCreatorLightingMode(uint8 LightingMode);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	uint8 GetCreatorLightingMode() const { return ActiveCreatorLightingMode; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterBuildModeCamera(AActor* ApartmentActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterWardrobeCamera(AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ExitWardrobeCamera();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterActivityCamera(AActor* Character, FName ActivityId, AActor* ContextActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ExitActivityCamera();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetBuildTopDownMode(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusRoom(const FBox& RoomBounds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusCharacter(AActor* Character, EApartmentLifeCharacterFocusMode FocusMode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusTargetFromDoubleClick(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetCharacterPreviewRotationEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void RotateFocusedCharacter(float YawDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AdjustPhotoFieldOfView(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void AdjustPhotoAperture(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void TakeHighResScreenshot();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	bool IsCharacterPreviewRotationEnabled() const { return bRotateCharacterInsteadOfCamera; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Camera")
	FOnPhotoModeUIVisibilityChanged OnPhotoModeUIVisibilityChanged;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void UpdateOrbitPivot();
	void ApplyFreeCameraMovement(float DeltaSeconds);
	void BindLegacyInput(UInputComponent* PlayerInputComponent);
	void ApplyCharacterFocusFraming(EApartmentLifeCharacterFocusMode FocusMode);
	FVector GetCharacterFocusPoint(AActor* Character, EApartmentLifeCharacterFocusMode FocusMode) const;
	EApartmentLifeCameraMode GetCameraModeForFocus(EApartmentLifeCharacterFocusMode FocusMode) const;
	void ClampPivotToBounds();
	void ApplyPhotoSettings(const FApartmentLifePhotoModeSettings& PhotoSettings);
	void PushModeSnapshot();
	void RestoreModeSnapshot();

	struct FCameraModeSnapshot
	{
		EApartmentLifeCameraMode CameraMode = EApartmentLifeCameraMode::Orbit;
		EApartmentLifePrimaryCameraMode PrimaryMode = EApartmentLifePrimaryCameraMode::Apartment;
		EApartmentLifeCharacterFocusMode CharacterFocus = EApartmentLifeCharacterFocusMode::FullBody;
		float Yaw = 0.f;
		float Pitch = -25.f;
		float ArmLength = 400.f;
		FVector PivotLocation = FVector::ZeroVector;
		bool bFocusLock = true;
		bool bRotateCharacter = false;
		TWeakObjectPtr<AActor> FocusTarget;
		FVector ActivityFocusOffset = FVector::ZeroVector;
		bool bPrivacyFraming = false;
		bool bAllowManualOrbit = true;
	};

	void OnOrbitPressed();
	void OnOrbitReleased();
	void OnRotatePressed();
	void OnRotateReleased();
	void OnPanPressed();
	void OnPanReleased();
	void OnFocusDoubleClick();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USceneComponent> PivotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UApartmentLifeOrbitSpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UApartmentLifeCineCameraComponent> CineCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float MinArmLength = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float MaxArmLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float ZoomSpeed = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float OrbitSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float MinPitch = -80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Orbit")
	float MaxPitch = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Free")
	float FreeMoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Pan")
	float PanSpeed = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Character")
	float CharacterFaceArmLength = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Character")
	float CharacterOutfitArmLength = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Character")
	float CharacterFullBodyArmLength = 380.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Furniture")
	float FurnitureFocusArmLength = 280.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Character")
	float PosePreviewArmLength = 300.f;

	UPROPERTY()
	TObjectPtr<AActor> FocusTarget;

	UPROPERTY()
	TObjectPtr<AActor> ActivityContextActor;

	FVector ActivityFocusOffset = FVector::ZeroVector;
	bool bActivityPrivacyFraming = false;
	bool bActivityAllowManualOrbit = true;

	EApartmentLifePrimaryCameraMode PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Apartment;
	EApartmentLifeCharacterFocusMode ActiveCharacterFocus = EApartmentLifeCharacterFocusMode::FullBody;
	EApartmentLifeCameraMode CameraMode = EApartmentLifeCameraMode::Orbit;
	FCameraModeSnapshot ModeSnapshot;

	FApartmentLifeCameraUserSettings ActiveSettings;
	FBox ApartmentBounds;
	bool bHasApartmentBounds = false;

	bool bRotateCharacterInsteadOfCamera = false;
	bool bFocusLockEnabled = true;
	bool bOrbitInputActive = false;
	bool bRotateInputActive = false;
	bool bPanInputActive = false;
	bool bInvertYAxis = false;
	bool bReduceMotion = false;

	float CurrentYaw = 0.f;
	float CurrentPitch = -25.f;
	float CurrentArmLength = 400.f;
	float DefaultYaw = 0.f;
	float DefaultPitch = -25.f;
	float DefaultArmLength = 400.f;
	FVector DefaultPivotLocation = FVector::ZeroVector;
	float LastTickDeltaSeconds = 0.016f;
	uint8 ActiveCreatorLightingMode = 3;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
	PosePreview			UMETA(DisplayName = "Pose Preview"),
	AnimationPreview	UMETA(DisplayName = "Animation Preview"),
	FurnitureFocus		UMETA(DisplayName = "Furniture Focus"),
	CharacterCreator	UMETA(DisplayName = "Character Creator"),
	BuildMode			UMETA(DisplayName = "Build Mode")
};

UENUM(BlueprintType)
enum class EApartmentLifeCharacterFocusMode : uint8
{
	FullBody	UMETA(DisplayName = "Full Body"),
	Face		UMETA(DisplayName = "Face"),
	Outfit		UMETA(DisplayName = "Outfit"),
	PosePreview	UMETA(DisplayName = "Pose Preview"),
	AnimationPreview UMETA(DisplayName = "Animation Preview")
};

/**
 * Premium inspection camera for rooms, furniture, outfits, and characters.
 * Supports orbit, smooth zoom/rotation, collision, focus lock, pan, and controller input.
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

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	EApartmentLifeCameraMode GetCameraMode() const { return CameraMode; }

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
	void FocusFurniture(AActor* Furniture);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterCharacterCreatorMode(AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void EnterBuildModeCamera(AActor* ApartmentActor = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetBuildTopDownMode(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusRoom(const FBox& RoomBounds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void FocusCharacter(AActor* Character, EApartmentLifeCharacterFocusMode FocusMode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetCharacterPreviewRotationEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void RotateFocusedCharacter(float YawDelta);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	bool IsCharacterPreviewRotationEnabled() const { return bRotateCharacterInsteadOfCamera; }

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

	void OnOrbitPressed();
	void OnOrbitReleased();
	void OnPanPressed();
	void OnPanReleased();

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

	EApartmentLifeCharacterFocusMode ActiveCharacterFocus = EApartmentLifeCharacterFocusMode::FullBody;
	bool bRotateCharacterInsteadOfCamera = false;

	EApartmentLifeCameraMode CameraMode = EApartmentLifeCameraMode::Orbit;
	bool bFocusLockEnabled = true;
	bool bOrbitInputActive = false;
	bool bPanInputActive = false;

	float CurrentYaw = 0.f;
	float CurrentPitch = -25.f;
	float CurrentArmLength = 400.f;
	FVector FreeCameraVelocity = FVector::ZeroVector;
};

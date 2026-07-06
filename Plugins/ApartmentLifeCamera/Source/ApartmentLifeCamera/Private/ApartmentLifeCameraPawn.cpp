// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeOrbitSpringArmComponent.h"
#include "ApartmentLifeCineCameraComponent.h"
#include "ApartmentLifeActivityCameraLibrary.h"
#include "ApartmentLifeCameraSettingsSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"
#include "Engine/GameInstance.h"

AApartmentLifeCameraPawn::AApartmentLifeCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	PivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	SetRootComponent(PivotComponent);

	SpringArm = CreateDefaultSubobject<UApartmentLifeOrbitSpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PivotComponent);
	SpringArm->TargetArmLength = CurrentArmLength;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	CineCamera = CreateDefaultSubobject<UApartmentLifeCineCameraComponent>(TEXT("CineCamera"));
	CineCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AApartmentLifeCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	DefaultYaw = CurrentYaw;
	DefaultPitch = CurrentPitch;
	DefaultArmLength = CurrentArmLength;
	DefaultPivotLocation = PivotComponent->GetComponentLocation();
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeCameraSettingsSubsystem* SettingsSubsystem = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
		{
			ApplyUserSettings(SettingsSubsystem->GetSettings());
		}
	}
}

void AApartmentLifeCameraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const bool bOrbitStyleMode = CameraMode == EApartmentLifeCameraMode::Orbit
		|| CameraMode == EApartmentLifeCameraMode::Photo
		|| CameraMode == EApartmentLifeCameraMode::RoomFocus
		|| CameraMode == EApartmentLifeCameraMode::CharacterFace
		|| CameraMode == EApartmentLifeCameraMode::CharacterOutfit
		|| CameraMode == EApartmentLifeCameraMode::CharacterFullBody
		|| CameraMode == EApartmentLifeCameraMode::CharacterUpperBody
		|| CameraMode == EApartmentLifeCameraMode::CharacterHair
		|| CameraMode == EApartmentLifeCameraMode::CharacterShoes
		|| CameraMode == EApartmentLifeCameraMode::PosePreview
		|| CameraMode == EApartmentLifeCameraMode::AnimationPreview
		|| CameraMode == EApartmentLifeCameraMode::FurnitureFocus
		|| CameraMode == EApartmentLifeCameraMode::CharacterCreator
		|| CameraMode == EApartmentLifeCameraMode::Activity
		|| CameraMode == EApartmentLifeCameraMode::Wardrobe;

	if (bOrbitStyleMode)
	{
		UpdateOrbitPivot();
		ClampPivotToBounds();
		const FRotator OrbitRotation(CurrentPitch, CurrentYaw, 0.f);
		SpringArm->SetWorldRotation(OrbitRotation);
	}
	else if (CameraMode == EApartmentLifeCameraMode::TopDown || CameraMode == EApartmentLifeCameraMode::BuildMode)
	{
		CurrentPitch = -89.f;
		SpringArm->SetWorldRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
		ClampPivotToBounds();
	}
	else if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		ApplyFreeCameraMovement(DeltaSeconds);
	}
}

void AApartmentLifeCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindLegacyInput(PlayerInputComponent);
}

void AApartmentLifeCameraPawn::BindLegacyInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("CameraOrbit"), IE_Pressed, this, &AApartmentLifeCameraPawn::OnOrbitPressed);
	PlayerInputComponent->BindAction(TEXT("CameraOrbit"), IE_Released, this, &AApartmentLifeCameraPawn::OnOrbitReleased);
	PlayerInputComponent->BindAction(TEXT("CameraRotate"), IE_Pressed, this, &AApartmentLifeCameraPawn::OnRotatePressed);
	PlayerInputComponent->BindAction(TEXT("CameraRotate"), IE_Released, this, &AApartmentLifeCameraPawn::OnRotateReleased);
	PlayerInputComponent->BindAction(TEXT("CameraPan"), IE_Pressed, this, &AApartmentLifeCameraPawn::OnPanPressed);
	PlayerInputComponent->BindAction(TEXT("CameraPan"), IE_Released, this, &AApartmentLifeCameraPawn::OnPanReleased);
	PlayerInputComponent->BindAction(TEXT("CameraFocus"), IE_Pressed, this, &AApartmentLifeCameraPawn::OnFocusDoubleClick);
	PlayerInputComponent->BindAction(TEXT("ToggleFreeCamera"), IE_Pressed, this, &AApartmentLifeCameraPawn::ToggleFreeCameraMode);
	PlayerInputComponent->BindAction(TEXT("TogglePhotoMode"), IE_Pressed, this, &AApartmentLifeCameraPawn::TogglePhotoMode);
	PlayerInputComponent->BindAction(TEXT("ToggleFocusLock"), IE_Pressed, this, &AApartmentLifeCameraPawn::ToggleFocusLock);
	PlayerInputComponent->BindAction(TEXT("ResetCamera"), IE_Pressed, this, &AApartmentLifeCameraPawn::ResetView);

	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &AApartmentLifeCameraPawn::AddYawInput);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &AApartmentLifeCameraPawn::AddPitchInput);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &AApartmentLifeCameraPawn::AddZoomInput);
	PlayerInputComponent->BindAxis(TEXT("CameraPanX"), this, &AApartmentLifeCameraPawn::AddPanInputX);
	PlayerInputComponent->BindAxis(TEXT("CameraPanY"), this, &AApartmentLifeCameraPawn::AddPanInputY);
}

void AApartmentLifeCameraPawn::SetFocusTarget(AActor* NewTarget)
{
	FocusTarget = NewTarget;
	UpdateOrbitPivot();
}

void AApartmentLifeCameraPawn::ToggleFocusLock()
{
	bFocusLockEnabled = !bFocusLockEnabled;
	UpdateOrbitPivot();
}

void AApartmentLifeCameraPawn::SetPrimaryCameraMode(EApartmentLifePrimaryCameraMode Mode)
{
	PrimaryCameraMode = Mode;
	ActiveSettings.LastPrimaryMode = Mode;
}

void AApartmentLifeCameraPawn::ReturnToApartmentCamera()
{
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Apartment;
	CameraMode = EApartmentLifeCameraMode::Orbit;
	bRotateCharacterInsteadOfCamera = false;
	CineCamera->SetPhotoModeEnabled(false);
	CurrentPitch = DefaultPitch;
	CurrentArmLength = DefaultArmLength;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	OnPhotoModeUIVisibilityChanged.Broadcast(false);
}

void AApartmentLifeCameraPawn::ResetView()
{
	CurrentYaw = DefaultYaw;
	CurrentPitch = DefaultPitch;
	CurrentArmLength = DefaultArmLength;
	PivotComponent->SetWorldLocation(DefaultPivotLocation);
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
}

void AApartmentLifeCameraPawn::SetApartmentBounds(const FBox& Bounds)
{
	ApartmentBounds = Bounds;
	bHasApartmentBounds = Bounds.IsValid != 0;
}

void AApartmentLifeCameraPawn::ApplyUserSettings(const FApartmentLifeCameraUserSettings& Settings)
{
	ActiveSettings = Settings;
	OrbitSensitivity = Settings.OrbitSensitivity;
	ZoomSpeed = 45.f * Settings.ZoomSensitivity;
	PanSpeed = 4.f * Settings.PanSensitivity;
	bInvertYAxis = Settings.bInvertYAxis;
	CharacterOutfitArmLength = Settings.PreferredWardrobeZoom;
	ApplyPhotoSettings(Settings.PhotoSettings);
}

FApartmentLifeCameraUserSettings AApartmentLifeCameraPawn::BuildUserSettings() const
{
	FApartmentLifeCameraUserSettings Settings = ActiveSettings;
	Settings.OrbitSensitivity = OrbitSensitivity;
	Settings.ZoomSensitivity = ZoomSpeed / 45.f;
	Settings.PanSensitivity = PanSpeed / 4.f;
	Settings.bInvertYAxis = bInvertYAxis;
	Settings.LastPrimaryMode = PrimaryCameraMode;
	Settings.PreferredWardrobeZoom = CharacterOutfitArmLength;
	return Settings;
}

void AApartmentLifeCameraPawn::ToggleFreeCameraMode()
{
	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		ReturnToApartmentCamera();
	}
	else
	{
		PushModeSnapshot();
		CameraMode = EApartmentLifeCameraMode::Free;
		PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Apartment;
		CineCamera->SetPhotoModeEnabled(false);
		SpringArm->SetRelativeLocation(FVector::ZeroVector);
	}
}

void AApartmentLifeCameraPawn::TogglePhotoMode()
{
	if (PrimaryCameraMode == EApartmentLifePrimaryCameraMode::Photo)
	{
		ExitPhotoMode();
	}
	else
	{
		EnterPhotoMode();
	}
}

void AApartmentLifeCameraPawn::EnterPhotoMode()
{
	PushModeSnapshot();
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Photo;
	CameraMode = EApartmentLifeCameraMode::Photo;
	CineCamera->SetPhotoModeEnabled(true);
	ApplyPhotoSettings(ActiveSettings.PhotoSettings);
	OnPhotoModeUIVisibilityChanged.Broadcast(ActiveSettings.PhotoSettings.bHideUI);
}

void AApartmentLifeCameraPawn::ExitPhotoMode()
{
	RestoreModeSnapshot();
	CineCamera->SetPhotoModeEnabled(false);
	OnPhotoModeUIVisibilityChanged.Broadcast(false);
}

void AApartmentLifeCameraPawn::AddYawInput(float YawDelta)
{
	if (FMath::IsNearlyZero(YawDelta))
	{
		return;
	}

	const float ScaledDelta = YawDelta * OrbitSensitivity;

	if (bRotateCharacterInsteadOfCamera && FocusTarget)
	{
		RotateFocusedCharacter(ScaledDelta);
		return;
	}

	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		CurrentYaw += ScaledDelta;
		SetActorRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
		return;
	}

	if (!bOrbitInputActive && !bRotateInputActive)
	{
		return;
	}

	if (PrimaryCameraMode == EApartmentLifePrimaryCameraMode::Activity && !bActivityAllowManualOrbit)
	{
		return;
	}

	CurrentYaw += ScaledDelta;
}

void AApartmentLifeCameraPawn::AddPitchInput(float PitchDelta)
{
	if (FMath::IsNearlyZero(PitchDelta))
	{
		return;
	}

	const float ScaledDelta = (bInvertYAxis ? -PitchDelta : PitchDelta) * OrbitSensitivity;

	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		CurrentPitch = FMath::Clamp(CurrentPitch + ScaledDelta, MinPitch, MaxPitch);
		SetActorRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
		return;
	}

	if (!bOrbitInputActive && !bRotateInputActive)
	{
		return;
	}

	if (PrimaryCameraMode == EApartmentLifePrimaryCameraMode::Activity && !bActivityAllowManualOrbit)
	{
		return;
	}

	CurrentPitch = FMath::Clamp(CurrentPitch + ScaledDelta, MinPitch, MaxPitch);
}

void AApartmentLifeCameraPawn::AddZoomInput(float ZoomDelta)
{
	if (FMath::IsNearlyZero(ZoomDelta))
	{
		return;
	}

	CurrentArmLength = FMath::Clamp(CurrentArmLength - ZoomDelta * ZoomSpeed, MinArmLength, MaxArmLength);
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);

	if (PrimaryCameraMode == EApartmentLifePrimaryCameraMode::Photo)
	{
		ActiveSettings.PhotoSettings.FocusDistance = CurrentArmLength;
		CineCamera->SetDepthOfFieldSettings(CurrentArmLength, ActiveSettings.PhotoSettings.Aperture, ActiveSettings.PhotoSettings.BlurAmount);
	}
}

void AApartmentLifeCameraPawn::AddPanInputX(float PanDelta)
{
	if (!bPanInputActive || CameraMode == EApartmentLifeCameraMode::Free || FMath::IsNearlyZero(PanDelta))
	{
		return;
	}

	PivotComponent->AddWorldOffset(SpringArm->GetRightVector() * PanDelta * PanSpeed);
	ClampPivotToBounds();
}

void AApartmentLifeCameraPawn::AddPanInputY(float PanDelta)
{
	if (!bPanInputActive || CameraMode == EApartmentLifeCameraMode::Free || FMath::IsNearlyZero(PanDelta))
	{
		return;
	}

	PivotComponent->AddWorldOffset(FVector::UpVector * PanDelta * PanSpeed);
}

void AApartmentLifeCameraPawn::UpdateOrbitPivot()
{
	if (!bFocusLockEnabled || !FocusTarget)
	{
		return;
	}

	FVector TargetLocation = FocusTarget->GetActorLocation();

	const bool bCharacterMode = CameraMode == EApartmentLifeCameraMode::CharacterFace
		|| CameraMode == EApartmentLifeCameraMode::CharacterOutfit
		|| CameraMode == EApartmentLifeCameraMode::CharacterFullBody
		|| CameraMode == EApartmentLifeCameraMode::CharacterUpperBody
		|| CameraMode == EApartmentLifeCameraMode::CharacterHair
		|| CameraMode == EApartmentLifeCameraMode::CharacterShoes
		|| CameraMode == EApartmentLifeCameraMode::PosePreview
		|| CameraMode == EApartmentLifeCameraMode::AnimationPreview
		|| CameraMode == EApartmentLifeCameraMode::CharacterCreator
		|| CameraMode == EApartmentLifeCameraMode::Wardrobe
		|| CameraMode == EApartmentLifeCameraMode::Activity;

	if (bCharacterMode)
	{
		TargetLocation = GetCharacterFocusPoint(FocusTarget, ActiveCharacterFocus) + ActivityFocusOffset;
	}
	else if (CameraMode == EApartmentLifeCameraMode::FurnitureFocus)
	{
		TargetLocation += ActivityFocusOffset;
	}

	if (bActivityPrivacyFraming)
	{
		TargetLocation += FVector(0.f, 80.f, 0.f);
	}

	PivotComponent->SetWorldLocation(FMath::VInterpTo(PivotComponent->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 8.f));
}

void AApartmentLifeCameraPawn::ClampPivotToBounds()
{
	if (!bHasApartmentBounds)
	{
		return;
	}

	FVector Location = PivotComponent->GetComponentLocation();
	Location.X = FMath::Clamp(Location.X, ApartmentBounds.Min.X, ApartmentBounds.Max.X);
	Location.Y = FMath::Clamp(Location.Y, ApartmentBounds.Min.Y, ApartmentBounds.Max.Y);
	Location.Z = FMath::Clamp(Location.Z, ApartmentBounds.Min.Z, ApartmentBounds.Max.Z);
	PivotComponent->SetWorldLocation(Location);
}

void AApartmentLifeCameraPawn::ApplyFreeCameraMovement(float DeltaSeconds)
{
	const FRotator Rotation = GetActorRotation();
	const FVector Forward = Rotation.Vector();
	const FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

	FVector MoveInput = FVector::ZeroVector;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		float ForwardAxis = 0.f;
		float RightAxis = 0.f;
		PC->GetInputAxisValue(TEXT("CameraPanY"), ForwardAxis);
		PC->GetInputAxisValue(TEXT("CameraPanX"), RightAxis);
		MoveInput = Forward * ForwardAxis + Right * RightAxis;
	}

	if (!MoveInput.IsNearlyZero())
	{
		AddMovementInput(MoveInput.GetSafeNormal(), FreeMoveSpeed * DeltaSeconds);
	}
}

void AApartmentLifeCameraPawn::SetBuildTopDownMode(bool bEnabled)
{
	if (bEnabled)
	{
		CameraMode = EApartmentLifeCameraMode::TopDown;
		PrimaryCameraMode = EApartmentLifePrimaryCameraMode::BuildMode;
		CurrentPitch = -89.f;
		CurrentArmLength = FMath::Clamp(CurrentArmLength, 400.f, MaxArmLength);
		SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	}
	else if (CameraMode == EApartmentLifeCameraMode::TopDown || CameraMode == EApartmentLifeCameraMode::BuildMode)
	{
		ReturnToApartmentCamera();
	}
}

void AApartmentLifeCameraPawn::FocusFurniture(AActor* Furniture, const FVector& FocusOffset)
{
	if (!Furniture)
	{
		return;
	}

	FocusTarget = Furniture;
	ActivityFocusOffset = FocusOffset;
	CameraMode = EApartmentLifeCameraMode::FurnitureFocus;
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Apartment;
	bFocusLockEnabled = true;
	CurrentArmLength = FurnitureFocusArmLength;
	CurrentPitch = -20.f;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	UpdateOrbitPivot();
}

void AApartmentLifeCameraPawn::EnterCharacterCreatorMode(AActor* Character)
{
	if (!Character)
	{
		return;
	}

	FocusCharacter(Character, EApartmentLifeCharacterFocusMode::FullBody);
	CameraMode = EApartmentLifeCameraMode::CharacterCreator;
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::CharacterFocus;
	bRotateCharacterInsteadOfCamera = true;
	CurrentPitch = -10.f;
	CurrentArmLength = ActiveSettings.PreferredCreatorZoom;
	ActiveCreatorLightingMode = ActiveSettings.CreatorLightingMode;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
}

void AApartmentLifeCameraPawn::ExitCharacterCreatorMode()
{
	bRotateCharacterInsteadOfCamera = false;
	ReturnToApartmentCamera();
}

void AApartmentLifeCameraPawn::SetCreatorLightingMode(uint8 LightingMode)
{
	ActiveCreatorLightingMode = LightingMode;
	ActiveSettings.CreatorLightingMode = LightingMode;
}

void AApartmentLifeCameraPawn::EnterBuildModeCamera(AActor* ApartmentActor)
{
	CameraMode = EApartmentLifeCameraMode::BuildMode;
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::BuildMode;
	CurrentPitch = -89.f;
	CurrentArmLength = FMath::Clamp(650.f, MinArmLength, MaxArmLength);
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);

	if (ApartmentActor)
	{
		FocusTarget = ApartmentActor;
		bFocusLockEnabled = true;
		PivotComponent->SetWorldLocation(ApartmentActor->GetActorLocation());
		SetApartmentBounds(FBox(ApartmentActor->GetActorLocation() - FVector(400.f), ApartmentActor->GetActorLocation() + FVector(400.f)));
	}
	else
	{
		bFocusLockEnabled = false;
	}
}

void AApartmentLifeCameraPawn::EnterWardrobeCamera(AActor* Character)
{
	if (!Character)
	{
		return;
	}

	FocusCharacter(Character, EApartmentLifeCharacterFocusMode::Outfit);
	CameraMode = EApartmentLifeCameraMode::Wardrobe;
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Wardrobe;
	bRotateCharacterInsteadOfCamera = true;
	CurrentArmLength = ActiveSettings.PreferredWardrobeZoom;
	CurrentPitch = -8.f;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
}

void AApartmentLifeCameraPawn::ExitWardrobeCamera()
{
	bRotateCharacterInsteadOfCamera = false;
	ReturnToApartmentCamera();
}

void AApartmentLifeCameraPawn::EnterActivityCamera(AActor* Character, FName ActivityId, AActor* ContextActor)
{
	if (!Character)
	{
		return;
	}

	const FApartmentLifeActivityCameraFrame Frame = UApartmentLifeActivityCameraLibrary::GetFrameForActivity(ActivityId);
	FocusTarget = Character;
	ActivityContextActor = ContextActor;
	ActivityFocusOffset = Frame.FocusOffset;
	bActivityPrivacyFraming = Frame.bPrivacyFraming;
	bActivityAllowManualOrbit = Frame.bAllowManualOrbit;
	ActiveCharacterFocus = EApartmentLifeCharacterFocusMode::FullBody;

	CameraMode = EApartmentLifeCameraMode::Activity;
	PrimaryCameraMode = UApartmentLifeActivityCameraLibrary::GetPrimaryModeForActivity(ActivityId);
	bFocusLockEnabled = true;
	bRotateCharacterInsteadOfCamera = false;
	CurrentArmLength = Frame.ArmLength;
	CurrentPitch = Frame.Pitch;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	UpdateOrbitPivot();
}

void AApartmentLifeCameraPawn::ExitActivityCamera()
{
	ActivityContextActor = nullptr;
	ActivityFocusOffset = FVector::ZeroVector;
	bActivityPrivacyFraming = false;
	ReturnToApartmentCamera();
}

void AApartmentLifeCameraPawn::FocusRoom(const FBox& RoomBounds)
{
	CameraMode = EApartmentLifeCameraMode::RoomFocus;
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::Apartment;
	const FVector Center = RoomBounds.GetCenter();
	PivotComponent->SetWorldLocation(Center);
	CurrentArmLength = RoomBounds.GetExtent().GetMax() * 2.5f;
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	bFocusLockEnabled = false;
}

void AApartmentLifeCameraPawn::FocusCharacter(AActor* Character, EApartmentLifeCharacterFocusMode FocusMode)
{
	if (!Character)
	{
		return;
	}

	FocusTarget = Character;
	ActiveCharacterFocus = FocusMode;
	CameraMode = GetCameraModeForFocus(FocusMode);
	PrimaryCameraMode = EApartmentLifePrimaryCameraMode::CharacterFocus;
	bFocusLockEnabled = true;
	ApplyCharacterFocusFraming(FocusMode);
}

void AApartmentLifeCameraPawn::FocusTargetFromDoubleClick(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	if (Target->IsA<APawn>())
	{
		FocusCharacter(Target, EApartmentLifeCharacterFocusMode::FullBody);
	}
	else
	{
		FocusFurniture(Target);
	}
}

void AApartmentLifeCameraPawn::SetCharacterPreviewRotationEnabled(bool bEnabled)
{
	bRotateCharacterInsteadOfCamera = bEnabled;
}

void AApartmentLifeCameraPawn::RotateFocusedCharacter(float YawDelta)
{
	if (!FocusTarget)
	{
		return;
	}

	FocusTarget->SetActorRotation(FocusTarget->GetActorRotation() + FRotator(0.f, YawDelta, 0.f));
}

void AApartmentLifeCameraPawn::AdjustPhotoFieldOfView(float Delta)
{
	ActiveSettings.PhotoSettings.FieldOfView = FMath::Clamp(ActiveSettings.PhotoSettings.FieldOfView + Delta, 20.f, 90.f);
	CineCamera->SetFieldOfViewSmooth(ActiveSettings.PhotoSettings.FieldOfView);
}

void AApartmentLifeCameraPawn::AdjustPhotoAperture(float Delta)
{
	ActiveSettings.PhotoSettings.Aperture = FMath::Clamp(ActiveSettings.PhotoSettings.Aperture + Delta, 0.8f, 16.f);
	CineCamera->SetDepthOfFieldSettings(ActiveSettings.PhotoSettings.FocusDistance, ActiveSettings.PhotoSettings.Aperture, ActiveSettings.PhotoSettings.BlurAmount);
}

void AApartmentLifeCameraPawn::TakeHighResScreenshot()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->ConsoleCommand(TEXT("HighResShot 1920x1080"));
	}
}

void AApartmentLifeCameraPawn::ApplyCharacterFocusFraming(EApartmentLifeCharacterFocusMode FocusMode)
{
	if (!FocusTarget)
	{
		return;
	}

	PivotComponent->SetWorldLocation(GetCharacterFocusPoint(FocusTarget, FocusMode));

	switch (FocusMode)
	{
	case EApartmentLifeCharacterFocusMode::Face:
		CurrentArmLength = CharacterFaceArmLength;
		CurrentPitch = -5.f;
		break;
	case EApartmentLifeCharacterFocusMode::Outfit:
		CurrentArmLength = CharacterOutfitArmLength;
		CurrentPitch = -10.f;
		break;
	case EApartmentLifeCharacterFocusMode::UpperBody:
		CurrentArmLength = CharacterOutfitArmLength * 0.9f;
		CurrentPitch = -6.f;
		break;
	case EApartmentLifeCharacterFocusMode::Hair:
		CurrentArmLength = CharacterFaceArmLength * 0.85f;
		CurrentPitch = -2.f;
		break;
	case EApartmentLifeCharacterFocusMode::Shoes:
		CurrentArmLength = CharacterFullBodyArmLength * 0.7f;
		CurrentPitch = -35.f;
		break;
	case EApartmentLifeCharacterFocusMode::PosePreview:
		CurrentArmLength = PosePreviewArmLength;
		CurrentPitch = -15.f;
		break;
	case EApartmentLifeCharacterFocusMode::AnimationPreview:
		CurrentArmLength = CharacterFullBodyArmLength;
		CurrentPitch = -20.f;
		break;
	default:
		CurrentArmLength = CharacterFullBodyArmLength;
		CurrentPitch = -20.f;
		break;
	}

	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
}

FVector AApartmentLifeCameraPawn::GetCharacterFocusPoint(AActor* Character, EApartmentLifeCharacterFocusMode FocusMode) const
{
	if (!Character)
	{
		return FVector::ZeroVector;
	}

	FVector Origin;
	FVector Extent;
	Character->GetActorBounds(true, Origin, Extent);

	switch (FocusMode)
	{
	case EApartmentLifeCharacterFocusMode::Face:
	case EApartmentLifeCharacterFocusMode::Hair:
		return Origin + FVector(0.f, 0.f, Extent.Z * 0.82f);
	case EApartmentLifeCharacterFocusMode::Outfit:
	case EApartmentLifeCharacterFocusMode::UpperBody:
		return Origin + FVector(0.f, 0.f, Extent.Z * 0.45f);
	case EApartmentLifeCharacterFocusMode::Shoes:
		return Origin + FVector(0.f, 0.f, Extent.Z * 0.05f);
	default:
		return Origin;
	}
}

EApartmentLifeCameraMode AApartmentLifeCameraPawn::GetCameraModeForFocus(EApartmentLifeCharacterFocusMode FocusMode) const
{
	switch (FocusMode)
	{
	case EApartmentLifeCharacterFocusMode::Face: return EApartmentLifeCameraMode::CharacterFace;
	case EApartmentLifeCharacterFocusMode::Outfit: return EApartmentLifeCameraMode::CharacterOutfit;
	case EApartmentLifeCharacterFocusMode::UpperBody: return EApartmentLifeCameraMode::CharacterUpperBody;
	case EApartmentLifeCharacterFocusMode::Hair: return EApartmentLifeCameraMode::CharacterHair;
	case EApartmentLifeCharacterFocusMode::Shoes: return EApartmentLifeCameraMode::CharacterShoes;
	case EApartmentLifeCharacterFocusMode::PosePreview: return EApartmentLifeCameraMode::PosePreview;
	case EApartmentLifeCharacterFocusMode::AnimationPreview: return EApartmentLifeCameraMode::AnimationPreview;
	default: return EApartmentLifeCameraMode::CharacterFullBody;
	}
}

void AApartmentLifeCameraPawn::ApplyPhotoSettings(const FApartmentLifePhotoModeSettings& PhotoSettings)
{
	CineCamera->SetFieldOfViewSmooth(PhotoSettings.FieldOfView);
	CineCamera->SetDepthOfFieldSettings(PhotoSettings.FocusDistance, PhotoSettings.Aperture, PhotoSettings.BlurAmount);
}

void AApartmentLifeCameraPawn::PushModeSnapshot()
{
	PreviousCameraMode = CameraMode;
	PreviousPrimaryMode = PrimaryCameraMode;
}

void AApartmentLifeCameraPawn::RestoreModeSnapshot()
{
	CameraMode = PreviousCameraMode;
	PrimaryCameraMode = PreviousPrimaryMode;
}

void AApartmentLifeCameraPawn::OnOrbitPressed() { bOrbitInputActive = true; }
void AApartmentLifeCameraPawn::OnOrbitReleased() { bOrbitInputActive = false; }
void AApartmentLifeCameraPawn::OnRotatePressed() { bRotateInputActive = true; }
void AApartmentLifeCameraPawn::OnRotateReleased() { bRotateInputActive = false; }
void AApartmentLifeCameraPawn::OnPanPressed() { bPanInputActive = true; }
void AApartmentLifeCameraPawn::OnPanReleased() { bPanInputActive = false; }

void AApartmentLifeCameraPawn::OnFocusDoubleClick()
{
	if (FocusTarget)
	{
		FocusTargetFromDoubleClick(FocusTarget);
	}
}

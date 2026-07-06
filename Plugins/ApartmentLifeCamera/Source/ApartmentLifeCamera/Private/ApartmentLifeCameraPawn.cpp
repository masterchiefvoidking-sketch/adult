// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeOrbitSpringArmComponent.h"
#include "ApartmentLifeCineCameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"

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
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
}

void AApartmentLifeCameraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CameraMode == EApartmentLifeCameraMode::Orbit || CameraMode == EApartmentLifeCameraMode::Photo || CameraMode == EApartmentLifeCameraMode::RoomFocus
		|| CameraMode == EApartmentLifeCameraMode::CharacterFace || CameraMode == EApartmentLifeCameraMode::CharacterOutfit
		|| CameraMode == EApartmentLifeCameraMode::CharacterFullBody || CameraMode == EApartmentLifeCameraMode::PosePreview
		|| CameraMode == EApartmentLifeCameraMode::AnimationPreview)
	{
		UpdateOrbitPivot();
		const FRotator OrbitRotation(CurrentPitch, CurrentYaw, 0.f);
		SpringArm->SetWorldRotation(OrbitRotation);
	}
	else if (CameraMode == EApartmentLifeCameraMode::TopDown)
	{
		CurrentPitch = -89.f;
		SpringArm->SetWorldRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
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
	PlayerInputComponent->BindAction(TEXT("CameraPan"), IE_Pressed, this, &AApartmentLifeCameraPawn::OnPanPressed);
	PlayerInputComponent->BindAction(TEXT("CameraPan"), IE_Released, this, &AApartmentLifeCameraPawn::OnPanReleased);
	PlayerInputComponent->BindAction(TEXT("ToggleFreeCamera"), IE_Pressed, this, &AApartmentLifeCameraPawn::ToggleFreeCameraMode);
	PlayerInputComponent->BindAction(TEXT("TogglePhotoMode"), IE_Pressed, this, &AApartmentLifeCameraPawn::TogglePhotoMode);
	PlayerInputComponent->BindAction(TEXT("ToggleFocusLock"), IE_Pressed, this, &AApartmentLifeCameraPawn::ToggleFocusLock);

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

void AApartmentLifeCameraPawn::ToggleFreeCameraMode()
{
	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		CameraMode = EApartmentLifeCameraMode::Orbit;
		CineCamera->SetPhotoModeEnabled(false);
		SpringArm->SetRelativeLocation(FVector::ZeroVector);
	}
	else
	{
		CameraMode = EApartmentLifeCameraMode::Free;
	}
}

void AApartmentLifeCameraPawn::TogglePhotoMode()
{
	if (CameraMode == EApartmentLifeCameraMode::Photo)
	{
		CameraMode = EApartmentLifeCameraMode::Orbit;
		CineCamera->SetPhotoModeEnabled(false);
	}
	else
	{
		CameraMode = EApartmentLifeCameraMode::Photo;
		CineCamera->SetPhotoModeEnabled(true);
		CineCamera->SetDepthOfFieldSettings(CurrentArmLength, 2.8f, 0.65f);
	}
}

void AApartmentLifeCameraPawn::AddYawInput(float YawDelta)
{
	if (FMath::IsNearlyZero(YawDelta))
	{
		return;
	}

	if (bRotateCharacterInsteadOfCamera && FocusTarget)
	{
		RotateFocusedCharacter(YawDelta * OrbitSensitivity);
		return;
	}

	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		CurrentYaw += YawDelta * OrbitSensitivity;
		SetActorRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
		return;
	}

	if (!bOrbitInputActive)
	{
		return;
	}

	CurrentYaw += YawDelta * OrbitSensitivity;
}

void AApartmentLifeCameraPawn::AddPitchInput(float PitchDelta)
{
	if (FMath::IsNearlyZero(PitchDelta))
	{
		return;
	}

	CurrentPitch = FMath::Clamp(CurrentPitch + PitchDelta * OrbitSensitivity, MinPitch, MaxPitch);

	if (CameraMode == EApartmentLifeCameraMode::Free)
	{
		SetActorRotation(FRotator(CurrentPitch, CurrentYaw, 0.f));
	}
}

void AApartmentLifeCameraPawn::AddZoomInput(float ZoomDelta)
{
	if (FMath::IsNearlyZero(ZoomDelta))
	{
		return;
	}

	CurrentArmLength = FMath::Clamp(CurrentArmLength - ZoomDelta * ZoomSpeed, MinArmLength, MaxArmLength);
	SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);

	if (CameraMode == EApartmentLifeCameraMode::Photo)
	{
		CineCamera->SetDepthOfFieldSettings(CurrentArmLength, 2.8f, 0.65f);
	}
}

void AApartmentLifeCameraPawn::AddPanInputX(float PanDelta)
{
	if (!bPanInputActive || CameraMode == EApartmentLifeCameraMode::Free || FMath::IsNearlyZero(PanDelta))
	{
		return;
	}

	PivotComponent->AddWorldOffset(SpringArm->GetRightVector() * PanDelta * PanSpeed);
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

	const FVector TargetLocation = (CameraMode == EApartmentLifeCameraMode::CharacterFace
		|| CameraMode == EApartmentLifeCameraMode::CharacterOutfit
		|| CameraMode == EApartmentLifeCameraMode::CharacterFullBody
		|| CameraMode == EApartmentLifeCameraMode::PosePreview
		|| CameraMode == EApartmentLifeCameraMode::AnimationPreview)
		? GetCharacterFocusPoint(FocusTarget, ActiveCharacterFocus)
		: FocusTarget->GetActorLocation();

	PivotComponent->SetWorldLocation(FMath::VInterpTo(PivotComponent->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 8.f));
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
		CurrentPitch = -89.f;
		CurrentArmLength = FMath::Clamp(CurrentArmLength, 400.f, MaxArmLength);
		SpringArm->SetTargetArmLengthSmooth(CurrentArmLength);
	}
	else
	{
		CameraMode = EApartmentLifeCameraMode::Orbit;
		CurrentPitch = -25.f;
	}
}

void AApartmentLifeCameraPawn::FocusRoom(const FBox& RoomBounds)
{
	CameraMode = EApartmentLifeCameraMode::RoomFocus;
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
	bFocusLockEnabled = true;
	ApplyCharacterFocusFraming(FocusMode);
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

	const FRotator NewRotation = FocusTarget->GetActorRotation() + FRotator(0.f, YawDelta, 0.f);
	FocusTarget->SetActorRotation(NewRotation);
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
		return Origin + FVector(0.f, 0.f, Extent.Z * 0.75f);
	case EApartmentLifeCharacterFocusMode::Outfit:
		return Origin + FVector(0.f, 0.f, Extent.Z * 0.35f);
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
	case EApartmentLifeCharacterFocusMode::PosePreview: return EApartmentLifeCameraMode::PosePreview;
	case EApartmentLifeCharacterFocusMode::AnimationPreview: return EApartmentLifeCameraMode::AnimationPreview;
	default: return EApartmentLifeCameraMode::CharacterFullBody;
	}
}

void AApartmentLifeCameraPawn::OnOrbitPressed()
{
	bOrbitInputActive = true;
}

void AApartmentLifeCameraPawn::OnOrbitReleased()
{
	bOrbitInputActive = false;
}

void AApartmentLifeCameraPawn::OnPanPressed()
{
	bPanInputActive = true;
}

void AApartmentLifeCameraPawn::OnPanReleased()
{
	bPanInputActive = false;
}

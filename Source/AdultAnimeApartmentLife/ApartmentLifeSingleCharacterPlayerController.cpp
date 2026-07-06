// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSingleCharacterPlayerController.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeInteractionComponent.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeInteractionHudComponent.h"
#include "ApartmentLifeInteractionTypes.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeWardrobeUiController.h"
#include "ApartmentLifeWardrobeShoppingComponent.h"
#include "ApartmentLifeWorkUiController.h"
#include "ApartmentLifeFinanceUiController.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeCameraSettingsSubsystem.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeDebugMenuComponent.h"
#include "ApartmentLifeDeveloperUiController.h"
#include "ApartmentLifeDeveloperSubsystem.h"
#include "ApartmentLifeUiBridgeComponent.h"
#include "ApartmentLifeUiSubsystem.h"
#include "ApartmentLifeCharacterCreatorUiController.h"
#include "ApartmentLifeImmersionSubsystem.h"
#include "ApartmentLifeImmersionTypes.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "Engine/World.h"

AApartmentLifeSingleCharacterPlayerController::AApartmentLifeSingleCharacterPlayerController()
{
	DebugMenuComponent = CreateDefaultSubobject<UApartmentLifeDebugMenuComponent>(TEXT("DebugMenu"));
	InteractionHudComponent = CreateDefaultSubobject<UApartmentLifeInteractionHudComponent>(TEXT("InteractionHud"));
	WardrobeUiController = CreateDefaultSubobject<UApartmentLifeWardrobeUiController>(TEXT("WardrobeUi"));
	WardrobeShoppingComponent = CreateDefaultSubobject<UApartmentLifeWardrobeShoppingComponent>(TEXT("WardrobeShopping"));
	WorkUiController = CreateDefaultSubobject<UApartmentLifeWorkUiController>(TEXT("WorkUi"));
	FinanceUiController = CreateDefaultSubobject<UApartmentLifeFinanceUiController>(TEXT("FinanceUi"));
	UiBridgeComponent = CreateDefaultSubobject<UApartmentLifeUiBridgeComponent>(TEXT("UiBridge"));
	CreatorUiController = CreateDefaultSubobject<UApartmentLifeCharacterCreatorUiController>(TEXT("CreatorUi"));
	DeveloperUiController = CreateDefaultSubobject<UApartmentLifeDeveloperUiController>(TEXT("DeveloperUi"));
}

void AApartmentLifeSingleCharacterPlayerController::SetSingleCharacterContext(
	AApartmentLifeApartmentUnit* InApartment,
	AApartmentLifeSimCharacter* InGirlCharacter,
	bool bInEnterGameplayDirectly)
{
	ApartmentUnit = InApartment;
	GirlCharacter = InGirlCharacter;
	bEnterGameplayDirectly = bInEnterGameplayDirectly;

	if (DebugMenuComponent)
	{
		DebugMenuComponent->InitializeContext(this, InApartment, InGirlCharacter);
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (InApartment)
		{
			CameraPawn->SetApartmentBounds(FBox(InApartment->GetActorLocation() - FVector(500.f), InApartment->GetActorLocation() + FVector(500.f)));
		}

		if (InGirlCharacter)
		{
			CameraPawn->SetFocusTarget(InGirlCharacter);
		}
		else if (InApartment)
		{
			CameraPawn->SetFocusTarget(InApartment);
		}
	}

	if (InteractionHudComponent && InGirlCharacter)
	{
		InteractionHudComponent->BindSelectionComponent(InGirlCharacter->GetInteractionSelectionComponent());
	}

	if (InGirlCharacter && InGirlCharacter->GetActivityComponent())
	{
		InGirlCharacter->GetActivityComponent()->OnActivityStarted.AddDynamic(this, &AApartmentLifeSingleCharacterPlayerController::HandleGirlActivityStarted);
		InGirlCharacter->GetActivityComponent()->OnActivityCompleted.AddDynamic(this, &AApartmentLifeSingleCharacterPlayerController::HandleGirlActivityCompleted);
	}

	if (WardrobeUiController && InGirlCharacter)
	{
		WardrobeUiController->InitializeContext(
			InGirlCharacter->GetWardrobeComponent(),
			InGirlCharacter->GetSimulationComponent());
	}

	if (WardrobeShoppingComponent && InGirlCharacter)
	{
		WardrobeShoppingComponent->InitializeContext(
			InGirlCharacter->GetWardrobeComponent(),
			InGirlCharacter->GetSimulationComponent());
	}

	if (WorkUiController && InGirlCharacter)
	{
		WorkUiController->InitializeContext(
			InGirlCharacter->GetSimulationComponent(),
			InGirlCharacter->GetProgressionComponent(),
			InGirlCharacter->GetActivityComponent());
	}

	if (FinanceUiController && InGirlCharacter)
	{
		FinanceUiController->InitializeContext(
			InGirlCharacter->GetSimulationComponent(),
			InGirlCharacter->GetProgressionComponent());
	}

	if (UiBridgeComponent)
	{
		UiBridgeComponent->OnPostLoadRequested.AddDynamic(this, &AApartmentLifeSingleCharacterPlayerController::ApplyPostLoadState);
		UiBridgeComponent->InitializeContext(
			this,
			InApartment,
			InGirlCharacter,
			WardrobeUiController,
			WorkUiController,
			FinanceUiController,
			InteractionHudComponent,
			CreatorUiController,
			DeveloperUiController,
			QuickSaveSlot,
			bEnterGameplayDirectly);
	}

	if (DeveloperUiController)
	{
		DeveloperUiController->InitializeContext(this, InApartment, InGirlCharacter);
	}

	if (CreatorUiController && InGirlCharacter)
	{
		CreatorUiController->InitializeContext(
			InGirlCharacter->GetCreatorComponent(),
			InGirlCharacter->GetWardrobeComponent(),
			InGirlCharacter->GetAnimationComponent(),
			GetCameraPawn());
	}
}

void AApartmentLifeSingleCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnInteract);
	InputComponent->BindAction(TEXT("SelectInteractable"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnSelectInteractable);
	InputComponent->BindAction(TEXT("CameraFocus"), IE_DoubleClick, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusDoubleClick);
	InputComponent->BindAction(TEXT("ExecuteAction1"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnExecuteAction1);
	InputComponent->BindAction(TEXT("ExecuteAction2"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnExecuteAction2);
	InputComponent->BindAction(TEXT("ExecuteAction3"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnExecuteAction3);
	InputComponent->BindAction(TEXT("ExecuteAction4"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnExecuteAction4);
	InputComponent->BindAction(TEXT("FocusGirlUpperBody"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlUpperBody);
	InputComponent->BindAction(TEXT("FocusGirlHair"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlHair);
	InputComponent->BindAction(TEXT("FocusGirlShoes"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlShoes);
	InputComponent->BindAction(TEXT("PhotoFovUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoFovUp);
	InputComponent->BindAction(TEXT("PhotoFovDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoFovDown);
	InputComponent->BindAction(TEXT("PhotoApertureUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoApertureUp);
	InputComponent->BindAction(TEXT("PhotoApertureDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoApertureDown);
	InputComponent->BindAction(TEXT("PhotoScreenshot"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoScreenshot);
	InputComponent->BindAction(TEXT("PhotoSaveBookmark"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnPhotoSaveBookmark);
	InputComponent->BindAction(TEXT("ToggleBuildMode"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleBuildMode);
	InputComponent->BindAction(TEXT("BuildTopDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildTopDown);
	InputComponent->BindAction(TEXT("OpenWardrobe"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenWardrobe);
	InputComponent->BindAction(TEXT("OpenWorkMenu"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenWorkMenu);
	InputComponent->BindAction(TEXT("OpenBudget"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleBudget);
	InputComponent->BindAction(TEXT("OpenApartmentShop"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenApartmentShop);
	InputComponent->BindAction(TEXT("StartSelectedWork"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnStartSelectedWork);
	InputComponent->BindAction(TEXT("SelectWork1"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnSelectWork1);
	InputComponent->BindAction(TEXT("SelectWork2"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnSelectWork2);
	InputComponent->BindAction(TEXT("SelectWork3"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnSelectWork3);
	InputComponent->BindAction(TEXT("SelectWork4"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnSelectWork4);
	InputComponent->BindAction(TEXT("BuildUndo"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildUndo);
	InputComponent->BindAction(TEXT("BuildRedo"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildRedo);
	InputComponent->BindAction(TEXT("QuickSave"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnQuickSave);
	InputComponent->BindAction(TEXT("QuickLoad"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnQuickLoad);
	InputComponent->BindAction(TEXT("TalkWithGirl"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnTalkWithGirl);
	InputComponent->BindAction(TEXT("FocusGirlFace"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlFace);
	InputComponent->BindAction(TEXT("FocusGirlOutfit"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlOutfit);
	InputComponent->BindAction(TEXT("ToggleDebugMenu"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleDebugMenu);
	InputComponent->BindAction(TEXT("TogglePerfOverlay"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnTogglePerfOverlay);
	InputComponent->BindAction(TEXT("UiBack"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnUiBack);
	InputComponent->BindAction(TEXT("ToggleGameHud"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleGameHud);
	InputComponent->BindAction(TEXT("OpenSaveLoadScreen"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenSaveLoadScreen);
	InputComponent->BindAction(TEXT("OpenProfileScreen"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenProfileScreen);
	InputComponent->BindAction(TEXT("OpenRoutinesScreen"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenRoutinesScreen);
	InputComponent->BindAction(TEXT("CharacterCreatorCamera"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnCharacterCreatorCamera);

	InputComponent->BindAction(TEXT("DebugAddMoney"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugAddMoney);
	InputComponent->BindAction(TEXT("DebugAdvanceHour"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugAdvanceHour);
	InputComponent->BindAction(TEXT("DebugSetMorning"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugSetMorning);
	InputComponent->BindAction(TEXT("DebugMoodUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugMoodUp);
	InputComponent->BindAction(TEXT("DebugMoodDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugMoodDown);
	InputComponent->BindAction(TEXT("DebugEnergyUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugEnergyUp);
	InputComponent->BindAction(TEXT("DebugEnergyDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugEnergyDown);
	InputComponent->BindAction(TEXT("DebugHygieneUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugHygieneUp);
	InputComponent->BindAction(TEXT("DebugHygieneDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugHygieneDown);
	InputComponent->BindAction(TEXT("DebugCycleOutfit"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugCycleOutfit);
	InputComponent->BindAction(TEXT("DebugTeleportBedroom"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportBedroom);
	InputComponent->BindAction(TEXT("DebugTeleportBathroom"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportBathroom);
	InputComponent->BindAction(TEXT("DebugTeleportLivingRoom"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportLivingRoom);
	InputComponent->BindAction(TEXT("DebugTeleportKitchen"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportKitchen);
	InputComponent->BindAction(TEXT("DebugTeleportOffice"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportOffice);
	InputComponent->BindAction(TEXT("DebugTriggerYoga"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugTriggerYoga);
	InputComponent->BindAction(TEXT("DebugSaveNow"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugSaveNow);
	InputComponent->BindAction(TEXT("DebugLoadNow"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugLoadNow);
	InputComponent->BindAction(TEXT("DebugResetApartment"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugResetApartment);
	InputComponent->BindAction(TEXT("DebugResetCharacter"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugResetCharacter);
	InputComponent->BindAction(TEXT("DebugPrintCreatorData"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugPrintCreatorData);
	InputComponent->BindAction(TEXT("DebugResetCreatorFace"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugResetCreatorFace);
	InputComponent->BindAction(TEXT("DebugResetCreatorBody"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugResetCreatorBody);
	InputComponent->BindAction(TEXT("DebugRemoveMoney"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugRemoveMoney);
	InputComponent->BindAction(TEXT("DebugStressUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugStressUp);
	InputComponent->BindAction(TEXT("DebugStressDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugStressDown);
	InputComponent->BindAction(TEXT("DebugHungerUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugHungerUp);
	InputComponent->BindAction(TEXT("DebugHungerDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugHungerDown);
	InputComponent->BindAction(TEXT("DebugComfortUp"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugComfortUp);
	InputComponent->BindAction(TEXT("DebugComfortDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugComfortDown);
	InputComponent->BindAction(TEXT("DebugUnlockClothing"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugUnlockClothing);
	InputComponent->BindAction(TEXT("DebugUnlockShop"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugUnlockShop);
	InputComponent->BindAction(TEXT("DebugCompleteActivity"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugCompleteActivity);
	InputComponent->BindAction(TEXT("DebugSpawnDesk"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugSpawnDesk);
	InputComponent->BindAction(TEXT("DebugDeleteSave"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugDeleteSave);
	InputComponent->BindAction(TEXT("DebugClearSaves"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnDebugClearSaves);
}

UApartmentLifeBuildModeComponent* AApartmentLifeSingleCharacterPlayerController::GetBuildMode() const
{
	return ApartmentUnit.IsValid() ? ApartmentUnit->GetBuildModeComponent() : nullptr;
}

UApartmentLifeInteractionComponent* AApartmentLifeSingleCharacterPlayerController::GetGirlInteraction() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeInteractionComponent>()
		: nullptr;
}

UApartmentLifeInteractionSelectionComponent* AApartmentLifeSingleCharacterPlayerController::GetGirlSelection() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->GetInteractionSelectionComponent()
		: nullptr;
}

void AApartmentLifeSingleCharacterPlayerController::FocusCameraOnInteractable(AActor* Target, const FVector& FocusOffset)
{
	if (!Target)
	{
		return;
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (Cast<AApartmentLifeFurnitureActor>(Target))
		{
			CameraPawn->FocusFurniture(Target, FocusOffset);
		}
		else if (Cast<AApartmentLifeSimCharacter>(Target))
		{
			CameraPawn->FocusCharacter(Target, EApartmentLifeCharacterFocusMode::FullBody);
		}
		else
		{
			CameraPawn->SetFocusTarget(Target);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::FocusCameraForActivity(FName ActivityId, AActor* ContextActor)
{
	if (!GirlCharacter.IsValid())
	{
		return;
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->EnterActivityCamera(GirlCharacter.Get(), ActivityId, ContextActor);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnInteract()
{
	if (UApartmentLifeInteractionSelectionComponent* Selection = GetGirlSelection())
	{
		if (Selection->GetSelectedInteractable())
		{
			Selection->ExecuteInteractionByIndex(0);
			FocusCameraForActivity(
				Selection->GetAvailableActions().Num() > 0
					? Selection->GetAvailableActions()[0].ActivityId
					: NAME_None,
				Selection->GetSelectedInteractable());
			return;
		}
	}

	if (UApartmentLifeInteractionComponent* Interaction = GetGirlInteraction())
	{
		if (Interaction->TryInteractFromView(this))
		{
			FocusCameraOnInteractable(Interaction->GetFocusedInteractable());
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnSelectInteractable()
{
	if (bBuildModeActive)
	{
		return;
	}

	if (UApartmentLifeInteractionSelectionComponent* Selection = GetGirlSelection())
	{
		if (Selection->SelectFromView(this))
		{
			FocusCameraOnInteractable(Selection->GetSelectedInteractable());
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnFocusDoubleClick()
{
	if (UApartmentLifeInteractionSelectionComponent* Selection = GetGirlSelection())
	{
		if (AActor* Target = Selection->GetSelectedInteractable())
		{
			if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
			{
				CameraPawn->FocusTargetFromDoubleClick(Target);
			}
			return;
		}

		if (Selection->SelectFromView(this))
		{
			if (AActor* Target = Selection->GetSelectedInteractable())
			{
				if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
				{
					CameraPawn->FocusTargetFromDoubleClick(Target);
				}
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnExecuteAction1() { if (UApartmentLifeInteractionSelectionComponent* S = GetGirlSelection()) { if (S->ExecuteInteractionByIndex(0)) FocusCameraForActivity(S->GetAvailableActions()[0].ActivityId, S->GetSelectedInteractable()); } }
void AApartmentLifeSingleCharacterPlayerController::OnExecuteAction2() { if (UApartmentLifeInteractionSelectionComponent* S = GetGirlSelection()) { if (S->ExecuteInteractionByIndex(1)) FocusCameraForActivity(S->GetAvailableActions()[1].ActivityId, S->GetSelectedInteractable()); } }
void AApartmentLifeSingleCharacterPlayerController::OnExecuteAction3() { if (UApartmentLifeInteractionSelectionComponent* S = GetGirlSelection()) { if (S->ExecuteInteractionByIndex(2)) FocusCameraForActivity(S->GetAvailableActions()[2].ActivityId, S->GetSelectedInteractable()); } }
void AApartmentLifeSingleCharacterPlayerController::OnExecuteAction4() { if (UApartmentLifeInteractionSelectionComponent* S = GetGirlSelection()) { if (S->ExecuteInteractionByIndex(3)) FocusCameraForActivity(S->GetAvailableActions()[3].ActivityId, S->GetSelectedInteractable()); } }

void AApartmentLifeSingleCharacterPlayerController::OnFocusGirlUpperBody()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::UpperBody);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnFocusGirlHair()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Hair);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnFocusGirlShoes()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Shoes);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoFovUp()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->AdjustPhotoFieldOfView(2.f);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoFovDown()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->AdjustPhotoFieldOfView(-2.f);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoApertureUp()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->AdjustPhotoAperture(-0.2f);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoApertureDown()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->AdjustPhotoAperture(0.2f);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoScreenshot()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->TakeHighResScreenshot();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnPhotoSaveBookmark()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UApartmentLifeImmersionSubsystem* Immersion = GI->GetSubsystem<UApartmentLifeImmersionSubsystem>())
			{
				FApartmentLifePhotoBookmark Bookmark;
				Bookmark.BookmarkId = FName(*FString::Printf(TEXT("photo.bookmark.%d"),
					Immersion->GetUserSettings().PhotoBookmarks.Num()));
				Bookmark.DisplayName = FText::FromString(FString::Printf(TEXT("Bookmark %d"),
					Immersion->GetUserSettings().PhotoBookmarks.Num() + 1));
				Bookmark.Location = CameraPawn->GetActorLocation();
				Bookmark.Rotation = CameraPawn->GetActorRotation();
				const FApartmentLifeCameraUserSettings CamSettings = CameraPawn->BuildUserSettings();
				Bookmark.FieldOfView = CamSettings.PhotoSettings.FieldOfView;
				Bookmark.Aperture = CamSettings.PhotoSettings.Aperture;
				Bookmark.TimeOfDayHour = CamSettings.PhotoSettings.TimeOfDayHour;
				Bookmark.LightingPreset = static_cast<EApartmentLifeLightingPreset>(CamSettings.PhotoSettings.LightingPresetIndex);
				Immersion->SavePhotoBookmark(Bookmark);
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::HandleGirlActivityStarted(FName ActivityId)
{
	if (UApartmentLifeInteractionSelectionComponent* Selection = GetGirlSelection())
	{
		FocusCameraForActivity(ActivityId, Selection->GetSelectedInteractable());
	}
	else
	{
		FocusCameraForActivity(ActivityId, nullptr);
	}
}

void AApartmentLifeSingleCharacterPlayerController::HandleGirlActivityCompleted(FName ActivityId)
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (CameraPawn->GetPrimaryCameraMode() == EApartmentLifePrimaryCameraMode::Activity)
		{
			CameraPawn->ExitActivityCamera();
		}
		else if (CameraPawn->GetPrimaryCameraMode() == EApartmentLifePrimaryCameraMode::Wardrobe)
		{
			CameraPawn->ExitWardrobeCamera();
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::CloseWardrobeSession()
{
	if (WardrobeUiController && WardrobeUiController->IsWardrobeOpen())
	{
		WardrobeUiController->CloseWardrobe();
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (CameraPawn->GetPrimaryCameraMode() == EApartmentLifePrimaryCameraMode::Wardrobe)
		{
			CameraPawn->ExitWardrobeCamera();
		}
	}

	if (GirlCharacter.IsValid())
	{
		if (UApartmentLifeActivityComponent* Activity = GirlCharacter->GetActivityComponent())
		{
			if (Activity->IsActivityActive())
			{
				const FString Id = Activity->GetCurrentActivityId().ToString().ToLower();
				if (Id.Contains(TEXT("wardrobe")) || Id.Contains(TEXT("dress")))
				{
					Activity->CancelCurrentActivity();
				}
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::CloseBuildModeSession()
{
	bBuildModeActive = false;

	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->ExitBuildMode();
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->SetBuildTopDownMode(false);
		CameraPawn->ReturnToApartmentCamera();
	}
}

void AApartmentLifeSingleCharacterPlayerController::CloseConversationSession()
{
	if (!GirlCharacter.IsValid())
	{
		return;
	}

	if (UApartmentLifeConversationComponent* Conversation = GirlCharacter->GetConversationComponent())
	{
		if (Conversation->GetCurrentSession().bIsActive)
		{
			Conversation->EndConversation();
		}
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (CameraPawn->GetPrimaryCameraMode() == EApartmentLifePrimaryCameraMode::CharacterFocus)
		{
			CameraPawn->ReturnToApartmentCamera();
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnToggleBuildMode()
{
	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		bBuildModeActive = !bBuildModeActive;
		if (bBuildModeActive)
		{
			BuildMode->EnterBuildMode();
			if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
			{
				CameraPawn->EnterBuildModeCamera(ApartmentUnit.Get());
			}
			if (UiBridgeComponent)
			{
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UApartmentLifeUiSubsystem* Ui = GI->GetSubsystem<UApartmentLifeUiSubsystem>())
					{
						Ui->ShowScreen(EApartmentLifeUiScreen::BuildMode);
					}
				}
			}
		}
		else
		{
			CloseBuildModeSession();
			if (UiBridgeComponent)
			{
				UiBridgeComponent->HandleUiBack();
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnBuildTopDown()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		const bool bEnable = CameraPawn->GetCameraMode() != EApartmentLifeCameraMode::TopDown
			&& CameraPawn->GetCameraMode() != EApartmentLifeCameraMode::BuildMode;
		CameraPawn->SetBuildTopDownMode(bEnable);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenWardrobe()
{
	if (!GirlCharacter.IsValid())
	{
		return;
	}

	if (WardrobeUiController && WardrobeUiController->IsWardrobeOpen())
	{
		CloseWardrobeSession();
		return;
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->EnterWardrobeCamera(GirlCharacter.Get());
	}

	if (WardrobeUiController)
	{
		WardrobeUiController->OpenWardrobe();
	}

	if (UApartmentLifeActivityComponent* Activity = GirlCharacter->GetActivityComponent())
	{
		Activity->StartActivity(FName(TEXT("activity.dress.wardrobe")));
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenWorkMenu()
{
	if (!WorkUiController)
	{
		return;
	}

	if (WorkUiController->IsWorkMenuOpen())
	{
		WorkUiController->CloseWorkSelection();
	}
	else
	{
		WorkUiController->OpenWorkSelection();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnToggleBudget()
{
	if (FinanceUiController)
	{
		FinanceUiController->ToggleBudgetOverlay();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenApartmentShop()
{
	if (FinanceUiController)
	{
		FinanceUiController->ToggleShoppingOverlay(EApartmentLifeShopCategory::Furniture);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnStartSelectedWork()
{
	if (WorkUiController && WorkUiController->IsWorkMenuOpen())
	{
		WorkUiController->StartSelectedWork();
	}
}

void AApartmentLifeSingleCharacterPlayerController::SelectWorkByIndex(int32 Index)
{
	if (WorkUiController && WorkUiController->IsWorkMenuOpen())
	{
		WorkUiController->SelectWorkIndex(Index);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnSelectWork1() { SelectWorkByIndex(0); }
void AApartmentLifeSingleCharacterPlayerController::OnSelectWork2() { SelectWorkByIndex(1); }
void AApartmentLifeSingleCharacterPlayerController::OnSelectWork3() { SelectWorkByIndex(2); }
void AApartmentLifeSingleCharacterPlayerController::OnSelectWork4() { SelectWorkByIndex(3); }

void AApartmentLifeSingleCharacterPlayerController::OnBuildUndo()
{
	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->Undo();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnBuildRedo()
{
	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->Redo();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnQuickSave()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UApartmentLifeCameraSettingsSubsystem* CameraSettings = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
			{
				CameraSettings->CaptureFromPawn(CameraPawn);
			}
		}
	}

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			const bool bSaved = SaveSubsystem->SaveToSlot(QuickSaveSlot);
			if (UiBridgeComponent && GI)
			{
				if (UApartmentLifeUiSubsystem* Ui = GI->GetSubsystem<UApartmentLifeUiSubsystem>())
				{
					Ui->ShowToast(
						bSaved
							? FText::FromString(TEXT("Quick save complete"))
							: FText::FromString(TEXT("Quick save failed")),
						2.f);
				}
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnQuickLoad()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			if (!SaveSubsystem->DoesSaveExist(QuickSaveSlot))
			{
				if (UApartmentLifeUiSubsystem* Ui = GI->GetSubsystem<UApartmentLifeUiSubsystem>())
				{
					Ui->ShowToast(FText::FromString(TEXT("No quick save found")), 2.f);
				}
				return;
			}

			const bool bLoaded = SaveSubsystem->LoadFromSlot(QuickSaveSlot);
			if (bLoaded)
			{
				ApplyPostLoadState();
				if (UApartmentLifeUiSubsystem* Ui = GI->GetSubsystem<UApartmentLifeUiSubsystem>())
				{
					Ui->ShowToast(FText::FromString(TEXT("Quick load complete")), 2.f);
				}
			}
			else if (UApartmentLifeUiSubsystem* Ui = GI->GetSubsystem<UApartmentLifeUiSubsystem>())
			{
				Ui->ShowToast(FText::FromString(TEXT("Quick load failed")), 2.f);
			}
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::ApplyPostLoadState()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			if (UApartmentLifeCameraSettingsSubsystem* CameraSettings = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
			{
				CameraSettings->ApplySettingsToPawn(CameraPawn);
			}
		}

		if (UApartmentLifeImmersionSubsystem* Immersion = GI->GetSubsystem<UApartmentLifeImmersionSubsystem>())
		{
			if (UWorld* World = GetWorld())
			{
				if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
				{
					const FApartmentLifeGameTime& Time = TimeSubsystem->GetCurrentTime();
					Immersion->UpdateFromGameTime(Time.Hour, TimeSubsystem->GetCurrentWeather().Weather);
				}
			}
		}

		if (UiBridgeComponent)
		{
			UiBridgeComponent->SyncImmersionSettingsFromSubsystem();
			UiBridgeComponent->RefreshAfterLoad();
		}
	}

	if (GirlCharacter.IsValid())
	{
		if (UApartmentLifeProgressionComponent* Progression = GirlCharacter->GetProgressionComponent())
		{
			Progression->ReapplyOwnedUpgrades();
		}

		GirlCharacter->RestoreAnimationAfterLoad();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnTalkWithGirl()
{
	if (!GirlCharacter.IsValid())
	{
		return;
	}

	if (UApartmentLifeConversationComponent* Conversation = GirlCharacter->GetConversationComponent())
	{
		if (Conversation->GetCurrentSession().bIsActive)
		{
			CloseConversationSession();
			return;
		}

		Conversation->StartConversationWithPlayer();
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Face);
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnFocusGirlFace()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Face);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnFocusGirlOutfit()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Outfit);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnToggleDebugMenu()
{
	if (DeveloperUiController)
	{
		DeveloperUiController->ToggleDeveloperHub();
		return;
	}

	if (DebugMenuComponent)
	{
		DebugMenuComponent->ToggleMenu();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnTogglePerfOverlay()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->TogglePerformanceOverlay();
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnUiBack()
{
	if (GirlCharacter.IsValid())
	{
		if (UApartmentLifeConversationComponent* Conversation = GirlCharacter->GetConversationComponent())
		{
			if (Conversation->GetCurrentSession().bIsActive)
			{
				CloseConversationSession();
				return;
			}
		}
	}

	if (WardrobeUiController && WardrobeUiController->IsWardrobeOpen())
	{
		CloseWardrobeSession();
		return;
	}

	if (DeveloperUiController && DeveloperUiController->IsDeveloperHubOpen())
	{
		DeveloperUiController->CloseDeveloperHub();
		return;
	}

	if (UiBridgeComponent)
	{
		UiBridgeComponent->HandleUiBack();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnToggleGameHud()
{
	if (UiBridgeComponent)
	{
		UiBridgeComponent->ToggleHud();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenSaveLoadScreen()
{
	if (UiBridgeComponent)
	{
		UiBridgeComponent->OpenSaveLoadScreen();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenProfileScreen()
{
	if (UiBridgeComponent)
	{
		UiBridgeComponent->OpenProfileScreen();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnOpenRoutinesScreen()
{
	if (UiBridgeComponent)
	{
		UiBridgeComponent->OpenRoutinesScreen();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnCharacterCreatorCamera()
{
	if (CreatorUiController)
	{
		CreatorUiController->ToggleCreator();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnDebugMoodUp()
{
	if (CreatorUiController && CreatorUiController->IsCreatorOpen())
	{
		CreatorUiController->AdjustSelectedSlider(0.05f);
		return;
	}

	if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("MoodUp")));
}

void AApartmentLifeSingleCharacterPlayerController::OnDebugMoodDown()
{
	if (CreatorUiController && CreatorUiController->IsCreatorOpen())
	{
		CreatorUiController->AdjustSelectedSlider(-0.05f);
		return;
	}

	if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("MoodDown")));
}

void AApartmentLifeSingleCharacterPlayerController::OnDebugAddMoney() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("AddMoney"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugAdvanceHour() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("AdvanceHour"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugSetMorning() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("SetMorning"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugEnergyUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("EnergyUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugEnergyDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("EnergyDown"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugHygieneUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("HygieneUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugHygieneDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("HygieneDown"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugCycleOutfit() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("CycleOutfit"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportBedroom() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TeleportBedroom"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportBathroom() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TeleportBathroom"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportLivingRoom() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TeleportLivingRoom"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportKitchen() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TeleportKitchen"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTeleportOffice() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TeleportOffice"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugTriggerYoga() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("TriggerYoga"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugSaveNow() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("SaveNow"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugLoadNow() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("LoadNow"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugResetApartment() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ResetApartment"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugResetCharacter() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ResetCharacter"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugPrintCreatorData() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("PrintCreatorData"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugResetCreatorFace() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ResetCreatorFace"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugResetCreatorBody() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ResetCreatorBody"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugRemoveMoney() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("RemoveMoney"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugStressUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("StressUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugStressDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("StressDown"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugHungerUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("HungerUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugHungerDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("HungerDown"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugComfortUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ComfortUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugComfortDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ComfortDown"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugUnlockClothing() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("UnlockAllClothing"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugUnlockShop() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("UnlockAllShop"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugCompleteActivity() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("CompleteActivity"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugSpawnDesk() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("SpawnDesk"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugDeleteSave() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("DeleteSave"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugClearSaves() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("ClearSaves"))); }

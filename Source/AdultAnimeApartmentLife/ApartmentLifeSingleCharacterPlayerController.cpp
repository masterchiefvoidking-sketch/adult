// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSingleCharacterPlayerController.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeInteractionComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeDebugMenuComponent.h"
#include "Engine/World.h"

AApartmentLifeSingleCharacterPlayerController::AApartmentLifeSingleCharacterPlayerController()
{
	DebugMenuComponent = CreateDefaultSubobject<UApartmentLifeDebugMenuComponent>(TEXT("DebugMenu"));
}

void AApartmentLifeSingleCharacterPlayerController::SetSingleCharacterContext(
	AApartmentLifeApartmentUnit* InApartment,
	AApartmentLifeSimCharacter* InGirlCharacter)
{
	ApartmentUnit = InApartment;
	GirlCharacter = InGirlCharacter;

	if (DebugMenuComponent)
	{
		DebugMenuComponent->InitializeContext(this, InApartment, InGirlCharacter);
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (InGirlCharacter)
		{
			CameraPawn->SetFocusTarget(InGirlCharacter);
		}
		else if (InApartment)
		{
			CameraPawn->SetFocusTarget(InApartment);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnInteract);
	InputComponent->BindAction(TEXT("ToggleBuildMode"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleBuildMode);
	InputComponent->BindAction(TEXT("BuildTopDown"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildTopDown);
	InputComponent->BindAction(TEXT("OpenWardrobe"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnOpenWardrobe);
	InputComponent->BindAction(TEXT("BuildUndo"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildUndo);
	InputComponent->BindAction(TEXT("BuildRedo"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnBuildRedo);
	InputComponent->BindAction(TEXT("QuickSave"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnQuickSave);
	InputComponent->BindAction(TEXT("QuickLoad"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnQuickLoad);
	InputComponent->BindAction(TEXT("TalkWithGirl"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnTalkWithGirl);
	InputComponent->BindAction(TEXT("FocusGirlFace"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlFace);
	InputComponent->BindAction(TEXT("FocusGirlOutfit"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnFocusGirlOutfit);
	InputComponent->BindAction(TEXT("ToggleDebugMenu"), IE_Pressed, this, &AApartmentLifeSingleCharacterPlayerController::OnToggleDebugMenu);
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

void AApartmentLifeSingleCharacterPlayerController::FocusCameraOnInteractable(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (Cast<AApartmentLifeFurnitureActor>(Target))
		{
			CameraPawn->FocusFurniture(Target);
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

void AApartmentLifeSingleCharacterPlayerController::OnInteract()
{
	if (UApartmentLifeInteractionComponent* Interaction = GetGirlInteraction())
	{
		if (Interaction->TryInteractFromView(this))
		{
			FocusCameraOnInteractable(Interaction->GetFocusedInteractable());
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
		}
		else
		{
			BuildMode->ExitBuildMode();
			if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
			{
				CameraPawn->SetBuildTopDownMode(false);
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

	if (UApartmentLifeWardrobeComponent* Wardrobe = GirlCharacter->GetWardrobeComponent())
	{
		Wardrobe->SelectOutfitForOutfitContext(EApartmentLifeOutfitContext::Everyday, FApartmentLifeWeatherState());
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->FocusCharacter(GirlCharacter.Get(), EApartmentLifeCharacterFocusMode::Outfit);
	}

	if (UApartmentLifeActivityComponent* Activity = GirlCharacter->GetActivityComponent())
	{
		Activity->StartActivity(FName(TEXT("activity.dress.wardrobe")));
	}
}

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
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			SaveSubsystem->SaveToSlot(QuickSaveSlot);
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnQuickLoad()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			SaveSubsystem->LoadFromSlot(QuickSaveSlot);
		}
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
	if (DebugMenuComponent)
	{
		DebugMenuComponent->ToggleMenu();
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnCharacterCreatorCamera()
{
	if (GirlCharacter.IsValid())
	{
		if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
		{
			CameraPawn->EnterCharacterCreatorMode(GirlCharacter.Get());
		}
	}
}

void AApartmentLifeSingleCharacterPlayerController::OnDebugAddMoney() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("AddMoney"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugAdvanceHour() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("AdvanceHour"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugSetMorning() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("SetMorning"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugMoodUp() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("MoodUp"))); }
void AApartmentLifeSingleCharacterPlayerController::OnDebugMoodDown() { if (DebugMenuComponent) DebugMenuComponent->HandleDebugAction(FName(TEXT("MoodDown"))); }
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

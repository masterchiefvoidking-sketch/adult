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
#include "Engine/World.h"

AApartmentLifeSingleCharacterPlayerController::AApartmentLifeSingleCharacterPlayerController()
{
}

void AApartmentLifeSingleCharacterPlayerController::SetSingleCharacterContext(
	AApartmentLifeApartmentUnit* InApartment,
	AApartmentLifeSimCharacter* InGirlCharacter)
{
	ApartmentUnit = InApartment;
	GirlCharacter = InGirlCharacter;

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
		Wardrobe->SelectOutfitForOutfitContext(EApartmentLifeOutfitContext::Casual, FApartmentLifeWeatherState());
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

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSlicePlayerController.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeInteractionComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeFurnitureActor.h"
#include "Engine/World.h"

AApartmentLifeSlicePlayerController::AApartmentLifeSlicePlayerController()
{
}

void AApartmentLifeSlicePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AApartmentLifeSlicePlayerController::SetSliceContext(
	AApartmentLifeApartmentUnit* InApartment,
	AApartmentLifeSimCharacter* InPlayerCharacter,
	AApartmentLifeSimCharacter* InPartnerCharacter)
{
	ApartmentUnit = InApartment;
	PlayerCharacter = InPlayerCharacter;
	PartnerCharacter = InPartnerCharacter;

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		if (InPlayerCharacter)
		{
			CameraPawn->SetFocusTarget(InPlayerCharacter);
		}
		else if (InApartment)
		{
			CameraPawn->SetFocusTarget(InApartment);
		}
	}
}

void AApartmentLifeSlicePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnInteract);
	InputComponent->BindAction(TEXT("ToggleBuildMode"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnToggleBuildMode);
	InputComponent->BindAction(TEXT("BuildTopDown"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnBuildTopDown);
	InputComponent->BindAction(TEXT("OpenWardrobe"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnOpenWardrobe);
	InputComponent->BindAction(TEXT("BuildUndo"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnBuildUndo);
	InputComponent->BindAction(TEXT("BuildRedo"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnBuildRedo);
	InputComponent->BindAction(TEXT("QuickSave"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnQuickSave);
	InputComponent->BindAction(TEXT("QuickLoad"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnQuickLoad);
	InputComponent->BindAction(TEXT("TalkToPartner"), IE_Pressed, this, &AApartmentLifeSlicePlayerController::OnTalkToPartner);
}

UApartmentLifeBuildModeComponent* AApartmentLifeSlicePlayerController::GetBuildMode() const
{
	return ApartmentUnit.IsValid() ? ApartmentUnit->GetBuildModeComponent() : nullptr;
}

UApartmentLifeInteractionComponent* AApartmentLifeSlicePlayerController::GetPlayerInteraction() const
{
	return PlayerCharacter.IsValid()
		? PlayerCharacter->FindComponentByClass<UApartmentLifeInteractionComponent>()
		: nullptr;
}

void AApartmentLifeSlicePlayerController::FocusCameraOnInteractable(AActor* Target)
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

void AApartmentLifeSlicePlayerController::OnInteract()
{
	if (UApartmentLifeInteractionComponent* Interaction = GetPlayerInteraction())
	{
		if (Interaction->TryInteractFromView(this))
		{
			FocusCameraOnInteractable(Interaction->GetFocusedInteractable());
		}
	}
}

void AApartmentLifeSlicePlayerController::OnToggleBuildMode()
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

void AApartmentLifeSlicePlayerController::OnBuildTopDown()
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		const bool bEnable = CameraPawn->GetCameraMode() != EApartmentLifeCameraMode::TopDown
			&& CameraPawn->GetCameraMode() != EApartmentLifeCameraMode::BuildMode;
		CameraPawn->SetBuildTopDownMode(bEnable);
	}
}

void AApartmentLifeSlicePlayerController::OnOpenWardrobe()
{
	if (!PlayerCharacter.IsValid())
	{
		return;
	}

	if (UApartmentLifeWardrobeComponent* Wardrobe = PlayerCharacter->GetWardrobeComponent())
	{
		Wardrobe->SelectOutfitForOutfitContext(EApartmentLifeOutfitContext::Casual, FApartmentLifeWeatherState());
	}

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->FocusCharacter(PlayerCharacter.Get(), EApartmentLifeCharacterFocusMode::Outfit);
	}

	if (UApartmentLifeActivityComponent* Activity = PlayerCharacter->GetActivityComponent())
	{
		Activity->StartActivity(FName(TEXT("activity.dress.wardrobe")));
	}
}

void AApartmentLifeSlicePlayerController::OnBuildUndo()
{
	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->Undo();
	}
}

void AApartmentLifeSlicePlayerController::OnBuildRedo()
{
	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->Redo();
	}
}

void AApartmentLifeSlicePlayerController::OnQuickSave()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			SaveSubsystem->SaveToSlot(QuickSaveSlot);
		}
	}
}

void AApartmentLifeSlicePlayerController::OnQuickLoad()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
		{
			SaveSubsystem->LoadFromSlot(QuickSaveSlot);
		}
	}
}

void AApartmentLifeSlicePlayerController::OnTalkToPartner()
{
	if (!PlayerCharacter.IsValid() || !PartnerCharacter.IsValid())
	{
		return;
	}

	UApartmentLifeConversationComponent* Conversation = PlayerCharacter->GetConversationComponent();
	UApartmentLifeNPCSimulationComponent* SpeakerSim = PlayerCharacter->GetSimulationComponent();
	UApartmentLifeNPCSimulationComponent* ListenerSim = PartnerCharacter->GetSimulationComponent();
	if (!Conversation || !SpeakerSim || !ListenerSim)
	{
		return;
	}

	FApartmentLifeGameTime Time;
	FApartmentLifeWeatherState Weather;
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Time = TimeSubsystem->GetCurrentTime();
			Weather = TimeSubsystem->GetCurrentWeather();
		}
	}

	const FApartmentLifeDialogueContext Context =
		UApartmentLifeSocialLibrary::BuildDialogueContext(SpeakerSim, ListenerSim, Time, Weather, FName(TEXT("apartment.home")));
	Conversation->StartConversation(PartnerCharacter.Get(), Context);

	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->FocusCharacter(PartnerCharacter.Get(), EApartmentLifeCharacterFocusMode::Face);
	}
}

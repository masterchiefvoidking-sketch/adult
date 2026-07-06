// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeUiSubsystem.h"
#include "Slate/SApartmentLifeRootWidget.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"

void UApartmentLifeUiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Settings.bHudVisible = true;
	ActiveScreen = EApartmentLifeUiScreen::Gameplay;
}

void UApartmentLifeUiSubsystem::Deinitialize()
{
	if (GEngine && GEngine->GameViewport && RootWidget.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(RootWidget.ToSharedRef());
	}
	RootWidget.Reset();
	Super::Deinitialize();
}

void UApartmentLifeUiSubsystem::EnsureViewportUi()
{
	if (RootWidget.IsValid())
	{
		return;
	}

	RootWidget = SNew(SApartmentLifeRootWidget)
		.OnBackPressed(FSimpleDelegate::CreateUObject(this, &UApartmentLifeUiSubsystem::HandleBack))
		.OnNewGamePressed(FSimpleDelegate::CreateLambda([this]() { OnMainMenuNewGame.Broadcast(); }))
		.OnContinuePressed(FSimpleDelegate::CreateLambda([this]() { OnMainMenuContinue.Broadcast(0); }))
		.OnLoadPressed(FSimpleDelegate::CreateLambda([this]() { ShowScreen(EApartmentLifeUiScreen::SaveLoad); }))
		.OnSettingsPressed(FSimpleDelegate::CreateLambda([this]() { ShowScreen(EApartmentLifeUiScreen::Settings); }))
		.OnQuitPressed(FSimpleDelegate::CreateLambda([]()
		{
			if (GEngine)
			{
				GEngine->Exec(nullptr, TEXT("quit"));
			}
		}))
		.OnHudTogglePressed(FSimpleDelegate::CreateUObject(this, &UApartmentLifeUiSubsystem::ToggleHud));

	RootWidget->OnListItemClicked.BindUObject(this, &UApartmentLifeUiSubsystem::HandleListItemClicked);
	RootWidget->OnListItemConfirmed.BindUObject(this, &UApartmentLifeUiSubsystem::HandleListItemConfirmed);

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(RootWidget.ToSharedRef(), 0);
	}

	RefreshView();
}

void UApartmentLifeUiSubsystem::ShowScreen(EApartmentLifeUiScreen Screen)
{
	EnsureViewportUi();
	ActiveScreen = Screen;
	OnScreenChanged.Broadcast(ActiveScreen);
	RefreshView();
}

void UApartmentLifeUiSubsystem::CloseScreen()
{
	ActiveScreen = EApartmentLifeUiScreen::Gameplay;
	OnScreenChanged.Broadcast(ActiveScreen);
	RefreshView();
}

void UApartmentLifeUiSubsystem::ToggleHud()
{
	Settings.bHudVisible = !Settings.bHudVisible;
	RefreshView();
}

void UApartmentLifeUiSubsystem::SetHudSuppressed(bool bSuppressed)
{
	bHudSuppressed = bSuppressed;
	RefreshView();
}

void UApartmentLifeUiSubsystem::SetHudState(const FApartmentLifeHudViewModel& State)
{
	HudState = State;
	RefreshView();
}

void UApartmentLifeUiSubsystem::SetPanelState(const FApartmentLifeUiPanelState& State)
{
	PanelState = State;
	RefreshView();
}

void UApartmentLifeUiSubsystem::SetMainMenuSlots(const TArray<FApartmentLifeMainMenuSlotViewModel>& Slots)
{
	MainMenuSlots = Slots;
	RefreshView();
}

void UApartmentLifeUiSubsystem::SetYogaState(const FApartmentLifeYogaUiState& State)
{
	YogaState = State;
	RefreshView();
}

void UApartmentLifeUiSubsystem::ShowToast(const FText& Message, float DurationSeconds)
{
	ActiveToast.Message = Message;
	ActiveToast.RemainingSeconds = DurationSeconds;
	RefreshView();
}

void UApartmentLifeUiSubsystem::RefreshView()
{
	if (!RootWidget.IsValid())
	{
		return;
	}

	RootWidget->SetView(ActiveScreen, Settings.bHudVisible && !bHudSuppressed, HudState, PanelState, MainMenuSlots, YogaState, ActiveToast);
}

void UApartmentLifeUiSubsystem::HandleListItemClicked(int32 Index)
{
	PanelState.SelectedIndex = Index;
	RefreshView();
}

void UApartmentLifeUiSubsystem::HandleListItemConfirmed(int32 Index)
{
	PanelState.SelectedIndex = Index;
	OnListItemActivated.Broadcast(ActiveScreen, Index);
	RefreshView();
}

void UApartmentLifeUiSubsystem::ShowMainMenu()
{
	ShowScreen(EApartmentLifeUiScreen::MainMenu);
}

bool UApartmentLifeUiSubsystem::TickToast(float DeltaSeconds)
{
	if (ActiveToast.RemainingSeconds <= 0.f || ActiveToast.Message.IsEmpty())
	{
		return false;
	}

	ActiveToast.RemainingSeconds -= DeltaSeconds;
	if (ActiveToast.RemainingSeconds <= 0.f)
	{
		ActiveToast.Message = FText::GetEmpty();
		ActiveToast.RemainingSeconds = 0.f;
		RefreshView();
		return true;
	}

	return false;
}

void UApartmentLifeUiSubsystem::HandleBack()
{
	OnBackRequested.Broadcast();
	CloseScreen();
}

void UApartmentLifeUiSubsystem::PushViewToWidget()
{
	RefreshView();
}

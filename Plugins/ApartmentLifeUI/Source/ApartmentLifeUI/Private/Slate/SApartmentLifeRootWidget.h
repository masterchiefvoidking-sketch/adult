// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeUiTypes.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

class SApartmentLifeRootWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SApartmentLifeRootWidget) {}
		SLATE_EVENT(FSimpleDelegate, OnBackPressed)
		SLATE_EVENT(FSimpleDelegate, OnNewGamePressed)
		SLATE_EVENT(FSimpleDelegate, OnContinuePressed)
		SLATE_EVENT(FSimpleDelegate, OnLoadPressed)
		SLATE_EVENT(FSimpleDelegate, OnSettingsPressed)
		SLATE_EVENT(FSimpleDelegate, OnQuitPressed)
		SLATE_EVENT(FSimpleDelegate, OnHudTogglePressed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetView(
		EApartmentLifeUiScreen Screen,
		bool bHudVisible,
		const FApartmentLifeHudViewModel& Hud,
		const FApartmentLifeUiPanelState& Panel,
		const TArray<FApartmentLifeMainMenuSlotViewModel>& MainMenuSlots,
		const FApartmentLifeYogaUiState& Yoga,
		const FApartmentLifeUiToast& Toast);

	DECLARE_DELEGATE_OneParam(FOnListItemClicked, int32);
	FOnListItemClicked OnListItemClicked;
	DECLARE_DELEGATE_OneParam(FOnListItemConfirmed, int32);
	FOnListItemConfirmed OnListItemConfirmed;

private:
	TSharedRef<class ITableRow> OnGenerateListRow(TSharedPtr<FApartmentLifeUiListEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);
	FReply HandleListItemDoubleClicked(TSharedPtr<FApartmentLifeUiListEntry> Item);

	EApartmentLifeUiScreen CurrentScreen = EApartmentLifeUiScreen::Gameplay;
	FSimpleDelegate BackDelegate;
	FSimpleDelegate NewGameDelegate;
	FSimpleDelegate ContinueDelegate;
	FSimpleDelegate LoadDelegate;
	FSimpleDelegate SettingsDelegate;
	FSimpleDelegate QuitDelegate;
	FSimpleDelegate HudToggleDelegate;

	TSharedPtr<class SVerticalBox> HudBox;
	TSharedPtr<class SVerticalBox> PanelBox;
	TSharedPtr<class SVerticalBox> MainMenuBox;
	TSharedPtr<class SVerticalBox> YogaBox;
	TSharedPtr<class STextBlock> ToastText;
	TSharedPtr<SListView<TSharedPtr<FApartmentLifeUiListEntry>>> ListView;
	TArray<TSharedPtr<FApartmentLifeUiListEntry>> ListItems;
};

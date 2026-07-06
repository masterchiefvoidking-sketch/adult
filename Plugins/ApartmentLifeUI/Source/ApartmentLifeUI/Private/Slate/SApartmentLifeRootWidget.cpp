// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "Slate/SApartmentLifeRootWidget.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/STableRow.h"

void SApartmentLifeRootWidget::Construct(const FArguments& InArgs)
{
	BackDelegate = InArgs._OnBackPressed;
	NewGameDelegate = InArgs._OnNewGamePressed;
	ContinueDelegate = InArgs._OnContinuePressed;
	LoadDelegate = InArgs._OnLoadPressed;
	SettingsDelegate = InArgs._OnSettingsPressed;
	QuitDelegate = InArgs._OnQuitPressed;
	HudToggleDelegate = InArgs._OnHudTogglePressed;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(12.f)
				[
					SAssignNew(HudBox, SVerticalBox)
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(24.f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(MainMenuBox, SVerticalBox)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.f, 12.f)
					[
						SAssignNew(PanelBox, SVerticalBox)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.f, 12.f)
					[
						SAssignNew(YogaBox, SVerticalBox)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(12.f)
				.HAlign(HAlign_Center)
				[
					SAssignNew(ToastText, STextBlock)
					.Visibility(EVisibility::Collapsed)
				]
			]
		]
	];
}

void SApartmentLifeRootWidget::SetView(
	EApartmentLifeUiScreen Screen,
	bool bHudVisible,
	const FApartmentLifeHudViewModel& Hud,
	const FApartmentLifeUiPanelState& Panel,
	const TArray<FApartmentLifeMainMenuSlotViewModel>& MainMenuSlots,
	const FApartmentLifeYogaUiState& Yoga,
	const FApartmentLifeUiToast& Toast)
{
	CurrentScreen = Screen;

	HudBox->ClearChildren();
	if (bHudVisible && Screen != EApartmentLifeUiScreen::MainMenu)
	{
		HudBox->AddSlot().AutoHeight()
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.02f, 0.04f, 0.08f, 0.82f))
			.Padding(FMargin(12.f, 8.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(
						TEXT("%s | %s | $%.0f | Mood %.0f | Energy %.0f | Hygiene %.0f | Hunger %.0f"),
						*Hud.TimeLabel, *Hud.WeatherLabel, Hud.Savings, Hud.Mood, Hud.Energy, Hud.Hygiene, Hud.Hunger)))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 4.f, 0.f, 0.f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(
						TEXT("Comfort %.0f | Confidence %.0f | Stress %.0f | Affection %.0f | %s"),
						Hud.Comfort, Hud.Confidence, Hud.Stress, Hud.Affection, *Hud.ActivityLabel)))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 4.f, 0.f, 0.f)
				[
					SNew(STextBlock)
					.Visibility(Hud.DeveloperOverlayLabel.IsEmpty() ? EVisibility::Collapsed : EVisibility::HitTestInvisible)
					.Text(FText::FromString(Hud.DeveloperOverlayLabel))
				]
			]
		];
		HudBox->AddSlot().AutoHeight().Padding(0.f, 4.f, 0.f, 0.f)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Toggle HUD (H)")))
			.OnClicked_Lambda([this]()
			{
				HudToggleDelegate.ExecuteIfBound();
				return FReply::Handled();
			})
		];
	}

	MainMenuBox->ClearChildren();
	PanelBox->ClearChildren();
	YogaBox->ClearChildren();

	const bool bShowMainMenu = Screen == EApartmentLifeUiScreen::MainMenu;
	const bool bShowPanel = Screen == EApartmentLifeUiScreen::ContextMenu
		|| Screen == EApartmentLifeUiScreen::Wardrobe
		|| Screen == EApartmentLifeUiScreen::BuildMode
		|| Screen == EApartmentLifeUiScreen::Shopping
		|| Screen == EApartmentLifeUiScreen::Work
		|| Screen == EApartmentLifeUiScreen::WorkResult
		|| Screen == EApartmentLifeUiScreen::Finance
		|| Screen == EApartmentLifeUiScreen::Routines
		|| Screen == EApartmentLifeUiScreen::Profile
		|| Screen == EApartmentLifeUiScreen::SaveLoad
		|| Screen == EApartmentLifeUiScreen::Settings
		|| Screen == EApartmentLifeUiScreen::CharacterCreator
		|| Screen == EApartmentLifeUiScreen::DeveloperHub;

	if (bShowMainMenu)
	{
		MainMenuBox->AddSlot().AutoHeight().Padding(8.f)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Apartment Life"))).Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
		];

		auto MakeMenuButton = [this](const FString& Label, FSimpleDelegate Delegate)
		{
			return SNew(SButton)
				.Text(FText::FromString(Label))
				.OnClicked_Lambda([Delegate]()
				{
					Delegate.ExecuteIfBound();
					return FReply::Handled();
				});
		};

		MainMenuBox->AddSlot().AutoHeight().Padding(4.f)[ MakeMenuButton(TEXT("New Game"), NewGameDelegate) ];
		MainMenuBox->AddSlot().AutoHeight().Padding(4.f)[ MakeMenuButton(TEXT("Continue"), ContinueDelegate) ];
		MainMenuBox->AddSlot().AutoHeight().Padding(4.f)[ MakeMenuButton(TEXT("Load Game"), LoadDelegate) ];
		MainMenuBox->AddSlot().AutoHeight().Padding(4.f)[ MakeMenuButton(TEXT("Settings"), SettingsDelegate) ];
		MainMenuBox->AddSlot().AutoHeight().Padding(4.f)[ MakeMenuButton(TEXT("Quit"), QuitDelegate) ];

		for (const FApartmentLifeMainMenuSlotViewModel& Slot : MainMenuSlots)
		{
			if (!Slot.bExists)
			{
				continue;
			}
			MainMenuBox->AddSlot().AutoHeight().Padding(8.f, 2.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("Slot %d — %s | %s | $%.0f"),
					Slot.SlotIndex + 1, *Slot.CharacterName, *Slot.LastPlayedLabel, Slot.Savings)))
			];
		}
	}

	if (bShowPanel)
	{
		PanelBox->AddSlot().AutoHeight().Padding(8.f)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.03f, 0.05f, 0.1f, 0.92f))
			.Padding(FMargin(16.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock).Text(Panel.Title).Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 4.f, 0.f, 8.f)
				[
					SNew(STextBlock).Text(Panel.Subtitle)
				]
				+ SVerticalBox::Slot().MaxHeight(360.f)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						SAssignNew(ListView, SListView<TSharedPtr<FApartmentLifeUiListEntry>>)
						.ListItemsSource(&ListItems)
						.OnGenerateRow(this, &SApartmentLifeRootWidget::OnGenerateListRow)
						.OnMouseButtonDoubleClick(this, &SApartmentLifeRootWidget::HandleListItemDoubleClicked)
						.SelectionMode(ESelectionMode::Single)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 8.f, 0.f, 0.f)
				[
					SNew(STextBlock).Text(Panel.Footer)
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 8.f, 0.f, 0.f)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Back / Close (Esc)")))
					.OnClicked_Lambda([this]()
					{
						BackDelegate.ExecuteIfBound();
						return FReply::Handled();
					})
				]
			]
		];

		ListItems.Reset();
		for (int32 Index = 0; Index < Panel.Entries.Num(); ++Index)
		{
			FApartmentLifeUiListEntry Entry = Panel.Entries[Index];
			if (Entry.Index == INDEX_NONE)
			{
				Entry.Index = Index;
			}
			Entry.bSelected = Index == Panel.SelectedIndex;
			ListItems.Add(MakeShared<FApartmentLifeUiListEntry>(Entry));
		}
		if (ListView.IsValid())
		{
			ListView->RequestListRefresh();
		}
	}

	if (Screen == EApartmentLifeUiScreen::Yoga)
	{
		YogaBox->AddSlot().AutoHeight().Padding(8.f)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.04f, 0.08f, 0.06f, 0.9f))
			.Padding(FMargin(16.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("Yoga — %s"), *Yoga.PoseName)))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 4.f, 0.f, 0.f)
				[
					SNew(STextBlock).Text(FText::FromString(FString::Printf(
						TEXT("Accuracy %.0f%% | Balance %.0f | Breath %.0f | Streak %d | %.0fs"),
						Yoga.PoseAccuracy, Yoga.Balance, Yoga.BreathingRhythm, Yoga.PoseStreak, Yoga.SessionSeconds)))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.f, 8.f, 0.f, 0.f)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Close")))
					.OnClicked_Lambda([this]()
					{
						BackDelegate.ExecuteIfBound();
						return FReply::Handled();
					})
				]
			]
		];
	}

	if (!Toast.Message.IsEmpty() && Toast.RemainingSeconds > 0.f)
	{
		ToastText->SetText(Toast.Message);
		ToastText->SetVisibility(EVisibility::HitTestInvisible);
	}
	else if (ToastText.IsValid())
	{
		ToastText->SetVisibility(EVisibility::Collapsed);
	}
}

TSharedRef<ITableRow> SApartmentLifeRootWidget::OnGenerateListRow(
	TSharedPtr<FApartmentLifeUiListEntry> Item,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	check(Item.IsValid());
	const FString Label = Item->bSelected
		? FString::Printf(TEXT("> %s"), *Item->Label.ToString())
		: Item->Label.ToString();
	const FString Detail = Item->Detail.IsEmpty() ? FString() : Item->Detail.ToString();
	const int32 ItemIndex = Item->Index;
	const bool bContextMenu = CurrentScreen == EApartmentLifeUiScreen::ContextMenu;

	return SNew(STableRow<TSharedPtr<FApartmentLifeUiListEntry>>, OwnerTable)
		.OnClicked_Lambda([this, ItemIndex, bContextMenu]()
		{
			if (bContextMenu)
			{
				OnListItemConfirmed.ExecuteIfBound(ItemIndex);
			}
			else
			{
				OnListItemClicked.ExecuteIfBound(ItemIndex);
			}
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(Item->bSelected ? FLinearColor(0.1f, 0.2f, 0.35f, 0.9f) : FLinearColor(0.f, 0.f, 0.f, 0.f))
			.Padding(FMargin(6.f, 4.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock).Text(FText::FromString(Label))
				]
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock).Text(FText::FromString(Detail)).ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.8f, 0.9f)))
				]
			]
		];
}

FReply SApartmentLifeRootWidget::HandleListItemDoubleClicked(TSharedPtr<FApartmentLifeUiListEntry> Item)
{
	if (Item.IsValid())
	{
		OnListItemConfirmed.ExecuteIfBound(Item->Index);
	}
	return FReply::Handled();
}

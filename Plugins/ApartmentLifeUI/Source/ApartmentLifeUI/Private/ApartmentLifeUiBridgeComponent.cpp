// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeUiBridgeComponent.h"
#include "ApartmentLifeUiSubsystem.h"
#include "ApartmentLifeUiOverlayGate.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeSaveGame.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeCameraSettingsSubsystem.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeWardrobeUiController.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWorkUiController.h"
#include "ApartmentLifeFinanceUiController.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeInteractionHudComponent.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeInteractionTypes.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeRoutineChainComponent.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeYogaMinigameComponent.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeCharacterCreatorUiController.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorLibrary.h"

namespace
{
	int32 EncodeSaveLoadIndex(int32 SlotIndex, int32 Action)
	{
		return SlotIndex * 10 + Action;
	}

	void DecodeSaveLoadIndex(int32 Encoded, int32& OutSlotIndex, int32& OutAction)
	{
		OutSlotIndex = Encoded / 10;
		OutAction = Encoded % 10;
	}
}

UApartmentLifeUiBridgeComponent::UApartmentLifeUiBridgeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UApartmentLifeUiBridgeComponent::BeginPlay()
{
	Super::BeginPlay();
	BindUiSubsystem();
}

void UApartmentLifeUiBridgeComponent::InitializeContext(
	APlayerController* InController,
	AApartmentLifeApartmentUnit* InApartment,
	AActor* InGirl,
	UApartmentLifeWardrobeUiController* InWardrobeUi,
	UApartmentLifeWorkUiController* InWorkUi,
	UApartmentLifeFinanceUiController* InFinanceUi,
	UApartmentLifeInteractionHudComponent* InInteractionHud,
	UApartmentLifeCharacterCreatorUiController* InCreatorUi,
	int32 InQuickSaveSlot,
	bool bInEnterGameplayDirectly)
{
	OwnerController = InController;
	ApartmentUnit = InApartment;
	GirlCharacter = InGirl;
	WardrobeUi = InWardrobeUi;
	WorkUi = InWorkUi;
	FinanceUi = InFinanceUi;
	InteractionHud = InInteractionHud;
	CreatorUi = InCreatorUi;
	QuickSaveSlot = InQuickSaveSlot;
	bEnterGameplayDirectly = bInEnterGameplayDirectly;

	if (WardrobeUi.IsValid())
	{
		WardrobeUi->OnWardrobeUiStateChanged.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleWardrobeUiStateChanged);
	}

	if (CreatorUi.IsValid())
	{
		CreatorUi->OnCreatorUiStateChanged.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleCreatorUiStateChanged);
	}

	if (UApartmentLifeInteractionSelectionComponent* Selection = GetSelection())
	{
		Selection->OnInteractableSelected.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleInteractableSelected);
		Selection->OnInteractableCleared.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleInteractableCleared);
	}

	if (GirlCharacter.IsValid())
	{
		if (UApartmentLifeYogaMinigameComponent* Yoga = GetGirlYoga())
		{
			Yoga->OnYogaSessionUpdated.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleYogaSessionUpdated);
		}

		if (UApartmentLifeRoutineChainComponent* Routine = GetGirlRoutine())
		{
			Routine->OnRoutineChainStepChanged.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleRoutineStepChanged);
		}
	}

	if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
	{
		BuildMode->OnSelectionChanged.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleBuildSelectionChanged);
	}

	BindUiSubsystem();

	FApartmentLifeUiOverlayGate::bSuppressDebugOverlays = true;

	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->EnsureViewportUi();
		RefreshMainMenuSlots();
		SyncSettingsFromCamera();

		if (bEnterGameplayDirectly)
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::Gameplay);
		}
		else
		{
			Ui->ShowMainMenu();
		}
	}
}

void UApartmentLifeUiBridgeComponent::BindUiSubsystem()
{
	if (bBindingsComplete)
	{
		return;
	}

	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	Ui->OnScreenChanged.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleScreenChanged);
	Ui->OnListItemActivated.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleListItemActivated);
	Ui->OnMainMenuNewGame.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleMainMenuNewGame);
	Ui->OnMainMenuContinue.AddDynamic(this, &UApartmentLifeUiBridgeComponent::HandleMainMenuContinue);
	bBindingsComplete = true;
}

UApartmentLifeUiSubsystem* UApartmentLifeUiBridgeComponent::GetUiSubsystem() const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	UGameInstance* GI = GetWorld()->GetGameInstance();
	return GI ? GI->GetSubsystem<UApartmentLifeUiSubsystem>() : nullptr;
}

UApartmentLifeInteractionSelectionComponent* UApartmentLifeUiBridgeComponent::GetSelection() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeInteractionSelectionComponent>()
		: nullptr;
}

UApartmentLifeNPCSimulationComponent* UApartmentLifeUiBridgeComponent::GetGirlSimulation() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeNPCSimulationComponent>()
		: nullptr;
}

UApartmentLifeProgressionComponent* UApartmentLifeUiBridgeComponent::GetGirlProgression() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeProgressionComponent>()
		: nullptr;
}

UApartmentLifeActivityComponent* UApartmentLifeUiBridgeComponent::GetGirlActivity() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeActivityComponent>()
		: nullptr;
}

UApartmentLifeWardrobeComponent* UApartmentLifeUiBridgeComponent::GetGirlWardrobe() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeWardrobeComponent>()
		: nullptr;
}

UApartmentLifeRoutineChainComponent* UApartmentLifeUiBridgeComponent::GetGirlRoutine() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeRoutineChainComponent>()
		: nullptr;
}

UApartmentLifeYogaMinigameComponent* UApartmentLifeUiBridgeComponent::GetGirlYoga() const
{
	return GirlCharacter.IsValid()
		? GirlCharacter->FindComponentByClass<UApartmentLifeYogaMinigameComponent>()
		: nullptr;
}

UApartmentLifeBuildModeComponent* UApartmentLifeUiBridgeComponent::GetBuildMode() const
{
	return ApartmentUnit.IsValid() ? ApartmentUnit->GetBuildModeComponent() : nullptr;
}

void UApartmentLifeUiBridgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->TickToast(DeltaTime);
	}

	HudRefreshAccumulator += DeltaTime;
	if (HudRefreshAccumulator >= 0.25f)
	{
		HudRefreshAccumulator = 0.f;
		RefreshHud();
		RefreshActiveScreen();
	}
}

void UApartmentLifeUiBridgeComponent::RefreshMainMenuSlots()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!Ui || !GI)
	{
		return;
	}

	UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>();
	if (!SaveSubsystem)
	{
		return;
	}

	TArray<FApartmentLifeMainMenuSlotViewModel> Slots;
	for (int32 SlotIndex = 0; SlotIndex < MaxSaveSlots; ++SlotIndex)
	{
		FApartmentLifeMainMenuSlotViewModel Slot;
		Slot.SlotIndex = SlotIndex;
		Slot.bExists = SaveSubsystem->DoesSaveExist(SlotIndex);

		if (Slot.bExists)
		{
			FApartmentLifeSaveSlotMetadata Metadata;
			int32 SaveVersion = 0;
			if (SaveSubsystem->GetSlotMetadata(SlotIndex, Metadata, SaveVersion))
			{
				Slot.CharacterName = Metadata.CharacterId.IsNone()
					? Metadata.SlotDisplayName
					: Metadata.CharacterId.ToString();
				Slot.LastPlayedLabel = Metadata.LastPlayedTimestamp.ToString();
			}
		}

		if (GirlCharacter.IsValid())
		{
			if (UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation())
			{
				Slot.Savings = Sim->Finance.Savings;
			}
		}

		Slots.Add(Slot);
	}

	Ui->SetMainMenuSlots(Slots);
}

void UApartmentLifeUiBridgeComponent::RefreshHud()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !GirlCharacter.IsValid())
	{
		return;
	}

	UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation();
	if (!Sim)
	{
		return;
	}

	FApartmentLifeHudViewModel Hud;
	if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = GetWorld()->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		Hud.TimeLabel = TimeSubsystem->GetCurrentTime().ToDisplayString();
	}

	Hud.Savings = Sim->Finance.Savings;
	Hud.Mood = Sim->Mood.OverallMood;
	Hud.Energy = Sim->Mood.Energy;
	Hud.Hygiene = Sim->Needs.Hygiene;
	Hud.Hunger = Sim->Needs.Hunger;
	Hud.Comfort = Sim->Mood.Comfort;
	Hud.Confidence = Sim->Mood.Confidence;
	Hud.Stress = Sim->Mood.Stress;
	Hud.Affection = Sim->AffectionTowardPlayer;

	if (UApartmentLifeActivityComponent* Activity = GetGirlActivity())
	{
		Hud.ActivityLabel = Activity->GetCurrentActivityId().ToString();
	}

	Ui->SetHudState(Hud);
}

void UApartmentLifeUiBridgeComponent::RefreshActiveScreen()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	if (WorkUi.IsValid())
	{
		if (WorkUi->IsShowingWorkResult())
		{
			if (Ui->GetActiveScreen() != EApartmentLifeUiScreen::WorkResult)
			{
				Ui->ShowScreen(EApartmentLifeUiScreen::WorkResult);
			}
			PushWorkResultPanel();
			return;
		}

		if (WorkUi->IsWorkMenuOpen() && Ui->GetActiveScreen() != EApartmentLifeUiScreen::Work)
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::Work);
		}
	}

	if (FinanceUi.IsValid())
	{
		if (FinanceUi->IsShoppingOpen() && Ui->GetActiveScreen() != EApartmentLifeUiScreen::Shopping)
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::Shopping);
		}
		else if (FinanceUi->IsBudgetOpen() && Ui->GetActiveScreen() != EApartmentLifeUiScreen::Finance)
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::Finance);
		}
	}

	if (GetBuildMode() && GetBuildMode()->IsBuildModeActive()
		&& Ui->GetActiveScreen() != EApartmentLifeUiScreen::BuildMode)
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::BuildMode);
	}

	switch (Ui->GetActiveScreen())
	{
	case EApartmentLifeUiScreen::Wardrobe: PushWardrobePanel(); break;
	case EApartmentLifeUiScreen::Work: PushWorkPanel(); break;
	case EApartmentLifeUiScreen::WorkResult: PushWorkResultPanel(); break;
	case EApartmentLifeUiScreen::Finance: PushFinancePanel(); break;
	case EApartmentLifeUiScreen::Shopping: PushShoppingPanel(); break;
	case EApartmentLifeUiScreen::ContextMenu: PushContextMenuPanel(); break;
	case EApartmentLifeUiScreen::BuildMode: PushBuildModePanel(); break;
	case EApartmentLifeUiScreen::Routines: PushRoutinesPanel(); break;
	case EApartmentLifeUiScreen::Profile: PushProfilePanel(); break;
	case EApartmentLifeUiScreen::SaveLoad: PushSaveLoadPanel(); break;
	case EApartmentLifeUiScreen::Settings: PushSettingsPanel(); break;
	case EApartmentLifeUiScreen::CharacterCreator: PushCharacterCreatorPanel(); break;
	case EApartmentLifeUiScreen::Yoga: PushYogaPanel(); break;
	default: break;
	}
}

void UApartmentLifeUiBridgeComponent::PushWardrobePanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !WardrobeUi.IsValid())
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Wardrobe"));
	Panel.Subtitle = WardrobeUi->IsShoppingMode()
		? FText::FromString(TEXT("Shopping mode — double-click to purchase"))
		: FText::FromString(TEXT("Double-click to equip | G to close"));

	if (UApartmentLifeNPCSimulationComponent* Sim = GirlCharacter.IsValid() ? GetGirlSimulation() : nullptr)
	{
		Panel.Footer = FText::FromString(FString::Printf(TEXT("Savings: $%.0f"), Sim->Finance.Savings));
	}

	const TArray<FName>& ItemIds = WardrobeUi->GetVisibleItemIds();
	const FName SelectedId = WardrobeUi->GetSelectedItemId();
	for (int32 Index = 0; Index < ItemIds.Num(); ++Index)
	{
		FApartmentLifeBuiltinClothingItem Item;
		if (!UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemIds[Index], Item))
		{
			continue;
		}

		FApartmentLifeUiListEntry Entry;
		Entry.Label = Item.DisplayName;
		Entry.Detail = FText::FromString(FString::Printf(TEXT("$%.0f"), Item.Price));
		Entry.bOwned = GirlCharacter.IsValid()
			&& GetGirlWardrobe()
			&& GetGirlWardrobe()->OwnsClothing(ItemIds[Index]);
		Entry.bSelected = ItemIds[Index] == SelectedId;
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushWorkPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !WorkUi.IsValid())
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Computer Work"));
	Panel.Subtitle = FText::FromString(TEXT("Select a task — double-click or Enter to start"));
	Panel.Footer = FText::FromString(TEXT("W to close"));

	const TArray<FApartmentLifeWorkTypeDefinition>& WorkTypes = WorkUi->GetVisibleWorkTypes();
	for (int32 Index = 0; Index < WorkTypes.Num(); ++Index)
	{
		const FApartmentLifeWorkTypeDefinition& Work = WorkTypes[Index];
		FApartmentLifeUiListEntry Entry;
		Entry.Label = Work.DisplayName;
		Entry.Detail = FText::FromString(FString::Printf(
			TEXT("$%.0f | %d min | Energy %.0f | Stress +%.0f | XP +%.1f | %.0f%% success"),
			Work.BasePayout, Work.DurationMinutes, Work.EnergyCost, Work.StressGain,
			Work.SkillXpGain, Work.SuccessChance * 100.f));
		Entry.bSelected = Index == WorkUi->GetSelectedWorkIndex();
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushWorkResultPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !WorkUi.IsValid())
	{
		return;
	}

	const FApartmentLifeWorkSessionResult& Result = WorkUi->GetLastResult();
	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Work Complete"));
	Panel.Subtitle = FText::FromString(FString::Printf(
		TEXT("Earned $%.0f (bonus $%.0f) | XP +%.1f | Stress %+.0f | %d min"),
		Result.TotalIncome, Result.BonusIncome, Result.SkillXpGained, Result.StressChange, Result.MinutesPassed));

	if (Result.NewUnlocks.Num() > 0)
	{
		Panel.Footer = FText::FromString(TEXT("New unlocks available!"));
	}

	FApartmentLifeUiListEntry Entry;
	Entry.Label = FText::FromString(TEXT("Close"));
	Panel.Entries.Add(Entry);
	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushFinancePanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !FinanceUi.IsValid() || !GirlCharacter.IsValid())
	{
		return;
	}

	UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation();
	UApartmentLifeProgressionComponent* Progression = GetGirlProgression();
	if (!Sim || !Progression)
	{
		return;
	}

	const FApartmentLifeDailyBudget& Budget = UApartmentLifeFinanceLibrary::GetCurrentBudget(Progression);
	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Budget"));
	Panel.Subtitle = FText::FromString(FString::Printf(
		TEXT("Savings $%.0f | Today +$%.0f | Week +$%.0f"),
		Sim->Finance.Savings, Budget.DailyIncome, Budget.WeeklyIncome));
	Panel.Footer = FText::FromString(FString::Printf(
		TEXT("Clothing $%.0f | Furniture $%.0f | Food $%.0f | Rent/day $%.0f"),
		Budget.ClothingSpent, Budget.FurnitureSpent, Budget.FoodSpent, Sim->Finance.MonthlyRent / 30.f));

	auto AddShopNav = [&](const FString& Label, EApartmentLifeShopCategory Category)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Label);
		Entry.Detail = FText::FromString(TEXT("Open shop"));
		Panel.Entries.Add(Entry);
	};

	AddShopNav(TEXT("Furniture Shop"), EApartmentLifeShopCategory::Furniture);
	AddShopNav(TEXT("Decor Shop"), EApartmentLifeShopCategory::Decorations);
	AddShopNav(TEXT("Grocery / Supplies"), EApartmentLifeShopCategory::Groceries);
	AddShopNav(TEXT("Electronics / Upgrades"), EApartmentLifeShopCategory::Electronics);
	AddShopNav(TEXT("Fitness / Yoga"), EApartmentLifeShopCategory::Fitness);

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushShoppingPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !FinanceUi.IsValid())
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Shop"));
	Panel.Subtitle = FText::FromString(TEXT("Double-click to purchase"));
	Panel.Footer = FText::FromString(TEXT("Esc to close"));

	const TArray<FApartmentLifeBuiltinShopItem>& Items = FinanceUi->GetVisibleShopItems();
	UApartmentLifeProgressionComponent* Progression = GirlCharacter.IsValid() ? GetGirlProgression() : nullptr;

	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		const FApartmentLifeBuiltinShopItem& Item = Items[Index];
		const bool bOwned = Progression && Progression->OwnsShopItem(Item.ItemId);
		FApartmentLifeUiListEntry Entry;
		Entry.Label = Item.DisplayName;
		Entry.Detail = FText::FromString(FString::Printf(TEXT("$%.0f %s"), Item.Price, bOwned ? TEXT("[owned]") : TEXT("")));
		Entry.bOwned = bOwned;
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushContextMenuPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	UApartmentLifeInteractionSelectionComponent* Selection = GetSelection();
	if (!Selection || !Selection->GetSelectedInteractable())
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(Selection->GetSelectedInteractable()->GetName());
	Panel.Subtitle = FText::FromString(TEXT("Click an action"));
	Panel.Footer = FText::FromString(TEXT("Esc to close"));

	const TArray<FApartmentLifeInteractionDescriptor>& Actions = Selection->GetAvailableActions();
	for (const FApartmentLifeInteractionDescriptor& Action : Actions)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = Action.DisplayName;
		Entry.Detail = FText::FromString(FString::Printf(TEXT("%s (%d min)"),
			*Action.StatEffectSummary.ToString(), Action.DurationMinutes));
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushBuildModePanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode();
	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Build Mode"));
	Panel.Subtitle = FText::FromString(TEXT("Place, move, and decorate your apartment"));
	Panel.Footer = FText::FromString(TEXT("B to exit | Ctrl+Z undo | Ctrl+Y redo"));

	const TArray<FString> Actions = {
		TEXT("Undo"),
		TEXT("Redo"),
		TEXT("Store Selected"),
		TEXT("Sell Selected"),
		TEXT("Recolor Selected"),
		TEXT("Exit Build Mode")
	};

	for (const FString& Action : Actions)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Action);
		if (BuildMode && Action == TEXT("Store Selected"))
		{
			Entry.Detail = FText::FromString(BuildMode->GetSelectedInstanceId().IsValid()
				? TEXT("Item selected") : TEXT("No selection"));
		}
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushRoutinesPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Routines"));
	Panel.Subtitle = FText::FromString(TEXT("Start a routine chain"));
	Panel.Footer = FText::FromString(TEXT("Esc to close"));

	ActiveRoutineOffset = 0;
	if (GirlCharacter.IsValid())
	{
		if (UApartmentLifeRoutineChainComponent* Routine = GetGirlRoutine())
		{
			if (Routine->IsRoutineActive())
			{
				Panel.Subtitle = FText::FromString(FString::Printf(
					TEXT("Active: %s — step %d"),
					*Routine->GetCurrentChainId().ToString(),
					Routine->GetCurrentStepIndex() + 1));

				FApartmentLifeUiListEntry Advance;
				Advance.Label = FText::FromString(TEXT("Advance Step"));
				Panel.Entries.Add(Advance);

				FApartmentLifeUiListEntry Cancel;
				Cancel.Label = FText::FromString(TEXT("Stop Routine"));
				Panel.Entries.Add(Cancel);
				ActiveRoutineOffset = 2;
			}
		}
	}

	for (const FApartmentLifeRoutineChainDefinition& Chain : UApartmentLifeActivityCatalogLibrary::GetBuiltinRoutineChains())
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = Chain.DisplayName;
		Entry.Detail = FText::FromString(FString::Printf(TEXT("%d steps"), Chain.ActivityIds.Num()));
		Panel.Entries.Add(Entry);
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushProfilePanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !GirlCharacter.IsValid())
	{
		return;
	}

	UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation();
	if (!Sim)
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Profile"));
	Panel.Subtitle = FText::FromString(Sim->CharacterId.ToString());

	const TArray<TPair<FString, FString>> Stats = {
		{TEXT("Mood"), FString::Printf(TEXT("%.0f"), Sim->Mood.OverallMood)},
		{TEXT("Energy"), FString::Printf(TEXT("%.0f"), Sim->Mood.Energy)},
		{TEXT("Hygiene"), FString::Printf(TEXT("%.0f"), Sim->Needs.Hygiene)},
		{TEXT("Hunger"), FString::Printf(TEXT("%.0f"), Sim->Needs.Hunger)},
		{TEXT("Comfort"), FString::Printf(TEXT("%.0f"), Sim->Mood.Comfort)},
		{TEXT("Confidence"), FString::Printf(TEXT("%.0f"), Sim->Mood.Confidence)},
		{TEXT("Stress"), FString::Printf(TEXT("%.0f"), Sim->Mood.Stress)},
		{TEXT("Affection"), FString::Printf(TEXT("%.0f"), Sim->AffectionTowardPlayer)},
		{TEXT("Savings"), FString::Printf(TEXT("$%.0f"), Sim->Finance.Savings)},
		{TEXT("Apartment Comfort"), FString::Printf(TEXT("%.0f"), Sim->Apartment.ComfortScore)}
	};

	for (const TPair<FString, FString>& Stat : Stats)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Stat.Key);
		Entry.Detail = FText::FromString(Stat.Value);
		Panel.Entries.Add(Entry);
	}

	if (UApartmentLifeRoutineChainComponent* Routine = GetGirlRoutine())
	{
		if (Routine->IsRoutineActive())
		{
			FApartmentLifeUiListEntry Entry;
			Entry.Label = FText::FromString(TEXT("Current Routine"));
			Entry.Detail = FText::FromString(Routine->GetCurrentChainId().ToString());
			Panel.Entries.Add(Entry);
		}
	}

	FApartmentLifeUiListEntry EditCharacter;
	EditCharacter.Index = 900;
	EditCharacter.Label = FText::FromString(TEXT("Edit Character"));
	EditCharacter.Detail = FText::FromString(TEXT("Open character creator"));
	Panel.Entries.Add(EditCharacter);

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushSaveLoadPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!Ui || !GI)
	{
		return;
	}

	UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>();
	if (!SaveSubsystem)
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Save / Load"));
	Panel.Subtitle = FText::FromString(FString::Printf(TEXT("Save version %d"), UApartmentLifeSaveGame::CurrentSaveVersion));
	Panel.Footer = FText::FromString(TEXT("Click Save, Load, or Delete for each slot"));

	for (int32 SlotIndex = 0; SlotIndex < MaxSaveSlots; ++SlotIndex)
	{
		FApartmentLifeSaveSlotMetadata Metadata;
		int32 SaveVersion = 0;
		const bool bExists = SaveSubsystem->GetSlotMetadata(SlotIndex, Metadata, SaveVersion);

		FApartmentLifeUiListEntry SaveEntry;
		SaveEntry.Index = EncodeSaveLoadIndex(SlotIndex, 0);
		SaveEntry.Label = FText::FromString(FString::Printf(TEXT("Slot %d — Save"), SlotIndex + 1));
		SaveEntry.Detail = bExists
			? FText::FromString(FString::Printf(TEXT("Overwrite %s"), *Metadata.LastPlayedTimestamp.ToString()))
			: FText::FromString(TEXT("Empty slot"));
		Panel.Entries.Add(SaveEntry);

		FApartmentLifeUiListEntry LoadEntry;
		LoadEntry.Index = EncodeSaveLoadIndex(SlotIndex, 1);
		LoadEntry.Label = FText::FromString(FString::Printf(TEXT("Slot %d — Load"), SlotIndex + 1));
		LoadEntry.Detail = bExists
			? FText::FromString(FString::Printf(TEXT("v%d | %s"), SaveVersion, *Metadata.CharacterId.ToString()))
			: FText::FromString(TEXT("No save"));
		LoadEntry.bOwned = bExists;
		Panel.Entries.Add(LoadEntry);

		if (bExists)
		{
			FApartmentLifeUiListEntry DeleteEntry;
			DeleteEntry.Index = EncodeSaveLoadIndex(SlotIndex, 2);
			DeleteEntry.Label = FText::FromString(FString::Printf(TEXT("Slot %d — Delete"), SlotIndex + 1));
			DeleteEntry.Detail = FText::FromString(TEXT("Requires confirmation"));
			Panel.Entries.Add(DeleteEntry);
		}
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushSettingsPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	const FApartmentLifeUiSettingsState& Settings = Ui->GetSettings();
	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Settings"));
	Panel.Subtitle = FText::FromString(TEXT("Click to toggle or adjust"));
	Panel.Footer = FText::FromString(TEXT("Changes apply immediately"));

	auto AddToggle = [&](const FString& Label, bool bValue)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Label);
		Entry.Detail = FText::FromString(bValue ? TEXT("On") : TEXT("Off"));
		Panel.Entries.Add(Entry);
	};

	auto AddValue = [&](const FString& Label, float Value)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Label);
		Entry.Detail = FText::FromString(FString::Printf(TEXT("%.2f"), Value));
		Panel.Entries.Add(Entry);
	};

	AddValue(TEXT("Camera Sensitivity"), Settings.CameraSensitivity);
	AddToggle(TEXT("Invert Mouse Y"), Settings.bInvertMouseY);
	AddValue(TEXT("Photo FOV"), Settings.PhotoFov);
	AddValue(TEXT("Master Volume"), Settings.MasterVolume);
	AddValue(TEXT("Music Volume"), Settings.MusicVolume);
	AddValue(TEXT("UI Volume"), Settings.UiVolume);
	AddToggle(TEXT("Autosave"), Settings.bAutosaveEnabled);
	AddToggle(TEXT("HUD Visible"), Settings.bHudVisible);

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushCharacterCreatorPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !CreatorUi.IsValid())
	{
		return;
	}

	FApartmentLifeUiPanelState Panel;
	Panel.Title = FText::FromString(TEXT("Character Creator"));
	Panel.Subtitle = FText::FromString(TEXT("Customize appearance — double-click to apply"));
	Panel.Footer = FText::FromString(TEXT("[ / ] adjust slider | C to close | Confirm when done"));

	const UEnum* TabEnum = StaticEnum<EApartmentLifeCreatorCategoryTab>();
	if (TabEnum)
	{
		for (int32 TabIndex = 0; TabIndex < TabEnum->NumEnums() - 1; ++TabIndex)
		{
			FApartmentLifeUiListEntry TabEntry;
			TabEntry.Index = TabIndex;
			TabEntry.Label = TabEnum->GetDisplayNameTextByIndex(TabIndex);
			TabEntry.bSelected = static_cast<int32>(CreatorUi->GetCategoryTab()) == TabIndex;
			Panel.Entries.Add(TabEntry);
		}
	}

	const TArray<FText>& Labels = CreatorUi->GetVisibleEntryLabels();
	const TArray<FText>& Details = CreatorUi->GetVisibleEntryDetails();
	for (int32 Index = 0; Index < Labels.Num(); ++Index)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Index = 100 + Index;
		Entry.Label = Labels[Index];
		Entry.Detail = Details.IsValidIndex(Index) ? Details[Index] : FText::GetEmpty();
		Entry.bSelected = Index == CreatorUi->GetSelectedEntryIndex();
		Panel.Entries.Add(Entry);
	}

	auto AddAction = [&](int32 EncodedIndex, const FString& Label, const FString& Detail)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Index = EncodedIndex;
		Entry.Label = FText::FromString(Label);
		Entry.Detail = FText::FromString(Detail);
		Panel.Entries.Add(Entry);
	};

	AddAction(200, TEXT("Randomize Full"), TEXT("Believable full randomize"));
	AddAction(201, TEXT("Randomize Face"), TEXT("Face only"));
	AddAction(202, TEXT("Randomize Body"), TEXT("Body only"));
	AddAction(203, TEXT("Reset Category"), TEXT("Reset current tab"));
	AddAction(204, TEXT("Confirm Character"), TEXT("Save and close"));
	AddAction(219, TEXT("Save Preset"), TEXT("Save current look to selected preset slot"));

	const UEnum* LightingEnum = StaticEnum<EApartmentLifeCreatorLightingMode>();
	if (LightingEnum)
	{
		for (int32 LightingIndex = 0; LightingIndex < LightingEnum->NumEnums() - 1; ++LightingIndex)
		{
			AddAction(205 + LightingIndex, LightingEnum->GetDisplayNameTextByIndex(LightingIndex).ToString(), TEXT("Lighting preview"));
		}
	}

	AddAction(215, TEXT("Focus Face"), TEXT("Camera focus"));
	AddAction(216, TEXT("Focus Upper Body"), TEXT("Camera focus"));
	AddAction(217, TEXT("Focus Full Body"), TEXT("Camera focus"));
	AddAction(218, TEXT("Focus Outfit"), TEXT("Camera focus"));

	const FApartmentLifeCreatorCompatibilityReport Report = CreatorUi->GetCompatibilityReport();
	if (Report.bHasClippingWarnings)
	{
		Panel.Footer = FText::FromString(FString::Printf(TEXT("Warning: %d outfit fit issues"), Report.Warnings.Num()));
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::PushYogaPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui || !GirlCharacter.IsValid())
	{
		return;
	}

	UApartmentLifeYogaMinigameComponent* Yoga = GetGirlYoga();
	if (!Yoga)
	{
		return;
	}

	const FApartmentLifeYogaSessionState& State = Yoga->GetSessionState();
	FApartmentLifeYogaUiState YogaUi;
	YogaUi.PoseName = State.CurrentPoseId.ToString();
	YogaUi.PoseAccuracy = State.PoseMatchScore;
	YogaUi.Balance = State.BalanceMeter;
	YogaUi.BreathingRhythm = State.BreathingRhythm;
	YogaUi.PoseStreak = State.PoseStreak;
	YogaUi.SessionSeconds = State.SessionDurationMinutes * 60.f;
	Ui->SetYogaState(YogaUi);
}

void UApartmentLifeUiBridgeComponent::SyncSettingsFromCamera()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!Ui || !GI)
	{
		return;
	}

	if (UApartmentLifeCameraSettingsSubsystem* CameraSettings = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
	{
		const FApartmentLifeCameraUserSettings& Cam = CameraSettings->GetSettings();
		FApartmentLifeUiSettingsState& Settings = Ui->GetMutableSettings();
		Settings.CameraSensitivity = Cam.OrbitSensitivity;
		Settings.bInvertMouseY = Cam.bInvertYAxis;
		Settings.PhotoFov = Cam.PhotoSettings.FieldOfView;
		Settings.bHudVisible = Ui->IsHudVisible();
	}

	if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
	{
		Ui->GetMutableSettings().bAutosaveEnabled = SaveSubsystem->IsAutosaveEnabled();
	}
}

void UApartmentLifeUiBridgeComponent::ApplySettingsToCamera()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!Ui || !GI)
	{
		return;
	}

	const FApartmentLifeUiSettingsState& Settings = Ui->GetSettings();

	if (UApartmentLifeCameraSettingsSubsystem* CameraSettings = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
	{
		FApartmentLifeCameraUserSettings Cam = CameraSettings->GetSettings();
		Cam.OrbitSensitivity = Settings.CameraSensitivity;
		Cam.bInvertYAxis = Settings.bInvertMouseY;
		Cam.PhotoSettings.FieldOfView = Settings.PhotoFov;
		CameraSettings->SetSettings(Cam);

		if (OwnerController.IsValid())
		{
			if (AApartmentLifeCameraPawn* CameraPawn = Cast<AApartmentLifeCameraPawn>(OwnerController->GetPawn()))
			{
				CameraSettings->ApplySettingsToPawn(CameraPawn);
			}
		}
	}

	if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
	{
		SaveSubsystem->SetAutosaveEnabled(Settings.bAutosaveEnabled);
	}
}

void UApartmentLifeUiBridgeComponent::OpenSaveLoadScreen()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::SaveLoad);
		PushSaveLoadPanel();
	}
}

void UApartmentLifeUiBridgeComponent::OpenProfileScreen()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::Profile);
		PushProfilePanel();
	}
}

void UApartmentLifeUiBridgeComponent::OpenCharacterCreatorScreen()
{
	if (CreatorUi.IsValid())
	{
		CreatorUi->OpenCreator(true);
	}
}

void UApartmentLifeUiBridgeComponent::OpenRoutinesScreen()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::Routines);
		PushRoutinesPanel();
	}
}

void UApartmentLifeUiBridgeComponent::ToggleHud()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ToggleHud();
	}
}

void UApartmentLifeUiBridgeComponent::HandleUiBack()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	if (!Ui)
	{
		return;
	}

	const EApartmentLifeUiScreen Screen = Ui->GetActiveScreen();
	if (Screen == EApartmentLifeUiScreen::WorkResult)
	{
		if (WorkUi.IsValid())
		{
			WorkUi->CloseWorkSelection();
		}
		Ui->CloseScreen();
		return;
	}

	if (Screen == EApartmentLifeUiScreen::Wardrobe && WardrobeUi.IsValid())
	{
		WardrobeUi->CloseWardrobe();
	}

	if (Screen == EApartmentLifeUiScreen::CharacterCreator && CreatorUi.IsValid())
	{
		CreatorUi->CloseCreator(false);
	}

	if (Screen == EApartmentLifeUiScreen::Work && WorkUi.IsValid())
	{
		WorkUi->CloseWorkSelection();
	}

	if (Screen == EApartmentLifeUiScreen::Finance || Screen == EApartmentLifeUiScreen::Shopping)
	{
		if (FinanceUi.IsValid())
		{
			FinanceUi->CloseAllOverlays();
		}
	}

	if (Screen == EApartmentLifeUiScreen::BuildMode)
	{
		if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
		{
			BuildMode->ExitBuildMode();
		}
	}

	if (Screen == EApartmentLifeUiScreen::Yoga && GirlCharacter.IsValid())
	{
		if (UApartmentLifeYogaMinigameComponent* Yoga = GetGirlYoga())
		{
			Yoga->EndYogaSession();
		}
	}

	Ui->HandleBack();
}

void UApartmentLifeUiBridgeComponent::HandleScreenChanged(EApartmentLifeUiScreen NewScreen)
{
	RefreshActiveScreen();
}

void UApartmentLifeUiBridgeComponent::HandleListItemActivated(EApartmentLifeUiScreen Screen, int32 Index)
{
	switch (Screen)
	{
	case EApartmentLifeUiScreen::ContextMenu:
		if (UApartmentLifeInteractionSelectionComponent* Selection = GetSelection())
		{
			Selection->ExecuteInteractionByIndex(Index);
		}
		break;

	case EApartmentLifeUiScreen::Wardrobe:
		if (WardrobeUi.IsValid())
		{
			WardrobeUi->SelectItemIndex(Index);
			if (WardrobeUi->IsShoppingMode())
			{
				if (WardrobeUi->PurchaseSelectedItem())
				{
					if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
					{
						Ui->ShowToast(FText::FromString(TEXT("Purchased!")), 2.f);
					}
				}
			}
			else
			{
				WardrobeUi->EquipSelectedItem();
			}
			PushWardrobePanel();
		}
		break;

	case EApartmentLifeUiScreen::Work:
		if (WorkUi.IsValid())
		{
			WorkUi->SelectWorkIndex(Index);
			WorkUi->StartSelectedWork();
		}
		break;

	case EApartmentLifeUiScreen::WorkResult:
		if (WorkUi.IsValid())
		{
			WorkUi->CloseWorkSelection();
		}
		if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
		{
			Ui->CloseScreen();
		}
		break;

	case EApartmentLifeUiScreen::Shopping:
		if (FinanceUi.IsValid())
		{
			if (FinanceUi->PurchaseShopItemIndex(Index))
			{
				if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
				{
					Ui->ShowToast(FText::FromString(TEXT("Purchased!")), 2.f);
				}
			}
			PushShoppingPanel();
		}
		break;

	case EApartmentLifeUiScreen::Finance:
		{
			const EApartmentLifeShopCategory Categories[] = {
				EApartmentLifeShopCategory::Furniture,
				EApartmentLifeShopCategory::Decorations,
				EApartmentLifeShopCategory::Groceries,
				EApartmentLifeShopCategory::Electronics,
				EApartmentLifeShopCategory::Fitness
			};
			if (Index >= 0 && Index < UE_ARRAY_COUNT(Categories))
			{
				ActiveShopCategory = Categories[Index];
				if (FinanceUi.IsValid())
				{
					FinanceUi->OpenShoppingOverlay(ActiveShopCategory);
				}
				if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
				{
					Ui->ShowScreen(EApartmentLifeUiScreen::Shopping);
					PushShoppingPanel();
				}
			}
		}
		break;

	case EApartmentLifeUiScreen::BuildMode:
		if (UApartmentLifeBuildModeComponent* BuildMode = GetBuildMode())
		{
			switch (Index)
			{
			case 0: BuildMode->Undo(); break;
			case 1: BuildMode->Redo(); break;
			case 2: BuildMode->StoreSelected(); break;
			case 3: BuildMode->SellSelected(); break;
			case 4: BuildMode->RecolorSelected(FLinearColor::MakeRandomColor()); break;
			case 5:
				BuildMode->ExitBuildMode();
				if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem()) { Ui->CloseScreen(); }
				break;
			default: break;
			}
			PushBuildModePanel();
		}
		break;

	case EApartmentLifeUiScreen::Routines:
		if (GirlCharacter.IsValid())
		{
			if (UApartmentLifeRoutineChainComponent* Routine = GetGirlRoutine())
			{
				if (Routine->IsRoutineActive())
				{
					if (Index == 0) { Routine->AdvanceRoutineChain(); }
					else if (Index == 1) { Routine->CancelRoutineChain(); }
				}
				else
				{
					const TArray<FApartmentLifeRoutineChainDefinition> Chains =
						UApartmentLifeActivityCatalogLibrary::GetBuiltinRoutineChains();
					const int32 ChainIndex = Index - ActiveRoutineOffset;
					if (Chains.IsValidIndex(ChainIndex))
					{
						Routine->StartRoutineChain(Chains[ChainIndex].ChainId);
					}
				}
			}
			PushRoutinesPanel();
		}
		break;

	case EApartmentLifeUiScreen::SaveLoad:
		{
			int32 SlotIndex = 0;
			int32 Action = 0;
			DecodeSaveLoadIndex(Index, SlotIndex, Action);
			UGameInstance* GI = GetWorld()->GetGameInstance();
			UApartmentLifeSaveSubsystem* SaveSubsystem = GI ? GI->GetSubsystem<UApartmentLifeSaveSubsystem>() : nullptr;
			if (!SaveSubsystem || SlotIndex >= MaxSaveSlots)
			{
				break;
			}

			if (Action == 0)
			{
				SaveSubsystem->SaveToSlot(SlotIndex);
				if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
				{
					Ui->ShowToast(FText::FromString(TEXT("Game saved")), 2.f);
				}
				RefreshMainMenuSlots();
				PushSaveLoadPanel();
			}
			else if (Action == 1)
			{
				if (SaveSubsystem->DoesSaveExist(SlotIndex))
				{
					SaveSubsystem->LoadFromSlot(SlotIndex);
					OnPostLoadRequested.Broadcast();
					if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
					{
						Ui->ShowToast(FText::FromString(TEXT("Game loaded")), 2.f);
						Ui->CloseScreen();
					}
				}
			}
			else if (Action == 2)
			{
				SaveSubsystem->DeleteSaveSlot(SlotIndex);
				if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
				{
					Ui->ShowToast(FText::FromString(TEXT("Save deleted")), 2.f);
				}
				RefreshMainMenuSlots();
				PushSaveLoadPanel();
			}
		}
		break;

	case EApartmentLifeUiScreen::Settings:
		if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
		{
			FApartmentLifeUiSettingsState& Settings = Ui->GetMutableSettings();
			switch (Index)
			{
			case 0: Settings.CameraSensitivity = FMath::Clamp(Settings.CameraSensitivity + 0.1f, 0.2f, 3.f); break;
			case 1: Settings.bInvertMouseY = !Settings.bInvertMouseY; break;
			case 2: Settings.PhotoFov = FMath::Clamp(Settings.PhotoFov + 2.f, 30.f, 90.f); break;
			case 3: Settings.MasterVolume = FMath::Clamp(Settings.MasterVolume + 0.1f, 0.f, 1.f); break;
			case 4: Settings.MusicVolume = FMath::Clamp(Settings.MusicVolume + 0.1f, 0.f, 1.f); break;
			case 5: Settings.UiVolume = FMath::Clamp(Settings.UiVolume + 0.1f, 0.f, 1.f); break;
			case 6: Settings.bAutosaveEnabled = !Settings.bAutosaveEnabled; break;
			case 7: Settings.bHudVisible = !Settings.bHudVisible; break;
			default: break;
			}
			ApplySettingsToCamera();
			PushSettingsPanel();
		}
		break;

	case EApartmentLifeUiScreen::CharacterCreator:
		if (CreatorUi.IsValid())
		{
			if (Index < 9)
			{
				CreatorUi->SetCategoryTab(static_cast<EApartmentLifeCreatorCategoryTab>(Index));
			}
			else if (Index >= 100 && Index < 200)
			{
				CreatorUi->SelectEntryIndex(Index - 100);
				CreatorUi->ActivateSelectedEntry();
			}
			else if (Index == 200) CreatorUi->Randomize(EApartmentLifeCreatorRandomizeScope::FullCharacter);
			else if (Index == 201) CreatorUi->Randomize(EApartmentLifeCreatorRandomizeScope::FaceOnly);
			else if (Index == 202) CreatorUi->Randomize(EApartmentLifeCreatorRandomizeScope::BodyOnly);
			else if (Index == 203) CreatorUi->ResetCurrentCategory();
			else if (Index == 204) CreatorUi->ConfirmCharacter();
			else if (Index >= 205 && Index < 215)
			{
				CreatorUi->SetLightingMode(static_cast<EApartmentLifeCreatorLightingMode>(Index - 205));
			}
			else if (Index == 215) CreatorUi->SetCameraFocus(FName(TEXT("Face")));
			else if (Index == 216) CreatorUi->SetCameraFocus(FName(TEXT("UpperBody")));
			else if (Index == 217) CreatorUi->SetCameraFocus(FName(TEXT("FullBody")));
			else if (Index == 218) CreatorUi->SetCameraFocus(FName(TEXT("Outfit")));
			else if (Index == 219) CreatorUi->SavePresetByIndex(CreatorUi->GetSelectedEntryIndex());
			PushCharacterCreatorPanel();
		}
		break;

	case EApartmentLifeUiScreen::Profile:
		if (Index == 900)
		{
			OpenCharacterCreatorScreen();
		}
		break;

	default:
		break;
	}
}

void UApartmentLifeUiBridgeComponent::HandleMainMenuNewGame()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->CloseScreen();
		Ui->ShowToast(FText::FromString(TEXT("New game started")), 2.f);
	}
}

void UApartmentLifeUiBridgeComponent::HandleMainMenuContinue(int32 SlotIndex)
{
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (!GI)
	{
		return;
	}

	UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>();
	if (SaveSubsystem && SaveSubsystem->DoesSaveExist(SlotIndex))
	{
		SaveSubsystem->LoadFromSlot(SlotIndex);
		OnPostLoadRequested.Broadcast();
	}

	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->CloseScreen();
	}
}

void UApartmentLifeUiBridgeComponent::HandleWardrobeUiStateChanged()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (WardrobeUi.IsValid() && WardrobeUi->IsWardrobeOpen())
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::Wardrobe);
			PushWardrobePanel();
		}
		else if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::Wardrobe)
		{
			Ui->CloseScreen();
		}
	}
}

void UApartmentLifeUiBridgeComponent::HandleCreatorUiStateChanged()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (CreatorUi.IsValid() && CreatorUi->IsCreatorOpen())
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::CharacterCreator);
			PushCharacterCreatorPanel();
		}
		else if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::CharacterCreator)
		{
			Ui->CloseScreen();
		}
	}
}

void UApartmentLifeUiBridgeComponent::HandleInteractableSelected(
	AActor* Target,
	const TArray<FApartmentLifeInteractionDescriptor>& Actions)
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::ContextMenu);
		PushContextMenuPanel();
	}
}

void UApartmentLifeUiBridgeComponent::HandleInteractableCleared()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::ContextMenu)
		{
			Ui->CloseScreen();
		}
	}
}

void UApartmentLifeUiBridgeComponent::HandleYogaSessionUpdated(const FApartmentLifeYogaSessionState& State)
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		Ui->ShowScreen(EApartmentLifeUiScreen::Yoga);
		PushYogaPanel();
	}
}

void UApartmentLifeUiBridgeComponent::HandleRoutineStepChanged(int32 StepIndex)
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::Routines)
		{
			PushRoutinesPanel();
		}
	}
}

void UApartmentLifeUiBridgeComponent::HandleBuildSelectionChanged(FGuid InstanceId)
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::BuildMode)
		{
			PushBuildModePanel();
		}
	}
}

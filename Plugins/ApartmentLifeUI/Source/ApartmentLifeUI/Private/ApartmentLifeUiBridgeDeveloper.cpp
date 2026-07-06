// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeUiBridgeComponent.h"
#include "ApartmentLifeUiSubsystem.h"
#include "ApartmentLifeDeveloperUiController.h"
#include "ApartmentLifeDeveloperSubsystem.h"
#include "ApartmentLifeDeveloperLibrary.h"
#include "ApartmentLifeDeveloperTypes.h"
#include "ApartmentLifeImmersionLibrary.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeDevToolsLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "Engine/GameInstance.h"

namespace
{
	void AddPanelAction(FApartmentLifeUiPanelState& Panel, const FString& Label, const FString& Detail, int32 Index)
	{
		FApartmentLifeUiListEntry Entry;
		Entry.Label = FText::FromString(Label);
		Entry.Detail = FText::FromString(Detail);
		Entry.Index = Index;
		Panel.Entries.Add(Entry);
	}
}

void UApartmentLifeUiBridgeComponent::PushDeveloperHubPanel()
{
	UApartmentLifeUiSubsystem* Ui = GetUiSubsystem();
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!Ui || !GI || !DeveloperUi.IsValid())
	{
		return;
	}

	UApartmentLifeDeveloperSubsystem* DevSubsystem = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>();
	if (!DevSubsystem)
	{
		return;
	}

	const EApartmentLifeDeveloperHubSection Section = DeveloperUi->GetActiveSection();
	FApartmentLifeUiPanelState Panel;
	Panel.Title = UApartmentLifeDeveloperLibrary::GetHubSectionLabel(Section);
	Panel.Subtitle = FText::FromString(TEXT("Double-click to activate | Esc to close | ` toggles perf overlay"));

	switch (Section)
	{
	case EApartmentLifeDeveloperHubSection::Root:
		Panel.Subtitle = FText::FromString(TEXT("Internal developer toolkit — double-click a section"));
		for (int32 Index = static_cast<int32>(EApartmentLifeDeveloperHubSection::Character);
			Index <= static_cast<int32>(EApartmentLifeDeveloperHubSection::Dashboard); ++Index)
		{
			const EApartmentLifeDeveloperHubSection HubSection = static_cast<EApartmentLifeDeveloperHubSection>(Index);
			AddPanelAction(Panel,
				UApartmentLifeDeveloperLibrary::GetHubSectionLabel(HubSection).ToString(),
				TEXT("Open section"), Index);
		}
		break;

	case EApartmentLifeDeveloperHubSection::Character:
		if (UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation())
		{
			Panel.Footer = FText::FromString(FString::Printf(
				TEXT("Mood %.0f | Energy %.0f | Hygiene %.0f | Stress %.0f | Savings $%.0f"),
				Sim->Mood.OverallMood, Sim->Mood.Energy, Sim->Needs.Hygiene, Sim->Mood.Stress, Sim->Finance.Savings));
		}
		AddPanelAction(Panel, TEXT("Mood +10"), TEXT("Live edit"), 1000);
		AddPanelAction(Panel, TEXT("Mood -10"), TEXT("Live edit"), 1001);
		AddPanelAction(Panel, TEXT("Energy +10"), TEXT("Live edit"), 1010);
		AddPanelAction(Panel, TEXT("Hygiene +10"), TEXT("Live edit"), 1020);
		AddPanelAction(Panel, TEXT("Stress +10"), TEXT("Live edit"), 1030);
		AddPanelAction(Panel, TEXT("Savings +$500"), TEXT("Live edit"), 1040);
		AddPanelAction(Panel, TEXT("Confidence +10"), TEXT("Live edit"), 1050);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Camera:
		AddPanelAction(Panel, TEXT("FOV +2"), TEXT("Photo/camera"), 2000);
		AddPanelAction(Panel, TEXT("FOV -2"), TEXT("Photo/camera"), 2001);
		AddPanelAction(Panel, TEXT("Orbit speed +0.1"), TEXT("Camera"), 2010);
		AddPanelAction(Panel, TEXT("Zoom speed +0.1"), TEXT("Camera"), 2020);
		AddPanelAction(Panel, TEXT("Pan speed +0.1"), TEXT("Camera"), 2030);
		AddPanelAction(Panel, TEXT("Enter photo mode"), TEXT("Screenshot studio"), 2040);
		AddPanelAction(Panel, TEXT("Take screenshot"), TEXT("High-res capture"), 2041);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Lighting:
		for (int32 PresetIndex = 0; PresetIndex <= static_cast<int32>(EApartmentLifeDeveloperLightingLabPreset::Studio); ++PresetIndex)
		{
			const UEnum* Enum = StaticEnum<EApartmentLifeDeveloperLightingLabPreset>();
			AddPanelAction(Panel,
				Enum ? Enum->GetDisplayNameTextByValue(PresetIndex).ToString() : FString::Printf(TEXT("Preset %d"), PresetIndex),
				TEXT("Apply lighting lab preset"),
				3000 + PresetIndex);
		}
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Apartment:
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UApartmentLifeGameTimeSubsystem>() : nullptr)
		{
			Panel.Footer = FText::FromString(FString::Printf(TEXT("%s | %s"),
				*TimeSubsystem->GetCurrentTime().ToDisplayString(),
				*UApartmentLifeImmersionLibrary::GetWeatherDisplayString(TimeSubsystem->GetCurrentWeather().Weather)));
		}
		if (UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation())
		{
			AddPanelAction(Panel, TEXT("Cleanliness +10"), FString::Printf(TEXT("%.0f"), Sim->Apartment.Cleanliness), 1100);
		}
		AddPanelAction(Panel, TEXT("Advance 1 hour"), TEXT("Time of day"), 1101);
		AddPanelAction(Panel, TEXT("Advance 1 day"), TEXT("Time of day"), 1102);
		AddPanelAction(Panel, TEXT("Morning lighting"), TEXT("Lighting lab"), 3000);
		AddPanelAction(Panel, TEXT("Night lighting"), TEXT("Lighting lab"), 3003);
		AddPanelAction(Panel, TEXT("Rain weather"), TEXT("Lighting lab"), 3004);
		AddPanelAction(Panel, TEXT("Open lighting lab"), TEXT(""), static_cast<int32>(EApartmentLifeDeveloperHubSection::Lighting));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Economy:
		if (UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation())
		{
			Panel.Footer = FText::FromString(FString::Printf(TEXT("Savings $%.0f"), Sim->Finance.Savings));
		}
		AddPanelAction(Panel, TEXT("Savings +$500"), TEXT("Live edit"), 1200);
		AddPanelAction(Panel, TEXT("Savings +$5000"), TEXT("Live edit"), 1201);
		AddPanelAction(Panel, TEXT("Max money"), TEXT("Quick action"), 4000);
		AddPanelAction(Panel, TEXT("Unlock all clothing"), TEXT("Quick action"), 4001);
		AddPanelAction(Panel, TEXT("Unlock all furniture"), TEXT("Quick action"), 4002);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::SaveSystem:
		AddPanelAction(Panel, TEXT("Duplicate save slot"), TEXT("Backup"), 8000);
		AddPanelAction(Panel, TEXT("Backup current profile"), TEXT("Backup"), 8001);
		AddPanelAction(Panel, TEXT("Restore backup"), TEXT("Backup"), 8002);
		AddPanelAction(Panel, TEXT("Reset clean profile"), TEXT("Delete source slot"), 8004);
		AddPanelAction(Panel, TEXT("Open backup tools"), TEXT(""), static_cast<int32>(EApartmentLifeDeveloperHubSection::Backup));
		Panel.Footer = FText::FromString(FString::Printf(TEXT("Source slot %d | Target slot %d"),
			DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::ScreenshotStudio:
		AddPanelAction(Panel, TEXT("Enter photo mode"), TEXT("Hide UI, orbit camera"), 2040);
		AddPanelAction(Panel, TEXT("Take screenshot"), TEXT("High-res capture"), 2041);
		AddPanelAction(Panel, TEXT("Studio lighting"), TEXT("Lighting preset"), 3008);
		AddPanelAction(Panel, TEXT("Warm indoor"), TEXT("Lighting preset"), 3006);
		AddPanelAction(Panel, TEXT("Open camera controls"), TEXT(""), static_cast<int32>(EApartmentLifeDeveloperHubSection::Camera));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::QuickActions:
		AddPanelAction(Panel, TEXT("Max money"), TEXT("+$99,999"), 4000);
		AddPanelAction(Panel, TEXT("Unlock all clothing"), TEXT(""), 4001);
		AddPanelAction(Panel, TEXT("Unlock all furniture"), TEXT(""), 4002);
		AddPanelAction(Panel, TEXT("Reset apartment"), TEXT(""), 4003);
		AddPanelAction(Panel, TEXT("Reset appearance"), TEXT(""), 4004);
		AddPanelAction(Panel, TEXT("Complete activity"), TEXT(""), 4005);
		AddPanelAction(Panel, TEXT("Advance 1 hour"), TEXT(""), 4006);
		AddPanelAction(Panel, TEXT("Advance 1 day"), TEXT(""), 4007);
		AddPanelAction(Panel, TEXT("Empty inventory"), TEXT(""), 4008);
		AddPanelAction(Panel, TEXT("Restore default settings"), TEXT(""), 4009);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::ContentBrowser:
		Panel.Subtitle = FText::FromString(TEXT("Click to select, double-click to preview"));
		for (int32 EntryIndex = 0; EntryIndex < DeveloperUi->GetVisibleEntries().Num(); ++EntryIndex)
		{
			const FApartmentLifeDeveloperContentEntry& Entry = DeveloperUi->GetVisibleEntries()[EntryIndex];
			FApartmentLifeUiListEntry ListEntry;
			ListEntry.Label = Entry.DisplayName;
			ListEntry.Detail = Entry.Detail;
			ListEntry.bOwned = Entry.bFavorite;
			ListEntry.Index = EntryIndex;
			Panel.Entries.Add(ListEntry);
		}
		AddPanelAction(Panel, TEXT("All content"), TEXT("Filter"), 5000);
		AddPanelAction(Panel, TEXT("Clothing only"), TEXT("Filter"), 5001);
		AddPanelAction(Panel, TEXT("Activities only"), TEXT("Filter"), 5002);
		AddPanelAction(Panel, TEXT("Furniture only"), TEXT("Filter"), 5003);
		AddPanelAction(Panel, TEXT("Toggle favorites filter"), TEXT(""), 5010);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Validation:
		{
			const FApartmentLifeDeveloperValidationReport& Report = DevSubsystem->GetLastValidationReport();
			Panel.Footer = FText::FromString(Report.Summary.IsEmpty() ? TEXT("Run validation to scan catalogs and saves") : Report.Summary);
			AddPanelAction(Panel, TEXT("Run full validation"), TEXT("Scan IDs, outfits, saves"), 6000);
			for (int32 IssueIndex = 0; IssueIndex < FMath::Min(Report.Issues.Num(), 12); ++IssueIndex)
			{
				const FApartmentLifeDeveloperValidationIssue& Issue = Report.Issues[IssueIndex];
				AddPanelAction(Panel, Issue.Message, Issue.Category, 6100 + IssueIndex);
			}
		}
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Performance:
		Panel.Footer = FText::FromString(
			UApartmentLifeDeveloperLibrary::FormatPerformanceSnapshot(DevSubsystem->GetPerformanceSnapshot()));
		AddPanelAction(Panel,
			DevSubsystem->IsPerformanceOverlayVisible() ? TEXT("Hide perf overlay") : TEXT("Show perf overlay"),
			TEXT("Toggle ` overlay"), 7000);
		AddPanelAction(Panel, TEXT("Refresh snapshot"), TEXT(""), 7001);
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Backup:
		AddPanelAction(Panel, TEXT("Duplicate save slot"), TEXT("Copy source to target"), 8000);
		AddPanelAction(Panel, TEXT("Backup current profile"), TEXT("Save to target slot"), 8001);
		AddPanelAction(Panel, TEXT("Restore backup"), TEXT("Load target slot"), 8002);
		AddPanelAction(Panel, TEXT("Delete source save"), TEXT(""), 8003);
		AddPanelAction(Panel, TEXT("Source slot +"), TEXT(""), 8010);
		AddPanelAction(Panel, TEXT("Target slot +"), TEXT(""), 8011);
		Panel.Footer = FText::FromString(FString::Printf(TEXT("Source slot %d | Target slot %d"),
			DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Dashboard:
		for (int32 CatIndex = 0; CatIndex < DevSubsystem->GetDashboard().Num(); ++CatIndex)
		{
			const FApartmentLifeDeveloperDashboardCategory& Cat = DevSubsystem->GetDashboard()[CatIndex];
			AddPanelAction(Panel,
				FString::Printf(TEXT("%s — %d%%"), *Cat.Name, Cat.CompletionPercent),
				Cat.Notes.IsEmpty() ? TEXT("Click to cycle completion") : Cat.Notes,
				9000 + CatIndex);
		}
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Animation:
		AddPanelAction(Panel, TEXT("Play idle"), TEXT("Animation preview"), 10000);
		AddPanelAction(Panel, TEXT("Play yoga"), TEXT("Animation preview"), 10001);
		AddPanelAction(Panel, TEXT("Play conversation"), TEXT("Animation preview"), 10002);
		AddPanelAction(Panel, TEXT("Pause / resume"), TEXT("Toggle playback"), 10003);
		AddPanelAction(Panel, TEXT("Toggle loop"), TEXT("Loop playback"), 10004);
		AddPanelAction(Panel, TEXT("Slow motion 0.5x"), TEXT("Playback speed"), 10005);
		AddPanelAction(Panel, TEXT("Normal speed 1x"), TEXT("Playback speed"), 10006);
		AddPanelAction(Panel, TEXT("Happy expression"), TEXT("Facial preview"), 10010);
		AddPanelAction(Panel, TEXT("Sleepy expression"), TEXT("Facial preview"), 10011);
		AddPanelAction(Panel, TEXT("Show IK targets"), TEXT("Debug draw"), 10020);
		AddPanelAction(Panel, TEXT("Reset animation"), TEXT(""), 10021);
		AddPanelAction(Panel, TEXT("Open content browser"), TEXT("All animations"), static_cast<int32>(EApartmentLifeDeveloperHubSection::ContentBrowser));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;

	case EApartmentLifeDeveloperHubSection::Wardrobe:
		DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Clothing);
		DeveloperUi->NavigateToSection(EApartmentLifeDeveloperHubSection::ContentBrowser);
		PushDeveloperHubPanel();
		return;

	case EApartmentLifeDeveloperHubSection::Furniture:
		DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Furniture);
		DeveloperUi->NavigateToSection(EApartmentLifeDeveloperHubSection::ContentBrowser);
		PushDeveloperHubPanel();
		return;

	case EApartmentLifeDeveloperHubSection::Activities:
		DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Activity);
		DeveloperUi->NavigateToSection(EApartmentLifeDeveloperHubSection::ContentBrowser);
		PushDeveloperHubPanel();
		return;

	case EApartmentLifeDeveloperHubSection::Dialogue:
		DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Dialogue);
		DeveloperUi->NavigateToSection(EApartmentLifeDeveloperHubSection::ContentBrowser);
		PushDeveloperHubPanel();
		return;

	default:
		AddPanelAction(Panel, TEXT("Open content browser"), TEXT(""), static_cast<int32>(EApartmentLifeDeveloperHubSection::ContentBrowser));
		AddPanelAction(Panel, TEXT("Open quick actions"), TEXT(""), static_cast<int32>(EApartmentLifeDeveloperHubSection::QuickActions));
		AddPanelAction(Panel, TEXT("Back to hub"), TEXT(""), 9999);
		break;
	}

	Ui->SetPanelState(Panel);
}

void UApartmentLifeUiBridgeComponent::HandleDeveloperHubActivation(int32 Index)
{
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	UApartmentLifeDeveloperSubsystem* DevSubsystem = GI ? GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>() : nullptr;
	if (!DeveloperUi.IsValid() || !DevSubsystem)
	{
		return;
	}

	if (Index == 9999)
	{
		DeveloperUi->NavigateToSection(EApartmentLifeDeveloperHubSection::Root);
		PushDeveloperHubPanel();
		return;
	}

	if (Index >= static_cast<int32>(EApartmentLifeDeveloperHubSection::Character)
		&& Index <= static_cast<int32>(EApartmentLifeDeveloperHubSection::Dashboard)
		&& DeveloperUi->GetActiveSection() == EApartmentLifeDeveloperHubSection::Root)
	{
		DeveloperUi->NavigateToSection(static_cast<EApartmentLifeDeveloperHubSection>(Index));
		PushDeveloperHubPanel();
		return;
	}

	AActor* Girl = GirlCharacter.Get();
	APlayerController* PC = OwnerController.Get();

	if (Index == 1000) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("mood"), 10.f);
	else if (Index == 1001) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("mood"), -10.f);
	else if (Index == 1010) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("energy"), 10.f);
	else if (Index == 1020) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("hygiene"), 10.f);
	else if (Index == 1030) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("stress"), 10.f);
	else if (Index == 1040) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("savings"), 500.f);
	else if (Index == 1050) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("confidence"), 10.f);
	else if (Index == 1100)
	{
		if (UApartmentLifeNPCSimulationComponent* Sim = GetGirlSimulation())
		{
			Sim->Apartment.Cleanliness = FMath::Clamp(Sim->Apartment.Cleanliness + 10.f, 0.f, 100.f);
		}
	}
	else if (Index == 1101) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("advance_hour"));
	else if (Index == 1102) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("advance_day"));
	else if (Index == 1200) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("savings"), 500.f);
	else if (Index == 1201) UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(Girl, TEXT("savings"), 5000.f);
	else if (Index == 2000) UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(PC, TEXT("fov"), 2.f);
	else if (Index == 2001) UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(PC, TEXT("fov"), -2.f);
	else if (Index == 2010) UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(PC, TEXT("orbit"), 0.1f);
	else if (Index == 2020) UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(PC, TEXT("zoom"), 0.1f);
	else if (Index == 2030) UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(PC, TEXT("pan"), 0.1f);
	else if (Index == 2040) UApartmentLifeDeveloperLibrary::EnterScreenshotStudio(PC, true);
	else if (Index == 2041) UApartmentLifeDeveloperLibrary::TakeStudioScreenshot(PC);
	else if (Index >= 3000 && Index < 3100)
	{
		UApartmentLifeDeveloperLibrary::ApplyLightingLabPreset(
			this, static_cast<EApartmentLifeDeveloperLightingLabPreset>(Index - 3000));
	}
	else if (Index == 4000) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("max_money"));
	else if (Index == 4001) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("unlock_clothing"));
	else if (Index == 4002) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("unlock_furniture"));
	else if (Index == 4003) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("reset_apartment"));
	else if (Index == 4004) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("reset_appearance"));
	else if (Index == 4005) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("complete_activity"));
	else if (Index == 4006) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("advance_hour"));
	else if (Index == 4007) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("advance_day"));
	else if (Index == 4008) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("empty_inventory"));
	else if (Index == 4009) UApartmentLifeDeveloperLibrary::ExecuteQuickAction(this, Girl, ApartmentUnit.Get(), TEXT("restore_defaults"));
	else if (Index == 5000) DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::All);
	else if (Index == 5001) DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Clothing);
	else if (Index == 5002) DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Activity);
	else if (Index == 5003) DeveloperUi->SetContentCategory(EApartmentLifeDeveloperContentCategory::Furniture);
	else if (Index == 5010) DeveloperUi->ToggleFavoritesOnly();
	else if (Index == 6000)
	{
		const FApartmentLifeDeveloperValidationReport Report =
			UApartmentLifeDeveloperLibrary::RunDataValidation(this, Girl);
		DevSubsystem->StoreValidationReport(Report);
		if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
		{
			Ui->ShowToast(FText::FromString(Report.Summary), 3.f);
		}
	}
	else if (Index == 7000) DevSubsystem->TogglePerformanceOverlay();
	else if (Index == 7001) DevSubsystem->TickPerformance(0.f, this, Girl, QuickSaveSlot);
	else if (Index == 8000) UApartmentLifeDeveloperLibrary::ExecuteBackupAction(this, TEXT("duplicate_save"), DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot);
	else if (Index == 8001) UApartmentLifeDeveloperLibrary::ExecuteBackupAction(this, TEXT("backup_profile"), DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot);
	else if (Index == 8002) UApartmentLifeDeveloperLibrary::ExecuteBackupAction(this, TEXT("restore_backup"), DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot);
	else if (Index == 8003) UApartmentLifeDeveloperLibrary::ExecuteBackupAction(this, TEXT("delete_save"), DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot);
	else if (Index == 8004) UApartmentLifeDeveloperLibrary::ExecuteBackupAction(this, TEXT("reset_clean_profile"), DevSubsystem->GetHubState().SourceSaveSlot, DevSubsystem->GetHubState().TargetSaveSlot);
	else if (Index == 8010) DevSubsystem->GetMutableHubState().SourceSaveSlot = (DevSubsystem->GetHubState().SourceSaveSlot + 1) % 8;
	else if (Index == 8011) DevSubsystem->GetMutableHubState().TargetSaveSlot = (DevSubsystem->GetHubState().TargetSaveSlot + 1) % 8;
	else if (Index >= 9000 && Index < 9100)
	{
		TArray<FApartmentLifeDeveloperDashboardCategory>& Dashboard = DevSubsystem->GetMutableDashboard();
		const int32 CatIndex = Index - 9000;
		if (Dashboard.IsValidIndex(CatIndex))
		{
			Dashboard[CatIndex].CompletionPercent = (Dashboard[CatIndex].CompletionPercent + 5) % 105;
		}
	}
	else if (Index == 10000) UApartmentLifeDevToolsLibrary::ResetAnimationState(Girl);
	else if (Index == 10001) UApartmentLifeDevToolsLibrary::PreviewAnimation(Girl, FName(TEXT("anim.Yoga")));
	else if (Index == 10002) UApartmentLifeDevToolsLibrary::PreviewAnimation(Girl, FName(TEXT("anim.Conversation")));
	else if (Index == 10003)
	{
		FApartmentLifeDeveloperAnimationPreviewState& Preview = DevSubsystem->GetMutableAnimationPreviewState();
		Preview.bPlaying = !Preview.bPlaying;
		UApartmentLifeDeveloperLibrary::ApplyAnimationPreviewControls(Girl, Preview);
	}
	else if (Index == 10004)
	{
		FApartmentLifeDeveloperAnimationPreviewState& Preview = DevSubsystem->GetMutableAnimationPreviewState();
		Preview.bLooping = !Preview.bLooping;
		UApartmentLifeDeveloperLibrary::ApplyAnimationPreviewControls(Girl, Preview);
	}
	else if (Index == 10005)
	{
		FApartmentLifeDeveloperAnimationPreviewState& Preview = DevSubsystem->GetMutableAnimationPreviewState();
		Preview.PlaybackSpeed = 0.5f;
		UApartmentLifeDeveloperLibrary::ApplyAnimationPreviewControls(Girl, Preview);
	}
	else if (Index == 10006)
	{
		FApartmentLifeDeveloperAnimationPreviewState& Preview = DevSubsystem->GetMutableAnimationPreviewState();
		Preview.PlaybackSpeed = 1.f;
		UApartmentLifeDeveloperLibrary::ApplyAnimationPreviewControls(Girl, Preview);
	}
	else if (Index == 10010) UApartmentLifeDevToolsLibrary::ForceFacialExpression(Girl, EApartmentLifeFacialExpression::Happy);
	else if (Index == 10011) UApartmentLifeDevToolsLibrary::ForceFacialExpression(Girl, EApartmentLifeFacialExpression::Sleepy);
	else if (Index == 10020) UApartmentLifeDevToolsLibrary::ShowIkTargets(Girl, 8.f);
	else if (Index == 10021) UApartmentLifeDevToolsLibrary::ResetAnimationState(Girl);
	else if (DeveloperUi->GetActiveSection() == EApartmentLifeDeveloperHubSection::ContentBrowser)
	{
		DeveloperUi->ActivateEntryIndex(Index);
	}

	PushDeveloperHubPanel();
	RefreshHud();
}

void UApartmentLifeUiBridgeComponent::HandleDeveloperUiStateChanged()
{
	if (UApartmentLifeUiSubsystem* Ui = GetUiSubsystem())
	{
		if (DeveloperUi.IsValid() && DeveloperUi->IsDeveloperHubOpen())
		{
			Ui->ShowScreen(EApartmentLifeUiScreen::DeveloperHub);
			PushDeveloperHubPanel();
		}
		else if (Ui->GetActiveScreen() == EApartmentLifeUiScreen::DeveloperHub)
		{
			Ui->CloseScreen();
		}
	}
}

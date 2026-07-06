// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDeveloperLibrary.h"
#include "ApartmentLifeDevToolsLibrary.h"
#include "ApartmentLifeDeveloperSubsystem.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifeCameraSettingsSubsystem.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeYogaPoseCatalogLibrary.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeImmersionSubsystem.h"
#include "ApartmentLifeImmersionLibrary.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeAnimationLibrary.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeYogaMinigameComponent.h"
#include "ApartmentLifeTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformMemory.h"
#include "GenericPlatform/GenericPlatformMemoryStats.h"

namespace
{
	bool MatchesSearch(const FString& Haystack, const FString& Query)
	{
		return Query.IsEmpty() || Haystack.ToLower().Contains(Query.ToLower());
	}

	void AddContentEntry(
		TArray<FApartmentLifeDeveloperContentEntry>& OutEntries,
		FName EntryId,
		const FText& DisplayName,
		const FText& Detail,
		EApartmentLifeDeveloperContentCategory Category,
		const TArray<FName>& FavoriteIds)
	{
		FApartmentLifeDeveloperContentEntry Entry;
		Entry.EntryId = EntryId;
		Entry.DisplayName = DisplayName;
		Entry.Detail = Detail;
		Entry.Category = Category;
		Entry.bFavorite = FavoriteIds.Contains(EntryId);
		OutEntries.Add(Entry);
	}

	void AddValidationIssue(
		FApartmentLifeDeveloperValidationReport& Report,
		EApartmentLifeDeveloperValidationSeverity Severity,
		const FString& Category,
		const FString& Message,
		FName RelatedId = NAME_None)
	{
		FApartmentLifeDeveloperValidationIssue Issue;
		Issue.Severity = Severity;
		Issue.Category = Category;
		Issue.Message = Message;
		Issue.RelatedId = RelatedId;
		Report.Issues.Add(Issue);
		if (Severity == EApartmentLifeDeveloperValidationSeverity::Error)
		{
			++Report.ErrorCount;
		}
		else if (Severity == EApartmentLifeDeveloperValidationSeverity::Warning)
		{
			++Report.WarningCount;
		}
	}
}

TArray<FApartmentLifeDeveloperContentEntry> UApartmentLifeDeveloperLibrary::QueryContentBrowser(
	EApartmentLifeDeveloperContentCategory Category,
	const FString& SearchQuery,
	bool bFavoritesOnly,
	const TArray<FName>& FavoriteIds)
{
	TArray<FApartmentLifeDeveloperContentEntry> Entries;

	const auto ShouldInclude = [&](EApartmentLifeDeveloperContentCategory ItemCategory) -> bool
	{
		return Category == EApartmentLifeDeveloperContentCategory::All || Category == ItemCategory;
	};

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Clothing))
	{
		for (const FApartmentLifeBuiltinClothingItem& Item : UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog())
		{
			const FString Label = Item.DisplayName.ToString();
			if (!MatchesSearch(Label, SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Item.ItemId, Item.DisplayName,
				FText::FromString(FString::Printf(TEXT("$%.0f | %s"), Item.Price, *UEnum::GetValueAsString(Item.Layer))),
				EApartmentLifeDeveloperContentCategory::Clothing, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Hairstyle))
	{
		for (const FName HairstyleId : UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinHairstyleIds())
		{
			if (!MatchesSearch(HairstyleId.ToString(), SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, HairstyleId, FText::FromName(HairstyleId),
				FText::FromString(TEXT("Hairstyle")), EApartmentLifeDeveloperContentCategory::Hairstyle, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Furniture))
	{
		for (const FApartmentLifeBuiltinShopItem& Item : UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog())
		{
			if (Item.Category != EApartmentLifeShopCategory::Furniture && Item.Category != EApartmentLifeShopCategory::Decorations)
			{
				continue;
			}
			const FString Label = Item.DisplayName.ToString();
			if (!MatchesSearch(Label, SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Item.ItemId, Item.DisplayName,
				FText::FromString(FString::Printf(TEXT("$%.0f"), Item.Price)),
				EApartmentLifeDeveloperContentCategory::Furniture, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Activity))
	{
		for (const FApartmentLifeActivityDefinition& Activity : UApartmentLifeActivityCatalogLibrary::GetBuiltinCatalog())
		{
			const FString Label = Activity.DisplayName.ToString();
			if (!MatchesSearch(Label, SearchQuery) && !MatchesSearch(Activity.ActivityId.ToString(), SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Activity.ActivityId, Activity.DisplayName,
				FText::FromString(FString::Printf(TEXT("%d min"), Activity.DurationMinutes)),
				EApartmentLifeDeveloperContentCategory::Activity, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Routine))
	{
		for (const FApartmentLifeRoutineChainDefinition& Chain : UApartmentLifeActivityCatalogLibrary::GetBuiltinRoutineChains())
		{
			const FString Label = Chain.DisplayName.ToString();
			if (!MatchesSearch(Label, SearchQuery) && !MatchesSearch(Chain.ChainId.ToString(), SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Chain.ChainId, Chain.DisplayName,
				FText::FromString(FString::Printf(TEXT("%d steps"), Chain.Steps.Num())),
				EApartmentLifeDeveloperContentCategory::Routine, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Animation))
	{
		const UEnum* AnimEnum = StaticEnum<EApartmentLifeAnimationGroup>();
		if (AnimEnum)
		{
			for (int32 Index = 0; Index < AnimEnum->NumEnums() - 1; ++Index)
			{
				const EApartmentLifeAnimationGroup Group = static_cast<EApartmentLifeAnimationGroup>(AnimEnum->GetValueByIndex(Index));
				const FName GroupId = UApartmentLifeCharacterPipelineLibrary::GetAnimationIdForGroup(Group);
				const FString Label = AnimEnum->GetDisplayNameTextByIndex(Index).ToString();
				if (!MatchesSearch(Label, SearchQuery))
				{
					continue;
				}
				AddContentEntry(Entries, GroupId, FText::FromString(Label),
					FText::FromString(TEXT("Animation group")),
					EApartmentLifeDeveloperContentCategory::Animation, FavoriteIds);
			}
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::YogaPose))
	{
		for (const FApartmentLifeBuiltinYogaPose& Pose : UApartmentLifeYogaPoseCatalogLibrary::GetBuiltinPoses())
		{
			if (!MatchesSearch(Pose.PoseId.ToString(), SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Pose.PoseId, FText::FromName(Pose.PoseId),
				FText::FromString(FString::Printf(TEXT("Difficulty %.1f"), Pose.Difficulty)),
				EApartmentLifeDeveloperContentCategory::YogaPose, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::WorkType))
	{
		for (const FApartmentLifeWorkTypeDefinition& Work : UApartmentLifeWorkLibrary::GetBuiltinWorkTypes())
		{
			const FString Label = Work.DisplayName.ToString();
			if (!MatchesSearch(Label, SearchQuery))
			{
				continue;
			}
			AddContentEntry(Entries, Work.WorkTypeId, Work.DisplayName,
				FText::FromString(FString::Printf(TEXT("$%.0f"), Work.BasePayout)),
				EApartmentLifeDeveloperContentCategory::WorkType, FavoriteIds);
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Dialogue))
	{
		const UEnum* TopicEnum = StaticEnum<EApartmentLifeConversationTopic>();
		if (TopicEnum)
		{
			for (int32 Index = 0; Index < TopicEnum->NumEnums() - 1; ++Index)
			{
				const FName TopicId = FName(*FString::Printf(TEXT("dialogue.topic.%s"), *TopicEnum->GetNameStringByIndex(Index)));
				const FString Label = TopicEnum->GetDisplayNameTextByIndex(Index).ToString();
				if (!MatchesSearch(Label, SearchQuery))
				{
					continue;
				}
				AddContentEntry(Entries, TopicId, FText::FromString(Label),
					FText::FromString(TEXT("Conversation topic")),
					EApartmentLifeDeveloperContentCategory::Dialogue, FavoriteIds);
			}
		}
	}

	if (ShouldInclude(EApartmentLifeDeveloperContentCategory::Music))
	{
		const UEnum* MusicEnum = StaticEnum<EApartmentLifeMusicCategory>();
		if (MusicEnum)
		{
			for (int32 Index = 0; Index < MusicEnum->NumEnums() - 1; ++Index)
			{
				const EApartmentLifeMusicCategory MusicCategory = static_cast<EApartmentLifeMusicCategory>(MusicEnum->GetValueByIndex(Index));
				const FName MusicId = UApartmentLifeImmersionLibrary::GetMusicIdForCategory(MusicCategory);
				const FString Label = MusicEnum->GetDisplayNameTextByIndex(Index).ToString();
				if (!MatchesSearch(Label, SearchQuery))
				{
					continue;
				}
				AddContentEntry(Entries, MusicId, FText::FromString(Label),
					FText::FromString(TEXT("Music category")),
					EApartmentLifeDeveloperContentCategory::Music, FavoriteIds);
			}
		}
	}

	if (bFavoritesOnly)
	{
		Entries.RemoveAll([&FavoriteIds](const FApartmentLifeDeveloperContentEntry& Entry)
		{
			return !FavoriteIds.Contains(Entry.EntryId);
		});
	}

	Entries.Sort([](const FApartmentLifeDeveloperContentEntry& A, const FApartmentLifeDeveloperContentEntry& B)
	{
		if (A.bFavorite != B.bFavorite)
		{
			return A.bFavorite;
		}
		return A.DisplayName.ToString() < B.DisplayName.ToString();
	});

	return Entries;
}

bool UApartmentLifeDeveloperLibrary::PreviewContentEntry(
	UObject* WorldContextObject,
	AActor* Character,
	AApartmentLifeApartmentUnit* Apartment,
	const FApartmentLifeDeveloperContentEntry& Entry)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	switch (Entry.Category)
	{
	case EApartmentLifeDeveloperContentCategory::Clothing:
		if (UApartmentLifeWardrobeComponent* Wardrobe = Character ? Character->FindComponentByClass<UApartmentLifeWardrobeComponent>() : nullptr)
		{
			FApartmentLifeBuiltinClothingItem Item;
			if (UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(Entry.EntryId, Item))
			{
				if (!Wardrobe->OwnsClothing(Entry.EntryId))
				{
					Wardrobe->AddOwnedClothing(Entry.EntryId);
				}
				Wardrobe->EquipClothing(Entry.EntryId, Item.Layer);
				return true;
			}
		}
		break;

	case EApartmentLifeDeveloperContentCategory::Hairstyle:
		if (UApartmentLifeCharacterCreatorComponent* Creator = Character ? Character->FindComponentByClass<UApartmentLifeCharacterCreatorComponent>() : nullptr)
		{
			Creator->SetHairstyle(Entry.EntryId);
			return true;
		}
		break;

	case EApartmentLifeDeveloperContentCategory::Furniture:
		return UApartmentLifeDevToolsLibrary::SpawnFurnitureAtCursor(
			WorldContextObject, Apartment, Entry.EntryId, EApartmentLifeRoomType::LivingRoom);

	case EApartmentLifeDeveloperContentCategory::Activity:
		return UApartmentLifeDevToolsLibrary::TriggerActivity(Character, Entry.EntryId);

	case EApartmentLifeDeveloperContentCategory::Routine:
		return UApartmentLifeDevToolsLibrary::PreviewRoutineChain(Character, Entry.EntryId);

	case EApartmentLifeDeveloperContentCategory::Animation:
		return UApartmentLifeDevToolsLibrary::PreviewAnimation(Character, Entry.EntryId);

	case EApartmentLifeDeveloperContentCategory::YogaPose:
		if (Character)
		{
			if (UApartmentLifeYogaMinigameComponent* Yoga = Character->FindComponentByClass<UApartmentLifeYogaMinigameComponent>())
			{
				Yoga->StartYogaSession(Entry.EntryId, true);
				return true;
			}
		}
		break;

	case EApartmentLifeDeveloperContentCategory::WorkType:
		return UApartmentLifeDevToolsLibrary::TriggerActivity(Character, FName(TEXT("activity.work.computer")));

	case EApartmentLifeDeveloperContentCategory::Dialogue:
		if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				return UApartmentLifeDevToolsLibrary::PreviewDialogue(Character, PC->GetPawn());
			}
		}
		break;

	case EApartmentLifeDeveloperContentCategory::Music:
		if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UApartmentLifeImmersionSubsystem* Immersion = GI->GetSubsystem<UApartmentLifeImmersionSubsystem>())
				{
					for (int32 Index = 0; Index < static_cast<int32>(EApartmentLifeMusicCategory::Yoga) + 8; ++Index)
					{
						const EApartmentLifeMusicCategory Category = static_cast<EApartmentLifeMusicCategory>(Index);
						if (UApartmentLifeImmersionLibrary::GetMusicIdForCategory(Category) == Entry.EntryId)
						{
							Immersion->SetMusicCategory(Category, true);
							return true;
						}
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::ToggleContentFavorite(FName EntryId, TArray<FName>& InOutFavoriteIds)
{
	if (EntryId.IsNone())
	{
		return false;
	}

	if (InOutFavoriteIds.Contains(EntryId))
	{
		InOutFavoriteIds.Remove(EntryId);
	}
	else
	{
		InOutFavoriteIds.AddUnique(EntryId);
	}
	return true;
}

FApartmentLifeDeveloperValidationReport UApartmentLifeDeveloperLibrary::RunDataValidation(
	UObject* WorldContextObject,
	AActor* Character)
{
	FApartmentLifeDeveloperValidationReport Report;

	TSet<FName> ClothingIds;
	for (const FApartmentLifeBuiltinClothingItem& Item : UApartmentLifeWardrobeCatalogLibrary::GetBuiltinCatalog())
	{
		if (ClothingIds.Contains(Item.ItemId))
		{
			AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Error, TEXT("Clothing"),
				FString::Printf(TEXT("Duplicate clothing ID: %s"), *Item.ItemId.ToString()), Item.ItemId);
		}
		ClothingIds.Add(Item.ItemId);
		if (Item.DisplayName.IsEmpty())
		{
			AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Warning, TEXT("Clothing"),
				TEXT("Missing display name"), Item.ItemId);
		}
	}

	TSet<FName> ShopIds;
	for (const FApartmentLifeBuiltinShopItem& Item : UApartmentLifeShoppingCatalogLibrary::GetBuiltinCatalog())
	{
		if (ShopIds.Contains(Item.ItemId))
		{
			AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Error, TEXT("Shop"),
				FString::Printf(TEXT("Duplicate shop ID: %s"), *Item.ItemId.ToString()), Item.ItemId);
		}
		ShopIds.Add(Item.ItemId);
	}

	TSet<FName> ActivityIds;
	for (const FApartmentLifeActivityDefinition& Activity : UApartmentLifeActivityCatalogLibrary::GetBuiltinCatalog())
	{
		if (ActivityIds.Contains(Activity.ActivityId))
		{
			AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Error, TEXT("Activity"),
				FString::Printf(TEXT("Duplicate activity ID: %s"), *Activity.ActivityId.ToString()), Activity.ActivityId);
		}
		ActivityIds.Add(Activity.ActivityId);
		if (Activity.DurationMinutes <= 0)
		{
			AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Warning, TEXT("Activity"),
				TEXT("Zero or negative duration"), Activity.ActivityId);
		}
	}

	if (UApartmentLifeWardrobeComponent* Wardrobe = Character ? Character->FindComponentByClass<UApartmentLifeWardrobeComponent>() : nullptr)
	{
		for (const FApartmentLifeEquippedClothingSlot& Slot : Wardrobe->GetEquippedSlots())
		{
			if (!ClothingIds.Contains(Slot.ClothingItemId))
			{
				AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Error, TEXT("Outfit"),
					FString::Printf(TEXT("Equipped item not in catalog: %s"), *Slot.ClothingItemId.ToString()), Slot.ClothingItemId);
			}
		}
	}

	if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeSaveSubsystem* Save = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
			{
				for (int32 SlotIndex = 0; SlotIndex < 8; ++SlotIndex)
				{
					if (Save->DoesSaveExist(SlotIndex))
					{
						int32 SaveVersion = 0;
						FApartmentLifeSaveSlotMetadata Metadata;
						if (!Save->GetSlotMetadata(SlotIndex, Metadata, SaveVersion))
						{
							AddValidationIssue(Report, EApartmentLifeDeveloperValidationSeverity::Warning, TEXT("Save"),
								FString::Printf(TEXT("Slot %d metadata unreadable"), SlotIndex));
						}
					}
				}
			}
		}
	}

	Report.Summary = FString::Printf(TEXT("%d issues (%d errors, %d warnings)"),
		Report.Issues.Num(), Report.ErrorCount, Report.WarningCount);
	return Report;
}

FString UApartmentLifeDeveloperLibrary::FormatValidationReport(const FApartmentLifeDeveloperValidationReport& Report)
{
	FString Output = FString::Printf(TEXT("Validation: %s\n"), *Report.Summary);
	for (const FApartmentLifeDeveloperValidationIssue& Issue : Report.Issues)
	{
		const TCHAR* Severity = Issue.Severity == EApartmentLifeDeveloperValidationSeverity::Error ? TEXT("ERR")
			: Issue.Severity == EApartmentLifeDeveloperValidationSeverity::Warning ? TEXT("WRN") : TEXT("INF");
		Output += FString::Printf(TEXT("[%s][%s] %s (%s)\n"), Severity, *Issue.Category, *Issue.Message,
			Issue.RelatedId.IsNone() ? TEXT("-") : *Issue.RelatedId.ToString());
	}
	return Output;
}

bool UApartmentLifeDeveloperLibrary::ApplyLightingLabPreset(UObject* WorldContextObject, EApartmentLifeDeveloperLightingLabPreset Preset)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	UGameInstance* GI = World->GetGameInstance();
	UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>();
	UApartmentLifeImmersionSubsystem* Immersion = GI ? GI->GetSubsystem<UApartmentLifeImmersionSubsystem>() : nullptr;
	if (!TimeSubsystem || !Immersion)
	{
		return false;
	}

	int32 Hour = 12;
	EApartmentLifeWeather Weather = EApartmentLifeWeather::Clear;
	EApartmentLifeLightingPreset Lighting = EApartmentLifeLightingPreset::Midday;

	switch (Preset)
	{
	case EApartmentLifeDeveloperLightingLabPreset::Morning: Hour = 7; Lighting = EApartmentLifeLightingPreset::Morning; break;
	case EApartmentLifeDeveloperLightingLabPreset::Noon: Hour = 12; Lighting = EApartmentLifeLightingPreset::Midday; break;
	case EApartmentLifeDeveloperLightingLabPreset::Sunset: Hour = 18; Lighting = EApartmentLifeLightingPreset::GoldenHour; break;
	case EApartmentLifeDeveloperLightingLabPreset::Night: Hour = 22; Lighting = EApartmentLifeLightingPreset::Night; break;
	case EApartmentLifeDeveloperLightingLabPreset::Rain: Hour = 14; Weather = EApartmentLifeWeather::Rain; Lighting = EApartmentLifeLightingPreset::RainyDay; break;
	case EApartmentLifeDeveloperLightingLabPreset::Cloudy: Hour = 11; Weather = EApartmentLifeWeather::Cloudy; Lighting = EApartmentLifeLightingPreset::CloudyDay; break;
	case EApartmentLifeDeveloperLightingLabPreset::WarmIndoor: Hour = 20; Lighting = EApartmentLifeLightingPreset::Evening; break;
	case EApartmentLifeDeveloperLightingLabPreset::CoolIndoor: Hour = 15; Lighting = EApartmentLifeLightingPreset::CloudyDay; break;
	case EApartmentLifeDeveloperLightingLabPreset::Studio: Hour = 12; Lighting = EApartmentLifeLightingPreset::Midday; break;
	default: break;
	}

	FApartmentLifeGameTime Time = TimeSubsystem->GetCurrentTime();
	Time.Hour = Hour;
	Time.Minute = 0;
	TimeSubsystem->SetCurrentTime(Time);
	FApartmentLifeWeatherState WeatherState = TimeSubsystem->GetCurrentWeather();
	WeatherState.Weather = Weather;
	TimeSubsystem->SetWeather(WeatherState);
	Immersion->UpdateFromGameTime(Hour, Weather);

	if (Preset == EApartmentLifeDeveloperLightingLabPreset::WarmIndoor
		|| Preset == EApartmentLifeDeveloperLightingLabPreset::CoolIndoor
		|| Preset == EApartmentLifeDeveloperLightingLabPreset::Studio)
	{
		FApartmentLifePhotoBookmark Bookmark;
		Bookmark.LightingPreset = Lighting;
		Bookmark.DisplayName = FText::FromString(TEXT("DevLightingLab"));
		Immersion->SavePhotoBookmark(Bookmark);
		Immersion->ApplyPhotoBookmark(Immersion->GetUserSettings().PhotoBookmarks.Num() - 1);
	}

	return true;
#endif
}

FApartmentLifeDeveloperPerformanceSnapshot UApartmentLifeDeveloperLibrary::CapturePerformanceSnapshot(
	UObject* WorldContextObject,
	AActor* Character,
	int32 SaveSlotIndex)
{
	FApartmentLifeDeveloperPerformanceSnapshot Snapshot;

	if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
	{
		Snapshot.Fps = World->GetDeltaSeconds() > KINDA_SMALL_NUMBER ? 1.f / World->GetDeltaSeconds() : 0.f;
		Snapshot.FrameTimeMs = World->GetDeltaSeconds() * 1000.f;

		const FPlatformMemoryStats MemStats = FPlatformMemory::GetStats();
		Snapshot.MemoryUsedMb = static_cast<float>(MemStats.UsedPhysical) / (1024.f * 1024.f);

		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeSaveSubsystem* Save = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
			{
				if (Save->DoesSaveExist(SaveSlotIndex))
				{
					Snapshot.SaveSizeKb = 64;
				}
			}
		}
	}

	if (Character)
	{
		if (UApartmentLifeAnimationComponent* Animation = Character->FindComponentByClass<UApartmentLifeAnimationComponent>())
		{
			Snapshot.ActiveAnimations = Animation->GetAnimationState().MontageId.IsNone() ? 0 : 1;
		}
		if (UApartmentLifeInteractionSelectionComponent* Selection = Character->FindComponentByClass<UApartmentLifeInteractionSelectionComponent>())
		{
			Snapshot.ActiveInteractions = Selection->GetSelectedInteractable() ? 1 : 0;
		}
	}

	return Snapshot;
}

FString UApartmentLifeDeveloperLibrary::FormatPerformanceSnapshot(const FApartmentLifeDeveloperPerformanceSnapshot& Snapshot)
{
	return FString::Printf(
		TEXT("FPS %.0f | Frame %.1fms | Mem %.0fMB | Draw %d | Anim %d | Interact %d | Save %dKB"),
		Snapshot.Fps, Snapshot.FrameTimeMs, Snapshot.MemoryUsedMb,
		Snapshot.DrawCalls, Snapshot.ActiveAnimations, Snapshot.ActiveInteractions, Snapshot.SaveSizeKb);
}

TArray<FApartmentLifeDeveloperDashboardCategory> UApartmentLifeDeveloperLibrary::BuildDefaultDashboard()
{
	TArray<FApartmentLifeDeveloperDashboardCategory> Categories;
	auto Add = [&Categories](const FString& Name, int32 Percent)
	{
		FApartmentLifeDeveloperDashboardCategory Cat;
		Cat.Name = Name;
		Cat.CompletionPercent = Percent;
		Categories.Add(Cat);
	};

	Add(TEXT("Camera"), 100);
	Add(TEXT("Wardrobe"), 95);
	Add(TEXT("Apartment"), 90);
	Add(TEXT("Activities"), 80);
	Add(TEXT("Animation"), 85);
	Add(TEXT("UI"), 92);
	Add(TEXT("Optimization"), 70);
	Add(TEXT("Testing"), 65);
	return Categories;
}

bool UApartmentLifeDeveloperLibrary::ExecuteQuickAction(
	UObject* WorldContextObject,
	AActor* Character,
	AApartmentLifeApartmentUnit* Apartment,
	FName ActionId)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	const FString Action = ActionId.ToString();
	if (Action == TEXT("max_money"))
	{
		return UApartmentLifeDevToolsLibrary::AddMoney(WorldContextObject, Character, 99999.f);
	}
	if (Action == TEXT("empty_inventory"))
	{
		if (UApartmentLifeWardrobeComponent* Wardrobe = Character ? Character->FindComponentByClass<UApartmentLifeWardrobeComponent>() : nullptr)
		{
			Wardrobe->SetClosetInventory(TArray<FName>());
			return true;
		}
	}
	if (Action == TEXT("unlock_clothing"))
	{
		return UApartmentLifeDevToolsLibrary::UnlockAllClothing(Character);
	}
	if (Action == TEXT("unlock_furniture"))
	{
		return UApartmentLifeDevToolsLibrary::UnlockAllShopItems(Character);
	}
	if (Action == TEXT("reset_apartment"))
	{
		return UApartmentLifeDevToolsLibrary::ResetApartmentLayout(Apartment);
	}
	if (Action == TEXT("reset_appearance"))
	{
		UApartmentLifeDevToolsLibrary::ResetCreatorFace(Character);
		UApartmentLifeDevToolsLibrary::ResetCreatorBody(Character);
		return UApartmentLifeDevToolsLibrary::ResetEquippedOutfit(Character);
	}
	if (Action == TEXT("complete_activity"))
	{
		return UApartmentLifeDevToolsLibrary::CompleteCurrentActivity(Character);
	}
	if (Action == TEXT("advance_hour"))
	{
		if (UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
		{
			if (UApartmentLifeGameTimeSubsystem* Time = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				Time->AdvanceMinutes(60);
				return true;
			}
		}
	}
	if (Action == TEXT("advance_day"))
	{
		return AdvanceGameDay(WorldContextObject);
	}
	if (Action == TEXT("restore_defaults"))
	{
		if (UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr)
		{
			if (UApartmentLifeCameraSettingsSubsystem* Cam = GI->GetSubsystem<UApartmentLifeCameraSettingsSubsystem>())
			{
				Cam->SetSettings(FApartmentLifeCameraUserSettings());
			}
		}
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::ExecuteBackupAction(
	UObject* WorldContextObject,
	FName ActionId,
	int32 SourceSlot,
	int32 TargetSlot)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UGameInstance* GI = GEngine ? GEngine->GetGameInstanceFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!GI)
	{
		return false;
	}

	UApartmentLifeSaveSubsystem* Save = GI->GetSubsystem<UApartmentLifeSaveSubsystem>();
	if (!Save)
	{
		return false;
	}

	const FString Action = ActionId.ToString();
	if (Action == TEXT("duplicate_save"))
	{
		if (!Save->DoesSaveExist(SourceSlot))
		{
			return false;
		}
		if (Save->LoadFromSlot(SourceSlot))
		{
			return Save->SaveToSlot(TargetSlot, FString::Printf(TEXT("Copy of slot %d"), SourceSlot));
		}
	}
	if (Action == TEXT("backup_profile"))
	{
		return Save->SaveToSlot(TargetSlot, TEXT("DevBackup"), false);
	}
	if (Action == TEXT("restore_backup"))
	{
		if (Save->DoesSaveExist(TargetSlot))
		{
			return Save->LoadFromSlot(TargetSlot);
		}
	}
	if (Action == TEXT("reset_clean_profile"))
	{
		Save->DeleteSaveSlot(SourceSlot);
		return true;
	}
	if (Action == TEXT("delete_save"))
	{
		return Save->DeleteSaveSlot(SourceSlot);
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::AdjustLiveCharacterStat(AActor* Character, FName StatId, float Delta)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Character))
	{
		const FString Stat = StatId.ToString().ToLower();
		if (Stat == TEXT("mood")) return UApartmentLifeDevToolsLibrary::SetMood(Character, Sim->GetMood().OverallMood + Delta);
		if (Stat == TEXT("energy")) return UApartmentLifeDevToolsLibrary::SetEnergy(Character, Sim->GetMood().Energy + Delta);
		if (Stat == TEXT("hygiene")) return UApartmentLifeDevToolsLibrary::SetHygiene(Character, Sim->Needs.Hygiene + Delta);
		if (Stat == TEXT("confidence")) return UApartmentLifeDevToolsLibrary::SetConfidence(Character, Sim->GetMood().Confidence + Delta);
		if (Stat == TEXT("stress")) return UApartmentLifeDevToolsLibrary::SetStress(Character, Sim->GetMood().Stress + Delta);
		if (Stat == TEXT("savings")) return UApartmentLifeDevToolsLibrary::AddMoney(Character, Character, Delta);
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::SetLiveCharacterStat(AActor* Character, FName StatId, float Value)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	const FString Stat = StatId.ToString().ToLower();
	if (Stat == TEXT("mood")) return UApartmentLifeDevToolsLibrary::SetMood(Character, Value);
	if (Stat == TEXT("energy")) return UApartmentLifeDevToolsLibrary::SetEnergy(Character, Value);
	if (Stat == TEXT("hygiene")) return UApartmentLifeDevToolsLibrary::SetHygiene(Character, Value);
	if (Stat == TEXT("confidence")) return UApartmentLifeDevToolsLibrary::SetConfidence(Character, Value);
	if (Stat == TEXT("stress")) return UApartmentLifeDevToolsLibrary::SetStress(Character, Value);
	if (Stat == TEXT("savings") && UApartmentLifeNPCSimulationComponent* Sim = UApartmentLifeDevToolsLibrary::GetSimulation(Character))
	{
		Sim->Finance.Savings = Value;
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::AdjustLiveApartmentSetting(UObject* WorldContextObject, FName SettingId, float Delta)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	const FString Setting = SettingId.ToString().ToLower();
	if (Setting == TEXT("time"))
	{
		if (UApartmentLifeGameTimeSubsystem* Time = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			FApartmentLifeGameTime Current = Time->GetCurrentTime();
			Current.Hour = (Current.Hour + FMath::RoundToInt(Delta)) % 24;
			Time->SetCurrentTime(Current);
			return true;
		}
	}
	if (Setting == TEXT("comfort"))
	{
		return false;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::AdjustLiveCameraSetting(APlayerController* Controller, FName SettingId, float Delta)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	AApartmentLifeCameraPawn* CameraPawn = Controller ? Cast<AApartmentLifeCameraPawn>(Controller->GetPawn()) : nullptr;
	if (!CameraPawn)
	{
		return false;
	}

	const FString Setting = SettingId.ToString().ToLower();
	if (Setting == TEXT("fov"))
	{
		CameraPawn->AdjustPhotoFieldOfView(Delta);
		return true;
	}
	if (Setting == TEXT("orbit"))
	{
		FApartmentLifeCameraUserSettings Settings = CameraPawn->BuildUserSettings();
		Settings.OrbitSensitivity = FMath::Clamp(Settings.OrbitSensitivity + Delta, 0.2f, 3.f);
		CameraPawn->ApplyUserSettings(Settings);
		return true;
	}
	if (Setting == TEXT("zoom"))
	{
		FApartmentLifeCameraUserSettings Settings = CameraPawn->BuildUserSettings();
		Settings.ZoomSensitivity = FMath::Clamp(Settings.ZoomSensitivity + Delta, 0.2f, 3.f);
		CameraPawn->ApplyUserSettings(Settings);
		return true;
	}
	if (Setting == TEXT("pan"))
	{
		FApartmentLifeCameraUserSettings Settings = CameraPawn->BuildUserSettings();
		Settings.PanSensitivity = FMath::Clamp(Settings.PanSensitivity + Delta, 0.2f, 3.f);
		CameraPawn->ApplyUserSettings(Settings);
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::ApplyAnimationPreviewControls(
	AActor* Character,
	const FApartmentLifeDeveloperAnimationPreviewState& State)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (!Character)
	{
		return false;
	}

	if (!State.bPlaying)
	{
		UApartmentLifeDevToolsLibrary::ResetAnimationState(Character);
		return true;
	}

	if (!State.AnimationGroupId.IsNone())
	{
		UApartmentLifeDevToolsLibrary::PreviewAnimation(Character, State.AnimationGroupId);
	}
	if (!State.MontageId.IsNone())
	{
		UApartmentLifeDevToolsLibrary::PreviewMontage(Character, State.MontageId);
	}
	UApartmentLifeDevToolsLibrary::ForceFacialExpression(Character, State.FacialExpression);
	return true;
#endif
}

bool UApartmentLifeDeveloperLibrary::EnterScreenshotStudio(APlayerController* Controller, bool bHideUi)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (AApartmentLifeCameraPawn* CameraPawn = Controller ? Cast<AApartmentLifeCameraPawn>(Controller->GetPawn()) : nullptr)
	{
		CameraPawn->EnterPhotoMode();
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::TakeStudioScreenshot(APlayerController* Controller)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	if (AApartmentLifeCameraPawn* CameraPawn = Controller ? Cast<AApartmentLifeCameraPawn>(Controller->GetPawn()) : nullptr)
	{
		CameraPawn->TakeHighResScreenshot();
		return true;
	}
	return false;
#endif
}

bool UApartmentLifeDeveloperLibrary::AdvanceGameDay(UObject* WorldContextObject)
{
#if UE_BUILD_SHIPPING
	return false;
#else
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return false;
	}

	if (UApartmentLifeGameTimeSubsystem* Time = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
	{
		FApartmentLifeGameTime Current = Time->GetCurrentTime();
		Current.Day += 1;
		Current.Hour = 8;
		Current.Minute = 0;
		Time->SetCurrentTime(Current);
		return true;
	}
	return false;
#endif
}

FText UApartmentLifeDeveloperLibrary::GetHubSectionLabel(EApartmentLifeDeveloperHubSection Section)
{
	const UEnum* Enum = StaticEnum<EApartmentLifeDeveloperHubSection>();
	return Enum ? Enum->GetDisplayNameTextByValue(static_cast<int64>(Section)) : FText::GetEmpty();
}

FText UApartmentLifeDeveloperLibrary::GetContentCategoryLabel(EApartmentLifeDeveloperContentCategory Category)
{
	const UEnum* Enum = StaticEnum<EApartmentLifeDeveloperContentCategory>();
	return Enum ? Enum->GetDisplayNameTextByValue(static_cast<int64>(Category)) : FText::GetEmpty();
}

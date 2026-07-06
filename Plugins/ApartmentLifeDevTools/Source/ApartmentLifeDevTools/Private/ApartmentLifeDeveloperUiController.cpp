// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDeveloperUiController.h"
#include "ApartmentLifeDeveloperSubsystem.h"
#include "ApartmentLifeDeveloperLibrary.h"
#include "ApartmentLifeApartmentUnit.h"
#include "Engine/GameInstance.h"

void UApartmentLifeDeveloperUiController::InitializeContext(
	APlayerController* InController,
	AApartmentLifeApartmentUnit* InApartment,
	AActor* InGirlCharacter)
{
	OwnerController = InController;
	ApartmentUnit = InApartment;
	GirlCharacter = InGirlCharacter;
}

void UApartmentLifeDeveloperUiController::OpenDeveloperHub()
{
	bIsOpen = true;
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->SetDeveloperModeEnabled(true);
			Dev->GetMutableHubState().ActiveSection = EApartmentLifeDeveloperHubSection::Root;
		}
	}
	RefreshContentList();
	BroadcastStateChanged();
}

void UApartmentLifeDeveloperUiController::CloseDeveloperHub()
{
	bIsOpen = false;
	BroadcastStateChanged();
}

void UApartmentLifeDeveloperUiController::ToggleDeveloperHub()
{
	if (bIsOpen)
	{
		CloseDeveloperHub();
	}
	else
	{
		OpenDeveloperHub();
	}
}

void UApartmentLifeDeveloperUiController::NavigateToSection(EApartmentLifeDeveloperHubSection Section)
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->GetMutableHubState().ActiveSection = Section;
			if (Section == EApartmentLifeDeveloperHubSection::ContentBrowser)
			{
				RefreshContentList();
			}
		}
	}
	BroadcastStateChanged();
}

void UApartmentLifeDeveloperUiController::SetContentCategory(EApartmentLifeDeveloperContentCategory Category)
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->GetMutableHubState().ContentCategory = Category;
		}
	}
	RefreshContentList();
}

void UApartmentLifeDeveloperUiController::SetSearchQuery(const FString& Query)
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->GetMutableHubState().SearchQuery = Query;
		}
	}
	RefreshContentList();
}

void UApartmentLifeDeveloperUiController::ToggleFavoritesOnly()
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			FApartmentLifeDeveloperHubState& State = Dev->GetMutableHubState();
			State.bFavoritesOnly = !State.bFavoritesOnly;
		}
	}
	RefreshContentList();
}

void UApartmentLifeDeveloperUiController::SelectContentIndex(int32 Index)
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			Dev->GetMutableHubState().SelectedContentIndex = VisibleEntries.IsValidIndex(Index) ? Index : INDEX_NONE;
		}
	}
	BroadcastStateChanged();
}

void UApartmentLifeDeveloperUiController::ActivateEntryIndex(int32 Index)
{
	if (!VisibleEntries.IsValidIndex(Index))
	{
		return;
	}

	UApartmentLifeDeveloperLibrary::PreviewContentEntry(
		OwnerController.Get(),
		GirlCharacter.Get(),
		ApartmentUnit.Get(),
		VisibleEntries[Index]);
	BroadcastStateChanged();
}

EApartmentLifeDeveloperHubSection UApartmentLifeDeveloperUiController::GetActiveSection() const
{
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (const UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>())
		{
			return Dev->GetHubState().ActiveSection;
		}
	}
	return EApartmentLifeDeveloperHubSection::Root;
}

void UApartmentLifeDeveloperUiController::RefreshContentList()
{
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!GI)
	{
		VisibleEntries.Empty();
		return;
	}

	UApartmentLifeDeveloperSubsystem* Dev = GI->GetSubsystem<UApartmentLifeDeveloperSubsystem>();
	if (!Dev)
	{
		VisibleEntries.Empty();
		return;
	}

	const FApartmentLifeDeveloperHubState& State = Dev->GetHubState();
	VisibleEntries = UApartmentLifeDeveloperLibrary::QueryContentBrowser(
		State.ContentCategory,
		State.SearchQuery,
		State.bFavoritesOnly,
		Dev->GetFavoriteContentIds());
}

void UApartmentLifeDeveloperUiController::BroadcastStateChanged()
{
	OnDeveloperUiStateChanged.Broadcast();
}

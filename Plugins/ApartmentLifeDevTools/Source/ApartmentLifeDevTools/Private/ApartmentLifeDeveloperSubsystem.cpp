// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDeveloperSubsystem.h"
#include "ApartmentLifeDeveloperLibrary.h"

void UApartmentLifeDeveloperSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DashboardCategories = UApartmentLifeDeveloperLibrary::BuildDefaultDashboard();
}

void UApartmentLifeDeveloperSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UApartmentLifeDeveloperSubsystem::SetDeveloperModeEnabled(bool bEnabled)
{
	if (bDeveloperModeEnabled == bEnabled)
	{
		return;
	}

	bDeveloperModeEnabled = bEnabled;
	OnDeveloperModeChanged.Broadcast();
}

void UApartmentLifeDeveloperSubsystem::ToggleDeveloperMode()
{
	SetDeveloperModeEnabled(!bDeveloperModeEnabled);
}

void UApartmentLifeDeveloperSubsystem::SetPerformanceOverlayVisible(bool bVisible)
{
	bPerformanceOverlayVisible = bVisible;
}

void UApartmentLifeDeveloperSubsystem::TogglePerformanceOverlay()
{
	bPerformanceOverlayVisible = !bPerformanceOverlayVisible;
}

void UApartmentLifeDeveloperSubsystem::TickPerformance(
	float DeltaSeconds,
	UObject* WorldContextObject,
	AActor* Character,
	int32 SaveSlotIndex)
{
	PerformanceAccumulator += DeltaSeconds;
	if (PerformanceAccumulator < 0.25f)
	{
		return;
	}

	PerformanceAccumulator = 0.f;
	PerformanceSnapshot = UApartmentLifeDeveloperLibrary::CapturePerformanceSnapshot(
		WorldContextObject, Character, SaveSlotIndex);
}

void UApartmentLifeDeveloperSubsystem::SetDashboardNote(int32 CategoryIndex, const FString& Notes)
{
	if (DashboardCategories.IsValidIndex(CategoryIndex))
	{
		DashboardCategories[CategoryIndex].Notes = Notes;
	}
}

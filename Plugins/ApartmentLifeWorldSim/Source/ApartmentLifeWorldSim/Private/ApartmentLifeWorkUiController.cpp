// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeWorkUiController.h"
#include "ApartmentLifeWorkLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeUiOverlayGate.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeProgressionComponent.h"
#include "Engine/Engine.h"

void UApartmentLifeWorkUiController::InitializeContext(
	UApartmentLifeNPCSimulationComponent* InSimulation,
	UApartmentLifeProgressionComponent* InProgression,
	UApartmentLifeActivityComponent* InActivity)
{
	Simulation = InSimulation;
	Progression = InProgression;
	Activity = InActivity;

	if (Activity.IsValid())
	{
		Activity->OnActivityCompletedWithResult.AddDynamic(this, &UApartmentLifeWorkUiController::HandleActivityCompletedWithResult);
	}

	if (Progression.IsValid())
	{
		Progression->OnWorkSessionCompleted.AddDynamic(this, &UApartmentLifeWorkUiController::HandleWorkSessionCompleted);
	}
}

void UApartmentLifeWorkUiController::OpenWorkSelection()
{
	bWorkMenuOpen = true;
	bShowingResult = false;
	VisibleWorkTypes = UApartmentLifeWorkLibrary::GetAvailableWorkTypes(Simulation.Get(), Progression.Get());
	SelectedWorkIndex = VisibleWorkTypes.Num() > 0 ? 0 : INDEX_NONE;
	RefreshOverlay();
}

void UApartmentLifeWorkUiController::CloseWorkSelection()
{
	bWorkMenuOpen = false;
	bShowingResult = false;
	RefreshOverlay();
}

void UApartmentLifeWorkUiController::SelectWorkIndex(int32 Index)
{
	SelectedWorkIndex = VisibleWorkTypes.IsValidIndex(Index) ? Index : INDEX_NONE;
	RefreshOverlay();
}

bool UApartmentLifeWorkUiController::StartSelectedWork()
{
	if (!Activity.IsValid() || !VisibleWorkTypes.IsValidIndex(SelectedWorkIndex))
	{
		return false;
	}

	const FApartmentLifeWorkTypeDefinition& Work = VisibleWorkTypes[SelectedWorkIndex];
	return Activity->StartActivity(Work.ActivityId, EApartmentLifeRoomType::Office);
}

void UApartmentLifeWorkUiController::ShowWorkResult(const FApartmentLifeWorkSessionResult& Result)
{
	LastResult = Result;
	bShowingResult = true;
	RefreshOverlay();
}

void UApartmentLifeWorkUiController::HandleWorkSessionCompleted(const FApartmentLifeWorkSessionResult& Result)
{
	ShowWorkResult(Result);
}

void UApartmentLifeWorkUiController::HandleActivityCompletedWithResult(FName ActivityId, const FApartmentLifeActivityCompletionResult& Result)
{
	(void)ActivityId;
	(void)Result;
}

void UApartmentLifeWorkUiController::RefreshOverlay() const
{
	if (FApartmentLifeUiOverlayGate::bSuppressDebugOverlays || !GEngine)
	{
		return;
	}

	if (bShowingResult)
	{
		GEngine->AddOnScreenDebugMessage(9000, 5.f, FColor::Green,
			FString::Printf(TEXT("Work Complete — Earned $%.0f (bonus $%.0f) | XP +%.1f | Stress +%.0f"),
				LastResult.TotalIncome, LastResult.BonusIncome, LastResult.SkillXpGained, LastResult.StressChange));
		if (LastResult.NewUnlocks.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(9001, 5.f, FColor::Yellow, TEXT("New unlocks available!"));
		}
		return;
	}

	if (!bWorkMenuOpen)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(9000, 0.f, FColor::Cyan, TEXT("Computer Work — select task:"));

	for (int32 Index = 0; Index < VisibleWorkTypes.Num(); ++Index)
	{
		const FApartmentLifeWorkTypeDefinition& Work = VisibleWorkTypes[Index];
		GEngine->AddOnScreenDebugMessage(9010 + Index, 0.f,
			Index == SelectedWorkIndex ? FColor::Yellow : FColor::White,
			FString::Printf(TEXT("  [%d] %s — $%.0f base, %d min"),
				Index + 1, *Work.DisplayName.ToString(), Work.BasePayout, Work.DurationMinutes));
	}

	if (Simulation.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(9090, 0.f, FColor::Silver,
			FString::Printf(TEXT("Savings: $%.0f | Energy: %.0f | Stress: %.0f"),
				Simulation->Finance.Savings, Simulation->Mood.Energy, Simulation->Mood.Stress));
	}
}

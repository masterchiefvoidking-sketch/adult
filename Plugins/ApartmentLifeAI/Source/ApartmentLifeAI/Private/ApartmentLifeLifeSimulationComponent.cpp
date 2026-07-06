// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeLifeSimulationComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"

UApartmentLifeLifeSimulationComponent::UApartmentLifeLifeSimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UApartmentLifeLifeSimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnMinuteAdvanced.AddDynamic(this, &UApartmentLifeLifeSimulationComponent::HandleMinuteAdvanced);
			EvaluateSchedule(TimeSubsystem->GetCurrentTime());
		}
	}
}

void UApartmentLifeLifeSimulationComponent::HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime)
{
	TickLifeSimulation(NewTime);
}

void UApartmentLifeLifeSimulationComponent::TickLifeSimulation(const FApartmentLifeGameTime& CurrentTime)
{
	EvaluateSchedule(CurrentTime);
	SimulateDailyEconomy(CurrentTime);
}

void UApartmentLifeLifeSimulationComponent::EvaluateSchedule(const FApartmentLifeGameTime& CurrentTime)
{
	FName BestActivity = NAME_None;

	for (const FApartmentLifeScheduleEntry& Entry : WeeklySchedule)
	{
		if (CurrentTime.Hour >= Entry.StartHour && CurrentTime.Hour < Entry.EndHour)
		{
			BestActivity = Entry.ActivityId;
			break;
		}
	}

	if (BestActivity != CurrentActivityId)
	{
		CurrentActivityId = BestActivity;
		OnScheduleActivityChanged.Broadcast(CurrentActivityId);
	}
}

void UApartmentLifeLifeSimulationComponent::SimulateDailyEconomy(const FApartmentLifeGameTime& CurrentTime)
{
	if (CurrentTime.Day == LastProcessedDay)
	{
		return;
	}

	LastProcessedDay = CurrentTime.Day;

	if (CurrentTime.Day == 1)
	{
		PayMonthlyBills();
	}

	const float DailyIncome = Economy.MonthlyIncome / 30.f;
	Economy.Savings += DailyIncome;
}

void UApartmentLifeLifeSimulationComponent::PayMonthlyBills()
{
	Economy.Savings -= Economy.MonthlyBills;
	Economy.Savings = FMath::Max(Economy.Savings, -500.f);
}

void UApartmentLifeLifeSimulationComponent::AddMemory(const FApartmentLifeMemoryEntry& Memory)
{
	Memories.Add(Memory);
	if (Memories.Num() > 128)
	{
		Memories.RemoveAt(0);
	}
}

void UApartmentLifeLifeSimulationComponent::AdjustRelationship(FName OtherCharacterId, float AffinityDelta, float TrustDelta)
{
	for (FApartmentLifeRelationshipState& Relationship : Relationships)
	{
		if (Relationship.OtherCharacterId == OtherCharacterId)
		{
			Relationship.Affinity = FMath::Clamp(Relationship.Affinity + AffinityDelta, -100.f, 100.f);
			Relationship.Trust = FMath::Clamp(Relationship.Trust + TrustDelta, 0.f, 100.f);
			return;
		}
	}

	FApartmentLifeRelationshipState NewRelationship;
	NewRelationship.OtherCharacterId = OtherCharacterId;
	NewRelationship.Affinity = FMath::Clamp(AffinityDelta, -100.f, 100.f);
	NewRelationship.Trust = FMath::Clamp(50.f + TrustDelta, 0.f, 100.f);
	Relationships.Add(NewRelationship);
}

float UApartmentLifeLifeSimulationComponent::ComputeLifestyleBudgetTier() const
{
	return FMath::Clamp(Economy.MonthlyIncome / 5000.f, 0.2f, 2.f);
}

FString UApartmentLifeLifeSimulationComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("life_%s"), *Owner->GetName());
	}
	return TEXT("life_unknown");
}

void UApartmentLifeLifeSimulationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("Savings"), FString::SanitizeFloat(Economy.Savings));
	OutData.Add(TEXT("CurrentActivity"), CurrentActivityId.ToString());
}

void UApartmentLifeLifeSimulationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Savings = InData.Find(TEXT("Savings")))
	{
		Economy.Savings = FCString::Atof(**Savings);
	}
	if (const FString* Activity = InData.Find(TEXT("CurrentActivity")))
	{
		CurrentActivityId = FName(**Activity);
	}
}

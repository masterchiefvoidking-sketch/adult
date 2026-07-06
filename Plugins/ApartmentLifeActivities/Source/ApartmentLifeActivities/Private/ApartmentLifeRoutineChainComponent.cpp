// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeRoutineChainComponent.h"
#include "ApartmentLifeActivityCatalogLibrary.h"
#include "ApartmentLifeActivityComponent.h"

bool UApartmentLifeRoutineChainComponent::StartRoutineChain(FName ChainId)
{
	FApartmentLifeRoutineChainDefinition Chain;
	if (!UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(ChainId, Chain) || Chain.ActivityIds.Num() == 0)
	{
		return false;
	}

	if (UApartmentLifeActivityComponent* Activity = GetActivityComponent())
	{
		Activity->OnActivityCompleted.RemoveDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
		Activity->OnActivityCompleted.AddDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
	}

	ActiveChain = Chain;
	CurrentChainId = ChainId;
	CurrentStepIndex = 0;
	bRoutineActive = true;
	StartCurrentStep();
	return true;
}

void UApartmentLifeRoutineChainComponent::AdvanceRoutineChain()
{
	if (!bRoutineActive)
	{
		return;
	}

	++CurrentStepIndex;
	if (!ActiveChain.ActivityIds.IsValidIndex(CurrentStepIndex))
	{
		CancelRoutineChain();
		OnRoutineChainCompleted.Broadcast();
		return;
	}

	StartCurrentStep();
}

void UApartmentLifeRoutineChainComponent::CancelRoutineChain()
{
	bRoutineActive = false;
	CurrentChainId = NAME_None;
	CurrentStepIndex = INDEX_NONE;
	ActiveChain = FApartmentLifeRoutineChainDefinition();

	if (UApartmentLifeActivityComponent* Activity = GetActivityComponent())
	{
		Activity->OnActivityCompleted.RemoveDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
	}
}

void UApartmentLifeRoutineChainComponent::ResumeAfterLoad()
{
	if (!bRoutineActive)
	{
		return;
	}

	if (UApartmentLifeActivityComponent* Activity = GetActivityComponent())
	{
		Activity->OnActivityCompleted.RemoveDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
		Activity->OnActivityCompleted.AddDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
	}

	StartCurrentStep();
}

FName UApartmentLifeRoutineChainComponent::GetCurrentStepActivityId() const
{
	return ActiveChain.ActivityIds.IsValidIndex(CurrentStepIndex) ? ActiveChain.ActivityIds[CurrentStepIndex] : NAME_None;
}

void UApartmentLifeRoutineChainComponent::StartCurrentStep()
{
	if (!ActiveChain.ActivityIds.IsValidIndex(CurrentStepIndex))
	{
		return;
	}

	if (UApartmentLifeActivityComponent* Activity = GetActivityComponent())
	{
		Activity->StartActivity(ActiveChain.ActivityIds[CurrentStepIndex]);
	}

	OnRoutineChainStepChanged.Broadcast(CurrentStepIndex);
}

UApartmentLifeActivityComponent* UApartmentLifeRoutineChainComponent::GetActivityComponent() const
{
	if (const AActor* Owner = GetOwner())
	{
		return Owner->FindComponentByClass<UApartmentLifeActivityComponent>();
	}
	return nullptr;
}

void UApartmentLifeRoutineChainComponent::HandleActivityCompleted(FName ActivityId)
{
	if (!bRoutineActive || ActivityId != GetCurrentStepActivityId())
	{
		return;
	}

	AdvanceRoutineChain();
}

FString UApartmentLifeRoutineChainComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("routine_%s"), *Owner->GetName());
	return TEXT("routine_unknown");
}

void UApartmentLifeRoutineChainComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("bRoutineActive"), bRoutineActive ? TEXT("1") : TEXT("0"));
	OutData.Add(TEXT("CurrentChainId"), CurrentChainId.ToString());
	OutData.Add(TEXT("CurrentStepIndex"), FString::FromInt(CurrentStepIndex));
}

void UApartmentLifeRoutineChainComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Active = InData.Find(TEXT("bRoutineActive"))) bRoutineActive = *Active == TEXT("1");
	if (const FString* ChainId = InData.Find(TEXT("CurrentChainId"))) CurrentChainId = FName(**ChainId);
	if (const FString* Step = InData.Find(TEXT("CurrentStepIndex"))) CurrentStepIndex = FCString::Atoi(**Step);

	if (bRoutineActive && UApartmentLifeActivityCatalogLibrary::TryGetRoutineChain(CurrentChainId, ActiveChain))
	{
		if (UApartmentLifeActivityComponent* Activity = GetActivityComponent())
		{
			Activity->OnActivityCompleted.RemoveDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
			Activity->OnActivityCompleted.AddDynamic(this, &UApartmentLifeRoutineChainComponent::HandleActivityCompleted);
		}
	}
	else
	{
		bRoutineActive = false;
	}
}

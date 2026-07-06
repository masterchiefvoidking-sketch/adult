// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeContentSettings.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeActivityFallbackLibrary.h"
#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

void UApartmentLifeActivityComponent::HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime)
{
	TArray<FName> ExpiredCooldowns;
	for (TPair<FName, int32>& Pair : CooldownRemainingMinutes)
	{
		--Pair.Value;
		if (Pair.Value <= 0)
		{
			ExpiredCooldowns.Add(Pair.Key);
		}
	}
	for (const FName& Id : ExpiredCooldowns)
	{
		CooldownRemainingMinutes.Remove(Id);
	}

	if (!bActivityActive)
	{
		return;
	}

	--RemainingMinutes;
	if (RemainingMinutes <= 0)
	{
		CompleteActivity();
	}
}

bool UApartmentLifeActivityComponent::StartActivity(FName ActivityId, EApartmentLifeRoomType CurrentRoom)
{
	if (ActivityId.IsNone() || !CanStartActivity(ActivityId, CurrentRoom))
	{
		return false;
	}

	if (bActivityActive)
	{
		CancelCurrentActivity();
	}

	CurrentActivityId = ActivityId;
	bActivityActive = true;
	RemainingMinutes = UApartmentLifeActivityLibrary::GetDurationMinutes(ActivityId);

	if (UApartmentLifeActivityData* Activity = ResolveActivity(ActivityId))
	{
		RemainingMinutes = Activity->DurationMinutes;
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnMinuteAdvanced.AddDynamic(this, &UApartmentLifeActivityComponent::HandleMinuteAdvanced);
		}
	}

	OnActivityStarted.Broadcast(CurrentActivityId);
	return true;
}

void UApartmentLifeActivityComponent::CancelCurrentActivity()
{
	if (!bActivityActive)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnMinuteAdvanced.RemoveDynamic(this, &UApartmentLifeActivityComponent::HandleMinuteAdvanced);
		}
	}

	bActivityActive = false;
	CurrentActivityId = NAME_None;
	RemainingMinutes = 0;
}

void UApartmentLifeActivityComponent::CompleteActivity()
{
	const FName CompletedId = CurrentActivityId;
	CancelCurrentActivity();
	RecordCooldown(CompletedId);

	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeNPCSimulationComponent* Simulation = Owner->FindComponentByClass<UApartmentLifeNPCSimulationComponent>())
		{
			LastCompletionResult = UApartmentLifeActivityLibrary::ApplyActivityCompletion(Simulation, CompletedId);
		}
		else
		{
			LastCompletionResult.ActivityId = CompletedId;
			LastCompletionResult.bSuccess = true;
		}
	}

	OnActivityCompleted.Broadcast(CompletedId);
	OnActivityCompletedWithResult.Broadcast(CompletedId, LastCompletionResult);
}

bool UApartmentLifeActivityComponent::IsCooldownReady(FName ActivityId) const
{
	const int32* Remaining = CooldownRemainingMinutes.Find(ActivityId);
	return !Remaining || *Remaining <= 0;
}

UApartmentLifeActivityData* UApartmentLifeActivityComponent::ResolveActivity(FName ActivityId) const
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				return Cast<UApartmentLifeActivityData>(Registry->FindAsset(ActivityId));
			}
		}
	}
	return nullptr;
}

bool UApartmentLifeActivityComponent::CanStartActivity(FName ActivityId, EApartmentLifeRoomType CurrentRoom) const
{
	if (UApartmentLifeActivityData* Activity = ResolveActivity(ActivityId))
	{
		if (Activity->bRequiresExplicitContent && !UApartmentLifeContentSettings::IsExplicitContentAllowed())
		{
			return false;
		}
	}

	if (!UApartmentLifeActivityLibrary::CanStartActivity(ActivityId, CurrentRoom, this))
	{
		return false;
	}

	return true;
}

void UApartmentLifeActivityComponent::RecordCooldown(FName ActivityId)
{
	FApartmentLifeActivityDefinition Def;
	if (UApartmentLifeActivityLibrary::TryGetDefinition(ActivityId, Def) && Def.CooldownMinutes > 0)
	{
		CooldownRemainingMinutes.Add(ActivityId, Def.CooldownMinutes);
	}
}

FString UApartmentLifeActivityComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("activity_%s"), *Owner->GetName());
	return TEXT("activity_unknown");
}

void UApartmentLifeActivityComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("bActivityActive"), bActivityActive ? TEXT("1") : TEXT("0"));
	OutData.Add(TEXT("CurrentActivityId"), CurrentActivityId.ToString());
	OutData.Add(TEXT("RemainingMinutes"), FString::FromInt(RemainingMinutes));

	TArray<FString> CooldownPairs;
	for (const TPair<FName, int32>& Pair : CooldownRemainingMinutes)
	{
		CooldownPairs.Add(FString::Printf(TEXT("%s:%d"), *Pair.Key.ToString(), Pair.Value));
	}
	OutData.Add(TEXT("Cooldowns"), FString::Join(CooldownPairs, TEXT(",")));
}

void UApartmentLifeActivityComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Active = InData.Find(TEXT("bActivityActive"))) bActivityActive = *Active == TEXT("1");
	if (const FString* ActivityId = InData.Find(TEXT("CurrentActivityId"))) CurrentActivityId = FName(**ActivityId);
	if (const FString* Remaining = InData.Find(TEXT("RemainingMinutes"))) RemainingMinutes = FCString::Atoi(**Remaining);

	CooldownRemainingMinutes.Empty();
	if (const FString* Cooldowns = InData.Find(TEXT("Cooldowns")))
	{
		TArray<FString> Pairs;
		Cooldowns->ParseIntoArray(Pairs, TEXT(","), true);
		for (const FString& Pair : Pairs)
		{
			FString IdStr, ValStr;
			if (Pair.Split(TEXT(":"), &IdStr, &ValStr))
			{
				CooldownRemainingMinutes.Add(FName(*IdStr), FCString::Atoi(*ValStr));
			}
		}
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnMinuteAdvanced.RemoveDynamic(this, &UApartmentLifeActivityComponent::HandleMinuteAdvanced);
			if (bActivityActive && !CurrentActivityId.IsNone())
			{
				TimeSubsystem->OnMinuteAdvanced.AddDynamic(this, &UApartmentLifeActivityComponent::HandleMinuteAdvanced);
			}
		}
	}

	if (!bActivityActive)
	{
		CurrentActivityId = NAME_None;
		RemainingMinutes = 0;
	}
}

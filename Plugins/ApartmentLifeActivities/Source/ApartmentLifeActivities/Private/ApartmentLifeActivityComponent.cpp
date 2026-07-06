// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeContentSettings.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeActivityFallbackLibrary.h"

void UApartmentLifeActivityComponent::HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime)
{
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

bool UApartmentLifeActivityComponent::StartActivity(FName ActivityId)
{
	if (ActivityId.IsNone())
	{
		return false;
	}

	UApartmentLifeActivityData* Activity = ResolveActivity(ActivityId);
	if (!Activity)
	{
		if (bActivityActive)
		{
			CancelCurrentActivity();
		}

		CurrentActivityId = ActivityId;
		bActivityActive = true;
		RemainingMinutes = FMath::Max(1, UApartmentLifeActivityFallbackLibrary::GetBuiltinDurationMinutes(ActivityId));

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

	if (!CanStartActivity(Activity))
	{
		return false;
	}

	if (bActivityActive)
	{
		CancelCurrentActivity();
	}

	CurrentActivityId = ActivityId;
	bActivityActive = true;
	RemainingMinutes = Activity->DurationMinutes;

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
	OnActivityCompleted.Broadcast(CompletedId);
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

bool UApartmentLifeActivityComponent::CanStartActivity(const UApartmentLifeActivityData* Activity) const
{
	if (!Activity)
	{
		return false;
	}

	if (Activity->bRequiresExplicitContent && !UApartmentLifeContentSettings::IsExplicitContentAllowed())
	{
		return false;
	}

	return true;
}

FString UApartmentLifeActivityComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("activity_%s"), *Owner->GetName());
	}
	return TEXT("activity_unknown");
}

void UApartmentLifeActivityComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("bActivityActive"), bActivityActive ? TEXT("1") : TEXT("0"));
	OutData.Add(TEXT("CurrentActivityId"), CurrentActivityId.ToString());
	OutData.Add(TEXT("RemainingMinutes"), FString::FromInt(RemainingMinutes));
}

void UApartmentLifeActivityComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Active = InData.Find(TEXT("bActivityActive")))
	{
		bActivityActive = *Active == TEXT("1");
	}
	if (const FString* ActivityId = InData.Find(TEXT("CurrentActivityId")))
	{
		CurrentActivityId = FName(**ActivityId);
	}
	if (const FString* Remaining = InData.Find(TEXT("RemainingMinutes")))
	{
		RemainingMinutes = FCString::Atoi(**Remaining);
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

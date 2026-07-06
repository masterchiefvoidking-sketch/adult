// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "JsonObjectConverter.h"

bool UApartmentLifeConversationComponent::StartConversation(AActor* Partner, const FApartmentLifeDialogueContext& Context)
{
	if (!Partner || CurrentSession.bIsActive)
	{
		return false;
	}

	UApartmentLifeNPCSimulationComponent* PartnerSim = Partner->FindComponentByClass<UApartmentLifeNPCSimulationComponent>();
	if (!PartnerSim)
	{
		return false;
	}

	CurrentSession = FApartmentLifeConversationSession();
	CurrentSession.bIsActive = true;
	CurrentSession.InitiatorCharacterId = Context.SpeakerCharacterId;
	CurrentSession.PartnerCharacterId = PartnerSim->GetCharacterId();
	CurrentSession.Phase = EApartmentLifeConversationPhase::Greeting;
	CurrentPartner = Partner;
	return true;
}

bool UApartmentLifeConversationComponent::AdvanceConversation(const UApartmentLifeDialogueTopicData* Topic)
{
	if (!CurrentSession.bIsActive || !Topic)
	{
		return false;
	}

	UApartmentLifeNPCSimulationComponent* SpeakerSim = GetOwnerSimulation();
	if (!SpeakerSim)
	{
		return false;
	}

	FApartmentLifeDialogueContext Context;
	if (UWorld* World = GetWorld())
	{
		FApartmentLifeGameTime Time;
		FApartmentLifeWeatherState Weather;
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Time = TimeSubsystem->GetCurrentTime();
			Weather = TimeSubsystem->GetCurrentWeather();
		}

		if (UApartmentLifeNPCSimulationComponent* ListenerSim = GetPartnerSimulation())
		{
			Context = UApartmentLifeSocialLibrary::BuildDialogueContext(SpeakerSim, ListenerSim, Time, Weather, NAME_None);
		}
	}

	const FApartmentLifeDialogueLine Line = UApartmentLifeSocialLibrary::GenerateDialogueLine(Topic, Context, CurrentSession.Phase);
	CurrentSession.Transcript.Add(Line);
	CurrentSession.TopicsDiscussed.AddUnique(Topic->Topic);
	RecentlyDiscussedTopics.AddUnique(Topic->Topic);
	OnConversationLineSpoken.Broadcast(Line);

	if (CurrentSession.Phase == EApartmentLifeConversationPhase::Greeting)
	{
		CurrentSession.Phase = EApartmentLifeConversationPhase::Topic;
	}
	else if (CurrentSession.Phase == EApartmentLifeConversationPhase::Topic)
	{
		CurrentSession.Phase = EApartmentLifeConversationPhase::Reaction;
	}
	else
	{
		CurrentSession.Phase = EApartmentLifeConversationPhase::Closing;
	}

	CurrentSession.SessionQuality = UApartmentLifeSocialLibrary::ComputeConversationQuality(CurrentSession, Context);
	return true;
}

void UApartmentLifeConversationComponent::EndConversation()
{
	if (!CurrentSession.bIsActive)
	{
		return;
	}

	ApplyConversationResults(CurrentSession.SessionQuality);
	CurrentSession.bIsActive = false;
	OnConversationEnded.Broadcast(CurrentSession);
	CurrentSession = FApartmentLifeConversationSession();
	CurrentPartner = nullptr;
}

void UApartmentLifeConversationComponent::ApplyConversationResults(float Quality)
{
	UApartmentLifeNPCSimulationComponent* SpeakerSim = GetOwnerSimulation();
	if (!SpeakerSim)
	{
		return;
	}

	SpeakerSim->RecordConversationWith(CurrentSession.PartnerCharacterId, Quality);
	SpeakerSim->RecordMemory(EApartmentLifeMemoryCategory::Conversation, CurrentSession.PartnerCharacterId, FMath::Abs(Quality));
	SpeakerSim->ApplySocialFulfillment(FMath::Max(0.f, Quality * 10.f));

	if (Quality > 0.3f)
	{
		SpeakerSim->MoodInfluences.PersonalAchievement = FMath::Clamp(
			SpeakerSim->MoodInfluences.PersonalAchievement + Quality * 5.f, 0.f, 100.f);
	}

	FApartmentLifeConversationHistoryEntry HistoryEntry;
	HistoryEntry.PartnerCharacterId = CurrentSession.PartnerCharacterId;
	HistoryEntry.Topics = CurrentSession.TopicsDiscussed;
	HistoryEntry.Quality = Quality;
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			HistoryEntry.Timestamp = TimeSubsystem->GetCurrentTime();
		}
	}
	ConversationHistory.Add(HistoryEntry);
}

UApartmentLifeNPCSimulationComponent* UApartmentLifeConversationComponent::GetOwnerSimulation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return Owner->FindComponentByClass<UApartmentLifeNPCSimulationComponent>();
	}
	return nullptr;
}

UApartmentLifeNPCSimulationComponent* UApartmentLifeConversationComponent::GetPartnerSimulation() const
{
	if (CurrentPartner)
	{
		return CurrentPartner->FindComponentByClass<UApartmentLifeNPCSimulationComponent>();
	}
	return nullptr;
}

FString UApartmentLifeConversationComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("conversation_%s"), *Owner->GetName());
	}
	return TEXT("conversation_unknown");
}

void UApartmentLifeConversationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString HistoryJson;
	FJsonObjectConverter::UStructToJsonObjectString(ConversationHistory, HistoryJson);
	OutData.Add(TEXT("ConversationHistory"), HistoryJson);
}

void UApartmentLifeConversationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* HistoryJson = InData.Find(TEXT("ConversationHistory")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*HistoryJson, &ConversationHistory);
	}
}

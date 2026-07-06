// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"

FName UApartmentLifeSocialLibrary::GetStageId(EApartmentLifeRelationshipStage Stage)
{
	return FName(*FString::Printf(TEXT("stage.%s"), *UEnum::GetValueAsString(Stage)));
}

EApartmentLifeRelationshipStage UApartmentLifeSocialLibrary::GetStageFromRelationship(const FApartmentLifeRelationshipRecord& Relationship)
{
	if (Relationship.RomanceAttraction > 70.f && Relationship.Trust > 75.f)
	{
		return EApartmentLifeRelationshipStage::Partner;
	}
	if (Relationship.RomanceAttraction > 55.f && Relationship.Trust > 60.f)
	{
		return EApartmentLifeRelationshipStage::Dating;
	}
	if (Relationship.RomanceAttraction > 40.f && Relationship.Compatibility > 55.f)
	{
		return EApartmentLifeRelationshipStage::RomanticInterest;
	}
	if (Relationship.Friendship > 75.f && Relationship.Comfort > 65.f)
	{
		return EApartmentLifeRelationshipStage::CloseFriend;
	}
	if (Relationship.Friendship > 45.f)
	{
		return EApartmentLifeRelationshipStage::Friend;
	}
	if (Relationship.Familiarity > 25.f)
	{
		return EApartmentLifeRelationshipStage::Acquaintance;
	}
	return EApartmentLifeRelationshipStage::Stranger;
}

float UApartmentLifeSocialLibrary::ScoreTopicAffinity(
	const UApartmentLifeDialogueTopicData* Topic,
	const FApartmentLifeDialogueContext& Context,
	const TArray<FApartmentLifeConversationHistoryEntry>& History)
{
	if (!Topic)
	{
		return 0.f;
	}

	if (Context.Relationship.Friendship < Topic->MinFriendshipToDiscuss || Context.Relationship.Trust < Topic->MinTrustToDiscuss)
	{
		return 0.f;
	}

	float Score = 1.f;

	for (const FApartmentLifeConversationHistoryEntry& Entry : History)
	{
		if (Entry.PartnerCharacterId == Context.ListenerCharacterId && Entry.Topics.Contains(Topic->Topic))
		{
			Score *= 0.6f;
		}
	}

	if (Context.SpeakerMood.Happiness > 60.f)
	{
		Score += 0.2f;
	}

	if (Topic->Topic == EApartmentLifeConversationTopic::Work && !Context.CareerId.IsNone())
	{
		Score += Context.SpeakerPersonality.Ambition * 0.5f;
	}

	if (Topic->Topic == EApartmentLifeConversationTopic::Cooking)
	{
		Score += Context.SpeakerPersonality.CookingInterest;
	}

	if (Topic->Topic == EApartmentLifeConversationTopic::Games)
	{
		Score += Context.SpeakerPersonality.GamingInterest;
	}

	if (Topic->Topic == EApartmentLifeConversationTopic::Fitness)
	{
		Score += Context.SpeakerPersonality.FitnessInterest;
	}

	for (const FName& MemorySubject : Context.RecentMemorySubjects)
	{
		if (MemorySubject.ToString().Contains(UEnum::GetValueAsString(Topic->Topic)))
		{
			Score += 0.35f;
		}
	}

	return FMath::Clamp(Score, 0.f, 5.f);
}

EApartmentLifeConversationTopic UApartmentLifeSocialLibrary::SelectConversationTopic(
	const TArray<UApartmentLifeDialogueTopicData*>& AvailableTopics,
	const FApartmentLifeDialogueContext& Context,
	const TArray<FApartmentLifeConversationHistoryEntry>& History)
{
	float BestScore = 0.f;
	EApartmentLifeConversationTopic BestTopic = EApartmentLifeConversationTopic::Weather;

	for (const UApartmentLifeDialogueTopicData* Topic : AvailableTopics)
	{
		const float Score = ScoreTopicAffinity(Topic, Context, History);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestTopic = Topic->Topic;
		}
	}

	return BestTopic;
}

FApartmentLifeDialogueLine UApartmentLifeSocialLibrary::GenerateDialogueLine(
	const UApartmentLifeDialogueTopicData* Topic,
	const FApartmentLifeDialogueContext& Context,
	EApartmentLifeConversationPhase Phase)
{
	FApartmentLifeDialogueLine Line;
	if (!Topic)
	{
		return Line;
	}

	Line.Topic = Topic->Topic;
	Line.LineId = FName(*FString::Printf(TEXT("line.%s.%d"), *UEnum::GetValueAsString(Topic->Topic), FMath::RandRange(0, 999)));

	const TArray<FText>* Pool = &Topic->OpeningLines;
	if (Phase == EApartmentLifeConversationPhase::Reaction)
	{
		Pool = &Topic->ResponseLines;
	}
	else if (Phase == EApartmentLifeConversationPhase::Closing)
	{
		Pool = &Topic->ClosingLines;
	}

	if (Pool->Num() > 0)
	{
		Line.SpeakerText = (*Pool)[FMath::RandRange(0, Pool->Num() - 1)];
	}
	else
	{
		Line.SpeakerText = FText::FromString(FString::Printf(TEXT("Let's talk about %s."), *UEnum::GetValueAsString(Topic->Topic)));
	}

	if (Context.SpeakerMood.Stress > 60.f)
	{
		Line.EmotionTag = FName(TEXT("emotion.stressed"));
	}
	else if (Context.SpeakerMood.Happiness > 70.f)
	{
		Line.EmotionTag = FName(TEXT("emotion.happy"));
	}
	else
	{
		Line.EmotionTag = FName(TEXT("emotion.neutral"));
	}

	Line.RelationshipImpact = 0.1f + Context.SpeakerPersonality.Empathy * 0.1f;
	return Line;
}

float UApartmentLifeSocialLibrary::ComputeConversationQuality(
	const FApartmentLifeConversationSession& Session,
	const FApartmentLifeDialogueContext& Context)
{
	if (!Session.bIsActive && Session.Transcript.Num() == 0)
	{
		return 0.f;
	}

	float Quality = 0.5f;
	Quality += Session.TopicsDiscussed.Num() * 0.08f;
	Quality += Context.Relationship.Compatibility * 0.002f;
	Quality += Context.SpeakerMood.SocialBattery * 0.002f;
	Quality += Context.SpeakerPersonality.Humor * 0.15f;
	Quality -= Context.SpeakerMood.Stress * 0.002f;
	return FMath::Clamp(Quality, -1.f, 1.f);
}

FApartmentLifeRomanceProgress UApartmentLifeSocialLibrary::EvaluateRomanceProgression(
	const FApartmentLifeRelationshipRecord& Relationship,
	const FApartmentLifeRomanceProgress& Current,
	const UApartmentLifeRomanceMilestoneData* Milestone)
{
	FApartmentLifeRomanceProgress Updated = Current;
	Updated.Stage = GetStageFromRelationship(Relationship);
	Updated.Attraction = Relationship.RomanceAttraction;

	if (!Milestone)
	{
		return Updated;
	}

	const bool bMeetsRequirements =
		Updated.Stage >= Milestone->RequiredStage
		&& Relationship.Trust >= Milestone->MinTrust
		&& Relationship.Compatibility >= Milestone->MinCompatibility
		&& Updated.QualityConversationsCompleted >= Milestone->MinQualityConversations
		&& Relationship.SharedActivityCount >= Milestone->MinSharedActivities;

	if (bMeetsRequirements && !Updated.AchievedMilestones.Contains(Milestone->AssetId))
	{
		Updated.AchievedMilestones.Add(Milestone->AssetId);
	}

	return Updated;
}

FApartmentLifeReputationState UApartmentLifeSocialLibrary::ApplyReputationAction(
	const FApartmentLifeReputationState& Current,
	EApartmentLifeReputationAction Action)
{
	FApartmentLifeReputationState Updated = Current;
	int32& Count = Updated.ActionCounts.FindOrAdd(Action);
	++Count;

	switch (Action)
	{
	case EApartmentLifeReputationAction::KeptPromise:
	case EApartmentLifeReputationAction::ArrivedOnTime:
	case EApartmentLifeReputationAction::Reliable:
		Updated.Reliability = FMath::Clamp(Updated.Reliability + 4.f, 0.f, 100.f);
		break;
	case EApartmentLifeReputationAction::Considerate:
		Updated.Consideration = FMath::Clamp(Updated.Consideration + 4.f, 0.f, 100.f);
		break;
	case EApartmentLifeReputationAction::Generous:
	case EApartmentLifeReputationAction::HelpedSomeone:
		Updated.Generosity = FMath::Clamp(Updated.Generosity + 4.f, 0.f, 100.f);
		break;
	case EApartmentLifeReputationAction::CancelledPlans:
	case EApartmentLifeReputationAction::BrokePromise:
	case EApartmentLifeReputationAction::Unreliable:
		Updated.Reliability = FMath::Clamp(Updated.Reliability - 6.f, 0.f, 100.f);
		break;
	case EApartmentLifeReputationAction::Rude:
		Updated.Consideration = FMath::Clamp(Updated.Consideration - 8.f, 0.f, 100.f);
		break;
	default:
		break;
	}

	Updated.OverallReputation = (Updated.Reliability + Updated.Consideration + Updated.Generosity) / 3.f;
	return Updated;
}

bool UApartmentLifeSocialLibrary::ShouldAcceptInvitation(
	const FApartmentLifeSocialInvitation& Invitation,
	const FApartmentLifeRelationshipRecord& Relationship,
	const FApartmentLifeMoodState& Mood,
	const FApartmentLifePersonalityTraits& Personality)
{
	float Score = Relationship.GetInvitationScore();
	Score += Mood.SocialBattery * 0.002f;
	Score += Personality.Extroversion * 0.2f;
	Score -= Personality.Introversion * 0.15f;
	Score -= Mood.Stress * 0.002f;
	return Score > 0.45f;
}

FApartmentLifeDialogueContext UApartmentLifeSocialLibrary::BuildDialogueContext(
	UApartmentLifeNPCSimulationComponent* SpeakerSim,
	UApartmentLifeNPCSimulationComponent* ListenerSim,
	const FApartmentLifeGameTime& Time,
	const FApartmentLifeWeatherState& Weather,
	FName LocationTag)
{
	FApartmentLifeDialogueContext Context;
	if (!SpeakerSim || !ListenerSim)
	{
		return Context;
	}

	Context.SpeakerCharacterId = SpeakerSim->GetCharacterId();
	Context.ListenerCharacterId = ListenerSim->GetCharacterId();
	Context.SpeakerPersonality = SpeakerSim->GetPersonality();
	Context.SpeakerMood = SpeakerSim->GetMood();
	Context.CareerId = SpeakerSim->GetCareer().CareerId;
	Context.TimeOfDay = Time;
	Context.Weather = Weather;
	Context.LocationTag = LocationTag;

	SpeakerSim->GetRelationshipWith(ListenerSim->GetCharacterId(), Context.Relationship);

	for (const FApartmentLifeMemoryRecord& Memory : SpeakerSim->GetMemories())
	{
		if (Memory.SubjectId == ListenerSim->GetCharacterId() && Memory.Strength > 0.3f)
		{
			Context.RecentMemorySubjects.Add(Memory.SubjectId);
		}
	}

	return Context;
}

FApartmentLifeDialogueContext UApartmentLifeSocialLibrary::BuildPlayerDialogueContext(
	UApartmentLifeNPCSimulationComponent* GirlSim,
	const FApartmentLifeGameTime& Time,
	const FApartmentLifeWeatherState& Weather,
	FName LocationTag)
{
	FApartmentLifeDialogueContext Context;
	if (!GirlSim)
	{
		return Context;
	}

	Context.SpeakerCharacterId = GirlSim->GetCharacterId();
	Context.ListenerCharacterId = FName(TEXT("player"));
	Context.SpeakerPersonality = GirlSim->GetPersonality();
	Context.SpeakerMood = GirlSim->GetMood();
	Context.CareerId = GirlSim->GetCareer().CareerId;
	Context.TimeOfDay = Time;
	Context.Weather = Weather;
	Context.LocationTag = LocationTag;

	Context.Relationship.Friendship = GirlSim->GetAffectionTowardPlayer();
	Context.Relationship.Trust = GirlSim->TrustTowardPlayer;
	Context.Relationship.Comfort = GirlSim->GetMood().Comfort;
	Context.Relationship.RomanceAttraction = GirlSim->GetAffectionTowardPlayer() * 0.85f;
	Context.Relationship.Compatibility = (GirlSim->GetAffectionTowardPlayer() + GirlSim->TrustTowardPlayer) * 0.5f;

	return Context;
}

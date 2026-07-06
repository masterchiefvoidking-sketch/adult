// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeWorldSimTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeRelationshipStageTest,
	"ApartmentLife.Social.RelationshipStage",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeRelationshipStageTest::RunTest(const FString& Parameters)
{
	FApartmentLifeRelationshipRecord Record;
	Record.Friendship = 80.f;
	Record.Comfort = 70.f;
	TestEqual(TEXT("Close friend stage"),
		UApartmentLifeSocialLibrary::GetStageFromRelationship(Record),
		EApartmentLifeRelationshipStage::CloseFriend);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeReputationTest,
	"ApartmentLife.Social.Reputation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeReputationTest::RunTest(const FString& Parameters)
{
	FApartmentLifeReputationState State;
	const FApartmentLifeReputationState Updated =
		UApartmentLifeSocialLibrary::ApplyReputationAction(State, EApartmentLifeReputationAction::KeptPromise);
	TestTrue(TEXT("Reliability improves"), Updated.Reliability > State.Reliability);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeTopicAffinityTest,
	"ApartmentLife.Social.TopicAffinity",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeTopicAffinityTest::RunTest(const FString& Parameters)
{
	UApartmentLifeDialogueTopicData* Topic = NewObject<UApartmentLifeDialogueTopicData>();
	Topic->Topic = EApartmentLifeConversationTopic::Cooking;
	Topic->MinFriendshipToDiscuss = 0.f;

	FApartmentLifeDialogueContext Context;
	Context.SpeakerPersonality.CookingInterest = 0.9f;
	Context.Relationship.Friendship = 50.f;
	Context.Relationship.Trust = 50.f;

	const float Score = UApartmentLifeSocialLibrary::ScoreTopicAffinity(Topic, Context, {});
	TestTrue(TEXT("Cooking topic scores well for cook"), Score > 1.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeMemoryRecencyTest,
	"ApartmentLife.Social.MemoryRecency",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeMemoryRecencyTest::RunTest(const FString& Parameters)
{
	FApartmentLifeMemoryRecord Memory;
	Memory.Strength = 1.f;
	Memory.Timestamp.TotalMinutes = 0;

	FApartmentLifeGameTime Current;
	Current.TotalMinutes = 60 * 24 * 30;
	const float Weight = Memory.GetRecencyWeight(Current);
	TestTrue(TEXT("Old memory weighs less"), Weight < 1.f);
	return true;
}

#endif

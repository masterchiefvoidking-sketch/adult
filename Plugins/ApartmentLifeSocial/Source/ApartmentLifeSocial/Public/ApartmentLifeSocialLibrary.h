// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeSocialDataAssets.h"
#include "ApartmentLifeSocialLibrary.generated.h"

class UApartmentLifeNPCSimulationComponent;

UCLASS()
class APARTMENTLIFESOCIAL_API UApartmentLifeSocialLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Social")
	static FName GetStageId(EApartmentLifeRelationshipStage Stage);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Social")
	static EApartmentLifeRelationshipStage GetStageFromRelationship(const FApartmentLifeRelationshipRecord& Relationship);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static float ScoreTopicAffinity(
		const UApartmentLifeDialogueTopicData* Topic,
		const FApartmentLifeDialogueContext& Context,
		const TArray<FApartmentLifeConversationHistoryEntry>& History);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static EApartmentLifeConversationTopic SelectConversationTopic(
		const TArray<UApartmentLifeDialogueTopicData*>& AvailableTopics,
		const FApartmentLifeDialogueContext& Context,
		const TArray<FApartmentLifeConversationHistoryEntry>& History);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static FApartmentLifeDialogueLine GenerateDialogueLine(
		const UApartmentLifeDialogueTopicData* Topic,
		const FApartmentLifeDialogueContext& Context,
		EApartmentLifeConversationPhase Phase);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static float ComputeConversationQuality(
		const FApartmentLifeConversationSession& Session,
		const FApartmentLifeDialogueContext& Context);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static FApartmentLifeRomanceProgress EvaluateRomanceProgression(
		const FApartmentLifeRelationshipRecord& Relationship,
		const FApartmentLifeRomanceProgress& Current,
		const UApartmentLifeRomanceMilestoneData* Milestone);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static FApartmentLifeReputationState ApplyReputationAction(
		const FApartmentLifeReputationState& Current,
		EApartmentLifeReputationAction Action);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static bool ShouldAcceptInvitation(
		const FApartmentLifeSocialInvitation& Invitation,
		const FApartmentLifeRelationshipRecord& Relationship,
		const FApartmentLifeMoodState& Mood,
		const FApartmentLifePersonalityTraits& Personality);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	static FApartmentLifeDialogueContext BuildDialogueContext(
		UApartmentLifeNPCSimulationComponent* SpeakerSim,
		UApartmentLifeNPCSimulationComponent* ListenerSim,
		const FApartmentLifeGameTime& Time,
		const FApartmentLifeWeatherState& Weather,
		FName LocationTag);
};

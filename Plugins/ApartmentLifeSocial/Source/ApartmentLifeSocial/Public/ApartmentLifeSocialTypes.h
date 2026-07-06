// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeSocialTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeConversationTopic : uint8
{
	Work			UMETA(DisplayName = "Work"),
	School			UMETA(DisplayName = "School"),
	Movies			UMETA(DisplayName = "Movies"),
	Music			UMETA(DisplayName = "Music"),
	Games			UMETA(DisplayName = "Games"),
	Books			UMETA(DisplayName = "Books"),
	Cooking			UMETA(DisplayName = "Cooking"),
	Fitness			UMETA(DisplayName = "Fitness"),
	Travel			UMETA(DisplayName = "Travel"),
	Fashion			UMETA(DisplayName = "Fashion"),
	Technology		UMETA(DisplayName = "Technology"),
	Pets			UMETA(DisplayName = "Pets"),
	Food			UMETA(DisplayName = "Food"),
	Holidays		UMETA(DisplayName = "Holidays"),
	HomeDecorating	UMETA(DisplayName = "Home Decorating"),
	FuturePlans		UMETA(DisplayName = "Future Plans"),
	Weather			UMETA(DisplayName = "Weather"),
	Personal		UMETA(DisplayName = "Personal")
};

UENUM(BlueprintType)
enum class EApartmentLifeRelationshipStage : uint8
{
	Stranger			UMETA(DisplayName = "Stranger"),
	Acquaintance		UMETA(DisplayName = "Acquaintance"),
	Friend				UMETA(DisplayName = "Friend"),
	CloseFriend			UMETA(DisplayName = "Close Friend"),
	RomanticInterest	UMETA(DisplayName = "Romantic Interest"),
	Dating				UMETA(DisplayName = "Dating"),
	Partner				UMETA(DisplayName = "Partner")
};

UENUM(BlueprintType)
enum class EApartmentLifeSocialActivityType : uint8
{
	Coffee			UMETA(DisplayName = "Coffee"),
	Dinner			UMETA(DisplayName = "Dinner"),
	Shopping		UMETA(DisplayName = "Shopping"),
	ParkWalk		UMETA(DisplayName = "Park Walk"),
	Museum			UMETA(DisplayName = "Museum"),
	Concert			UMETA(DisplayName = "Concert"),
	Festival		UMETA(DisplayName = "Festival"),
	MovieNight		UMETA(DisplayName = "Movie Night"),
	CookingTogether	UMETA(DisplayName = "Cooking Together"),
	Decorating		UMETA(DisplayName = "Decorating"),
	Gaming			UMETA(DisplayName = "Gaming"),
	Yoga			UMETA(DisplayName = "Yoga"),
	Hiking			UMETA(DisplayName = "Hiking"),
	Exercise		UMETA(DisplayName = "Exercise")
};

UENUM(BlueprintType)
enum class EApartmentLifeReputationAction : uint8
{
	KeptPromise			UMETA(DisplayName = "Kept Promise"),
	ArrivedOnTime		UMETA(DisplayName = "Arrived On Time"),
	Considerate			UMETA(DisplayName = "Considerate"),
	HelpedSomeone		UMETA(DisplayName = "Helped Someone"),
	Generous			UMETA(DisplayName = "Generous"),
	Reliable			UMETA(DisplayName = "Reliable"),
	CancelledPlans		UMETA(DisplayName = "Cancelled Plans"),
	BrokePromise		UMETA(DisplayName = "Broke Promise"),
	Rude				UMETA(DisplayName = "Rude"),
	Unreliable			UMETA(DisplayName = "Unreliable")
};

UENUM(BlueprintType)
enum class EApartmentLifeSocialEventType : uint8
{
	Birthday			UMETA(DisplayName = "Birthday"),
	Holiday				UMETA(DisplayName = "Holiday"),
	Promotion			UMETA(DisplayName = "Promotion"),
	ApartmentParty		UMETA(DisplayName = "Apartment Party"),
	NeighborhoodEvent	UMETA(DisplayName = "Neighborhood Event"),
	SeasonalFestival	UMETA(DisplayName = "Seasonal Festival"),
	NewNeighbor			UMETA(DisplayName = "New Neighbor"),
	FurnitureSale		UMETA(DisplayName = "Furniture Sale"),
	HobbyClub			UMETA(DisplayName = "Hobby Club")
};

UENUM(BlueprintType)
enum class EApartmentLifeConversationPhase : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Greeting	UMETA(DisplayName = "Greeting"),
	Topic		UMETA(DisplayName = "Topic"),
	Reaction	UMETA(DisplayName = "Reaction"),
	Closing		UMETA(DisplayName = "Closing")
};

UENUM(BlueprintType)
enum class EApartmentLifeInvitationStatus : uint8
{
	Pending		UMETA(DisplayName = "Pending"),
	Accepted	UMETA(DisplayName = "Accepted"),
	Declined	UMETA(DisplayName = "Declined"),
	Completed	UMETA(DisplayName = "Completed"),
	Cancelled	UMETA(DisplayName = "Cancelled")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeDialogueContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName SpeakerCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName ListenerCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EApartmentLifeConversationTopic CurrentTopic = EApartmentLifeConversationTopic::Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FApartmentLifePersonalityTraits SpeakerPersonality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FApartmentLifeMoodState SpeakerMood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FApartmentLifeRelationshipRecord Relationship;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FApartmentLifeGameTime TimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FApartmentLifeWeatherState Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName LocationTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName CareerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FName> RecentMemorySubjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float SavingsSnapshot = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float ApartmentComfortScore = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName LineId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EApartmentLifeConversationTopic Topic = EApartmentLifeConversationTopic::Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName EmotionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float RelationshipImpact = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeConversationSession
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FName InitiatorCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FName PartnerCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	EApartmentLifeConversationPhase Phase = EApartmentLifeConversationPhase::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	TArray<EApartmentLifeConversationTopic> TopicsDiscussed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	TArray<FApartmentLifeDialogueLine> Transcript;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	float SessionQuality = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	bool bIsActive = false;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeReputationState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation", meta = (ClampMin = "0", ClampMax = "100"))
	float Reliability = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation", meta = (ClampMin = "0", ClampMax = "100"))
	float Consideration = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation", meta = (ClampMin = "0", ClampMax = "100"))
	float Generosity = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation", meta = (ClampMin = "0", ClampMax = "100"))
	float OverallReputation = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputation")
	TMap<EApartmentLifeReputationAction, int32> ActionCounts;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeSocialInvitation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	FGuid InvitationId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	FName InviterCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	FName InviteeCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	EApartmentLifeSocialActivityType ActivityType = EApartmentLifeSocialActivityType::Coffee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	EApartmentLifeInvitationStatus Status = EApartmentLifeInvitationStatus::Pending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invitation")
	FApartmentLifeGameTime ScheduledTime;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeConversationHistoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	FName PartnerCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	FApartmentLifeGameTime Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	TArray<EApartmentLifeConversationTopic> Topics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	float Quality = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFESOCIAL_API FApartmentLifeRomanceProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Romance")
	EApartmentLifeRelationshipStage Stage = EApartmentLifeRelationshipStage::Stranger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Romance")
	float Attraction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Romance")
	TArray<FName> AchievedMilestones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Romance")
	int32 QualityConversationsRequired = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Romance")
	int32 QualityConversationsCompleted = 0;
};

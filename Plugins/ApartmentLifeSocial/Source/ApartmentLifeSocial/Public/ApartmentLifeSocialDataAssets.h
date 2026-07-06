// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeSocialDataAssets.generated.h"

UCLASS(BlueprintType)
class APARTMENTLIFESOCIAL_API UApartmentLifeDialogueTopicData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("DialogueTopic")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	EApartmentLifeConversationTopic Topic = EApartmentLifeConversationTopic::Weather;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	float MinFriendshipToDiscuss = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	float MinTrustToDiscuss = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	TArray<FName> PersonalityAffinityTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	TArray<FText> OpeningLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	TArray<FText> ResponseLines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Topic")
	TArray<FText> ClosingLines;
};

UCLASS(BlueprintType)
class APARTMENTLIFESOCIAL_API UApartmentLifeSocialActivityInviteData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("SocialActivityInvite")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	EApartmentLifeSocialActivityType ActivityType = EApartmentLifeSocialActivityType::Coffee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	FName LinkedActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	float MinFriendshipRequired = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	float RelationshipGain = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	float MemoryEmotionalWeight = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	FText InvitationText;
};

UCLASS(BlueprintType)
class APARTMENTLIFESOCIAL_API UApartmentLifeRomanceMilestoneData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("RomanceMilestone")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	EApartmentLifeRelationshipStage RequiredStage = EApartmentLifeRelationshipStage::Friend;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	float MinTrust = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	float MinCompatibility = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	int32 MinQualityConversations = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	int32 MinSharedActivities = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Romance")
	FText MilestoneDescription;
};

UCLASS(BlueprintType)
class APARTMENTLIFESOCIAL_API UApartmentLifeSocialEventData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("SocialEvent")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	EApartmentLifeSocialEventType EventType = EApartmentLifeSocialEventType::Birthday;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	FName LinkedActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float MoodBoost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event")
	float RelationshipBoost = 5.f;
};

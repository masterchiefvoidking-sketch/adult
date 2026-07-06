// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeConversationComponent.generated.h"

class UApartmentLifeNPCSimulationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConversationLineSpoken, const FApartmentLifeDialogueLine&, Line);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConversationEnded, const FApartmentLifeConversationSession&, Session);

UCLASS(ClassGroup = Social, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFESOCIAL_API UApartmentLifeConversationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Conversation")
	bool StartConversation(AActor* Partner, const FApartmentLifeDialogueContext& Context);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Conversation")
	bool StartConversationWithPlayer();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Conversation")
	bool AdvanceConversation(const UApartmentLifeDialogueTopicData* Topic);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Conversation")
	void EndConversation();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Conversation")
	const FApartmentLifeConversationSession& GetCurrentSession() const { return CurrentSession; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Conversation")
	const TArray<FApartmentLifeConversationHistoryEntry>& GetConversationHistory() const { return ConversationHistory; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Conversation")
	FOnConversationLineSpoken OnConversationLineSpoken;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Conversation")
	FOnConversationEnded OnConversationEnded;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void ApplyConversationResults(float Quality);
	UApartmentLifeNPCSimulationComponent* GetOwnerSimulation() const;
	UApartmentLifeNPCSimulationComponent* GetPartnerSimulation() const;

	UPROPERTY()
	TObjectPtr<AActor> CurrentPartner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conversation")
	FApartmentLifeConversationSession CurrentSession;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conversation")
	TArray<FApartmentLifeConversationHistoryEntry> ConversationHistory;

	UPROPERTY()
	TArray<EApartmentLifeConversationTopic> RecentlyDiscussedTopics;

	bool bPlayerConversation = false;
};

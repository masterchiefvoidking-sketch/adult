// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeSocialSubsystem.generated.h"

class UApartmentLifeNPCSimulationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialInvitationIssued, const FApartmentLifeSocialInvitation&, Invitation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialEventTriggered, FName, EventId);

UCLASS()
class APARTMENTLIFESOCIAL_API UApartmentLifeSocialSubsystem : public UGameInstanceSubsystem, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	void RegisterSimCharacter(AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	void ProcessSocialAutonomy(const FApartmentLifeGameTime& CurrentTime);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	bool IssueInvitation(FName InviterId, FName InviteeId, EApartmentLifeSocialActivityType ActivityType, FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	bool RespondToInvitation(const FGuid& InvitationId, bool bAccept);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Social")
	void RecordReputationAction(FName CharacterId, EApartmentLifeReputationAction Action);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Social")
	const FApartmentLifeReputationState& GetReputation(FName CharacterId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Social")
	const TArray<FApartmentLifeSocialInvitation>& GetPendingInvitations() const { return PendingInvitations; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Social")
	FOnSocialInvitationIssued OnSocialInvitationIssued;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Social")
	FOnSocialEventTriggered OnSocialEventTriggered;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UFUNCTION()
	void HandleHourAdvanced(const FApartmentLifeGameTime& CurrentTime);

	UApartmentLifeNPCSimulationComponent* FindSimByCharacterId(FName CharacterId) const;
	void ProcessNPCFriendshipMaintenance(const FApartmentLifeGameTime& CurrentTime);
	void CompleteInvitation(const FApartmentLifeSocialInvitation& Invitation);

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> RegisteredCharacters;

	UPROPERTY()
	TArray<FApartmentLifeSocialInvitation> PendingInvitations;

	UPROPERTY()
	TArray<FApartmentLifeSocialInvitation> CompletedInvitations;

	UPROPERTY()
	TMap<FName, FApartmentLifeReputationState> ReputationByCharacter;

	bool bTimeBound = false;
};

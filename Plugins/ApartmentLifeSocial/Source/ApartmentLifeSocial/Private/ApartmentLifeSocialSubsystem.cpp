// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSocialSubsystem.h"
#include "ApartmentLifeSaveableRegistry.h"
#include "ApartmentLifeSocialLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "JsonObjectConverter.h"

void UApartmentLifeSocialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FApartmentLifeSaveableRegistry::Register(this);
}

void UApartmentLifeSocialSubsystem::Deinitialize()
{
	FApartmentLifeSaveableRegistry::Unregister(this);
	Super::Deinitialize();
}

void UApartmentLifeSocialSubsystem::HandleHourAdvanced(const FApartmentLifeGameTime& CurrentTime)
{
	ProcessSocialAutonomy(CurrentTime);
}

void UApartmentLifeSocialSubsystem::RegisterSimCharacter(AActor* Character)
{
	if (!Character)
	{
		return;
	}

	RegisteredCharacters.AddUnique(Character);

	if (!bTimeBound)
	{
		if (UWorld* World = Character->GetWorld())
		{
			if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
			{
				TimeSubsystem->OnHourAdvanced.AddDynamic(this, &UApartmentLifeSocialSubsystem::HandleHourAdvanced);
				bTimeBound = true;
			}
		}
	}
}

void UApartmentLifeSocialSubsystem::ProcessSocialAutonomy(const FApartmentLifeGameTime& CurrentTime)
{
	ProcessNPCFriendshipMaintenance(CurrentTime);
}

void UApartmentLifeSocialSubsystem::ProcessNPCFriendshipMaintenance(const FApartmentLifeGameTime& CurrentTime)
{
	for (const TWeakObjectPtr<AActor>& WeakCharacter : RegisteredCharacters)
	{
		AActor* Character = WeakCharacter.Get();
		if (!Character)
		{
			continue;
		}

		UApartmentLifeNPCSimulationComponent* Sim = Character->FindComponentByClass<UApartmentLifeNPCSimulationComponent>();
		if (!Sim || Sim->Needs.Social < 40.f)
		{
			continue;
		}

		for (const FApartmentLifeRelationshipRecord& Relationship : Sim->GetRelationships())
		{
			if (Relationship.Friendship > 30.f && Sim->TryInitiateSocialInvitation(Relationship.OtherCharacterId))
			{
				IssueInvitation(Sim->GetCharacterId(), Relationship.OtherCharacterId, EApartmentLifeSocialActivityType::Coffee, FName(TEXT("activity.social.coffee")));
				break;
			}
		}
	}
}

bool UApartmentLifeSocialSubsystem::IssueInvitation(FName InviterId, FName InviteeId, EApartmentLifeSocialActivityType ActivityType, FName ActivityId)
{
	FApartmentLifeSocialInvitation Invitation;
	Invitation.InvitationId = FGuid::NewGuid();
	Invitation.InviterCharacterId = InviterId;
	Invitation.InviteeCharacterId = InviteeId;
	Invitation.ActivityType = ActivityType;
	Invitation.ActivityId = ActivityId;
	Invitation.Status = EApartmentLifeInvitationStatus::Pending;

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Invitation.ScheduledTime = TimeSubsystem->GetCurrentTime();
		}
	}

	UApartmentLifeNPCSimulationComponent* InviteeSim = FindSimByCharacterId(InviteeId);
	if (InviteeSim)
	{
		FApartmentLifeRelationshipRecord Relationship;
		InviteeSim->GetRelationshipWith(InviterId, Relationship);

		if (!UApartmentLifeSocialLibrary::ShouldAcceptInvitation(Invitation, Relationship, InviteeSim->GetMood(), InviteeSim->GetPersonality()))
		{
			Invitation.Status = EApartmentLifeInvitationStatus::Declined;
			PendingInvitations.Add(Invitation);
			return false;
		}

		Invitation.Status = EApartmentLifeInvitationStatus::Accepted;
		PendingInvitations.Add(Invitation);
		OnSocialInvitationIssued.Broadcast(Invitation);
		CompleteInvitation(Invitation);
		return true;
	}

	PendingInvitations.Add(Invitation);
	OnSocialInvitationIssued.Broadcast(Invitation);
	return true;
}

bool UApartmentLifeSocialSubsystem::RespondToInvitation(const FGuid& InvitationId, bool bAccept)
{
	for (FApartmentLifeSocialInvitation& Invitation : PendingInvitations)
	{
		if (Invitation.InvitationId == InvitationId)
		{
			Invitation.Status = bAccept ? EApartmentLifeInvitationStatus::Accepted : EApartmentLifeInvitationStatus::Declined;
			if (bAccept)
			{
				CompleteInvitation(Invitation);
			}
			return true;
		}
	}
	return false;
}

void UApartmentLifeSocialSubsystem::CompleteInvitation(const FApartmentLifeSocialInvitation& Invitation)
{
	UApartmentLifeNPCSimulationComponent* InviterSim = FindSimByCharacterId(Invitation.InviterCharacterId);
	UApartmentLifeNPCSimulationComponent* InviteeSim = FindSimByCharacterId(Invitation.InviteeCharacterId);

	if (InviterSim)
	{
		InviterSim->RecordSharedActivityWith(Invitation.InviteeCharacterId, 0.7f);
		InviterSim->RecordMemory(EApartmentLifeMemoryCategory::SharedActivity, Invitation.InviteeCharacterId, 0.6f);
		InviterSim->ApplySocialFulfillment(12.f);
	}

	if (InviteeSim)
	{
		InviteeSim->RecordSharedActivityWith(Invitation.InviterCharacterId, 0.7f);
		InviteeSim->RecordMemory(EApartmentLifeMemoryCategory::Invitation, Invitation.InviterCharacterId, 0.5f);
		InviteeSim->ApplySocialFulfillment(12.f);

		if (UApartmentLifeActivityComponent* Activity = InviteeSim->GetOwner()->FindComponentByClass<UApartmentLifeActivityComponent>())
		{
			Activity->StartActivity(Invitation.ActivityId);
		}
	}

	CompletedInvitations.Add(Invitation);
}

void UApartmentLifeSocialSubsystem::RecordReputationAction(FName CharacterId, EApartmentLifeReputationAction Action)
{
	FApartmentLifeReputationState& Reputation = ReputationByCharacter.FindOrAdd(CharacterId);
	Reputation = UApartmentLifeSocialLibrary::ApplyReputationAction(Reputation, Action);
}

const FApartmentLifeReputationState& UApartmentLifeSocialSubsystem::GetReputation(FName CharacterId) const
{
	if (const FApartmentLifeReputationState* Found = ReputationByCharacter.Find(CharacterId))
	{
		return *Found;
	}

	static const FApartmentLifeReputationState Default;
	return Default;
}

UApartmentLifeNPCSimulationComponent* UApartmentLifeSocialSubsystem::FindSimByCharacterId(FName CharacterId) const
{
	for (const TWeakObjectPtr<AActor>& WeakCharacter : RegisteredCharacters)
	{
		if (AActor* Character = WeakCharacter.Get())
		{
			if (UApartmentLifeNPCSimulationComponent* Sim = Character->FindComponentByClass<UApartmentLifeNPCSimulationComponent>())
			{
				if (Sim->GetCharacterId() == CharacterId)
				{
					return Sim;
				}
			}
		}
	}
	return nullptr;
}

FString UApartmentLifeSocialSubsystem::GetSaveId_Implementation() const
{
	return TEXT("social_subsystem");
}

void UApartmentLifeSocialSubsystem::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString InvitationsJson;
	FJsonObjectConverter::UStructToJsonObjectString(PendingInvitations, InvitationsJson);
	OutData.Add(TEXT("PendingInvitations"), InvitationsJson);

	FString ReputationJson;
	FJsonObjectConverter::UStructToJsonObjectString(ReputationByCharacter, ReputationJson);
	OutData.Add(TEXT("Reputation"), ReputationJson);
}

void UApartmentLifeSocialSubsystem::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* InvitationsJson = InData.Find(TEXT("PendingInvitations")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*InvitationsJson, &PendingInvitations);
	}
	if (const FString* ReputationJson = InData.Find(TEXT("Reputation")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*ReputationJson, &ReputationByCharacter);
	}
}

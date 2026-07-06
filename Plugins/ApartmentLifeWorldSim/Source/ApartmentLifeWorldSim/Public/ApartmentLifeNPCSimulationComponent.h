// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeWorldSimTypes.h"
#include "ApartmentLifeWorldSimDataAssets.h"
#include "ApartmentLifeNPCSimulationComponent.generated.h"

class UApartmentLifeActivityComponent;
class UApartmentLifeGameTimeSubsystem;
class UApartmentLifeDataRegistrySubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCActivityChanged, FName, ActivityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCPurchasedItem, FName, ItemId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCCareerPromoted, int32, NewTier);

/**
 * Master NPC life engine. Orchestrates schedule generation, career, finance, mood,
 * memory, relationships, shopping, and apartment progression. Runs whether player is present.
 */
UCLASS(ClassGroup = AI, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWORLDSIM_API UApartmentLifeNPCSimulationComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UApartmentLifeNPCSimulationComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	FName GetCharacterId() const { return CharacterId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	FName GetCurrentActivityId() const { return CurrentActivityId; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const FApartmentLifePersonalityTraits& GetPersonality() const { return Personality; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const FApartmentLifeMoodState& GetMood() const { return Mood; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const FApartmentLifeFinancialLedger& GetFinance() const { return Finance; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const FApartmentLifeCareerState& GetCareer() const { return Career; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	bool HasCloseRelationship() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const TArray<FApartmentLifeRelationshipRecord>& GetRelationships() const { return Relationships; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const TArray<FApartmentLifeMemoryRecord>& GetMemories() const { return Memories; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	bool GetRelationshipWith(FName OtherCharacterId, FApartmentLifeRelationshipRecord& OutRecord) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void ApplySocialFulfillment(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void ComputeSharedInterestsWith(FName OtherCharacterId, const FApartmentLifePersonalityTraits& OtherPersonality);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void RecordConversationWith(FName OtherCharacterId, float Quality);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void RecordSharedActivityWith(FName OtherCharacterId, float Quality);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void GainSkill(EApartmentLifeSkill Skill, float Amount);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	const FApartmentLifeSkillSet& GetSkills() const { return Skills; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void SetHomeDistrict(FName DistrictId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Simulation")
	FName GetHomeDistrictId() const { return Career.HomeDistrictId; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void SetCareerData(UApartmentLifeCareerData* InCareerData);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void SetScheduleTemplate(UApartmentLifeScheduleTemplateData* InTemplate);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void SimulateMinute(const FApartmentLifeGameTime& CurrentTime);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void RecordMemory(EApartmentLifeMemoryCategory Category, FName SubjectId, float EmotionalWeight);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	void AdjustRelationship(FName OtherCharacterId, float FriendshipDelta, float TrustDelta, float RespectDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	bool TryInitiateSocialInvitation(FName TargetCharacterId) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Simulation")
	bool ExecuteTopShoppingIntent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifePersonalityTraits Personality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeMoodState Mood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeMoodInfluences MoodInfluences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeCareerState Career;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeFinancialLedger Finance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeNPCNeeds Needs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeSkillSet Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
	FApartmentLifeApartmentProgressState Apartment;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Simulation")
	FOnNPCActivityChanged OnActivityChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Simulation")
	FOnNPCPurchasedItem OnPurchasedItem;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Simulation")
	FOnNPCCareerPromoted OnCareerPromoted;

	// IApartmentLifeSaveable
	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UFUNCTION()
	void HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime);

	UFUNCTION()
	void HandleDayAdvanced(const FApartmentLifeGameTime& NewTime);

	UFUNCTION()
	void HandleRandomEvent(FName EventId);

	void RegenerateDailySchedule(const FApartmentLifeGameTime& CurrentTime);
	void EvaluateCurrentScheduleSlot(const FApartmentLifeGameTime& CurrentTime);
	void UpdateNeedsAndMood(const FApartmentLifeGameTime& CurrentTime);
	void ProcessDailyFinance();
	void ProcessMemoryDecay();
	void ProcessShoppingEvaluation();
	void ProcessApartmentProgression();
	void StartActivityForCurrentSlot();
	UApartmentLifeActivityComponent* GetActivityComponent() const;
	TArray<UApartmentLifeShoppingItemData*> GetAvailableShoppingItems() const;

	UPROPERTY()
	TObjectPtr<UApartmentLifeCareerData> CareerData;

	UPROPERTY()
	TObjectPtr<UApartmentLifeScheduleTemplateData> ScheduleTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simulation")
	TArray<FApartmentLifeDailyScheduleSlot> TodaysSchedule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simulation")
	TArray<FApartmentLifeMemoryRecord> Memories;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simulation")
	TArray<FApartmentLifeRelationshipRecord> Relationships;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simulation")
	TArray<FApartmentLifeShoppingIntent> PendingShoppingIntents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simulation")
	FName CurrentActivityId;

	int32 LastScheduleDay = -1;
	int32 LastFinanceDay = -1;
	int32 MinutesSinceShoppingEval = 0;
};

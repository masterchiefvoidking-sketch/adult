// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeWorldSimTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeMemoryCategory : uint8
{
	PersonMet			UMETA(DisplayName = "Person Met"),
	Conversation		UMETA(DisplayName = "Conversation"),
	GiftReceived		UMETA(DisplayName = "Gift"),
	Invitation			UMETA(DisplayName = "Invitation"),
	Holiday				UMETA(DisplayName = "Holiday"),
	Birthday			UMETA(DisplayName = "Birthday"),
	SharedHobby			UMETA(DisplayName = "Shared Hobby"),
	ApartmentVisit		UMETA(DisplayName = "Apartment Visit"),
	PersonalMilestone	UMETA(DisplayName = "Personal Milestone"),
	PromiseKept			UMETA(DisplayName = "Promise Kept"),
	PromiseBroken		UMETA(DisplayName = "Promise Broken"),
	MissedAppointment	UMETA(DisplayName = "Missed Appointment"),
	SharedActivity		UMETA(DisplayName = "Shared Activity"),
	LifeEvent			UMETA(DisplayName = "Life Event"),
	RelationshipMilestone UMETA(DisplayName = "Relationship Milestone")
};

UENUM(BlueprintType)
enum class EApartmentLifeShoppingCategory : uint8
{
	Furniture	UMETA(DisplayName = "Furniture"),
	Groceries	UMETA(DisplayName = "Groceries"),
	Decor		UMETA(DisplayName = "Decor"),
	Electronics	UMETA(DisplayName = "Electronics"),
	Kitchen		UMETA(DisplayName = "Kitchen"),
	Plants		UMETA(DisplayName = "Plants"),
	Books		UMETA(DisplayName = "Books"),
	Games		UMETA(DisplayName = "Games"),
	Clothing	UMETA(DisplayName = "Clothing")
};

UENUM(BlueprintType)
enum class EApartmentLifeMoodLevel : uint8
{
	Miserable	UMETA(DisplayName = "Miserable"),
	Low			UMETA(DisplayName = "Low"),
	Neutral		UMETA(DisplayName = "Neutral"),
	Good		UMETA(DisplayName = "Good"),
	Excellent	UMETA(DisplayName = "Excellent")
};

UENUM(BlueprintType)
enum class EApartmentLifeScheduleBlock : uint8
{
	Sleep		UMETA(DisplayName = "Sleep"),
	WakeUp		UMETA(DisplayName = "Wake Up"),
	Hygiene		UMETA(DisplayName = "Hygiene"),
	Breakfast	UMETA(DisplayName = "Breakfast"),
	GetDressed	UMETA(DisplayName = "Get Dressed"),
	Commute		UMETA(DisplayName = "Commute"),
	Work		UMETA(DisplayName = "Work"),
	Lunch		UMETA(DisplayName = "Lunch"),
	Shopping	UMETA(DisplayName = "Shopping"),
	Exercise	UMETA(DisplayName = "Exercise"),
	Study		UMETA(DisplayName = "Study"),
	Dinner		UMETA(DisplayName = "Dinner"),
	Relax		UMETA(DisplayName = "Relax"),
	Entertainment UMETA(DisplayName = "Entertainment"),
	Chores		UMETA(DisplayName = "Chores"),
	Social		UMETA(DisplayName = "Social")
};

UENUM(BlueprintType)
enum class EApartmentLifeSkill : uint8
{
	Cooking			UMETA(DisplayName = "Cooking"),
	Fitness			UMETA(DisplayName = "Fitness"),
	Creativity		UMETA(DisplayName = "Creativity"),
	Programming		UMETA(DisplayName = "Programming"),
	Art				UMETA(DisplayName = "Art"),
	Music			UMETA(DisplayName = "Music"),
	Organization	UMETA(DisplayName = "Organization"),
	Communication	UMETA(DisplayName = "Communication"),
	Leadership		UMETA(DisplayName = "Leadership"),
	HomeMaintenance	UMETA(DisplayName = "Home Maintenance")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeSkillSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Cooking = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Fitness = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Creativity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Programming = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Art = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Music = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Organization = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Communication = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float Leadership = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (ClampMin = "1", ClampMax = "100"))
	float HomeMaintenance = 1.f;

	APARTMENTLIFEWORLDSIM_API void GainSkill(EApartmentLifeSkill Skill, float Amount);
	APARTMENTLIFEWORLDSIM_API float GetSkill(EApartmentLifeSkill Skill) const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifePersonalityTraits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Introversion = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Extroversion = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Kindness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Confidence = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Humor = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Patience = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Empathy = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Ambition = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Tidiness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Creativity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Curiosity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Responsibility = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Generosity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Optimism = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float EmotionalOpenness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta = (ClampMin = "0", ClampMax = "1"))
	float Organization = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float SpendingHabits = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float FitnessInterest = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float CookingInterest = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float ReadingInterest = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float GamingInterest = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interests", meta = (ClampMin = "0", ClampMax = "1"))
	float MusicInterest = 0.5f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeMoodState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float OverallMood = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Happiness = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Stress = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Fatigue = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Energy = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Confidence = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Motivation = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Loneliness = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float Comfort = 65.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood", meta = (ClampMin = "0", ClampMax = "100"))
	float SocialBattery = 70.f;

	APARTMENTLIFEWORLDSIM_API EApartmentLifeMoodLevel GetMoodLevel() const;
	APARTMENTLIFEWORLDSIM_API float GetProductivityMultiplier() const;
	APARTMENTLIFEWORLDSIM_API float GetSocialDriveMultiplier() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeMoodInfluences
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float SleepQuality = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float WorkSatisfaction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float ApartmentCleanliness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float WeatherComfort = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float FinancialSecurity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float MealQuality = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float ExerciseBenefit = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float SocialFulfillment = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float PersonalAchievement = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeCareerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	FName CareerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	int32 SkillLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	float Experience = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	float HourlyWage = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	float MonthlySalary = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	bool bIsSalaried = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	int32 PromotionTier = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	int32 VacationDaysRemaining = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career", meta = (ClampMin = "0", ClampMax = "100"))
	float WorkSatisfaction = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	FName WorkplacePOIId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	FName HomeDistrictId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Career")
	FName WorkDistrictId;

	APARTMENTLIFEWORLDSIM_API float ComputeMonthlyIncome() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeFinancialLedger
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float Savings = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float EmergencyFund = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float InvestmentAccount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float Debt = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance", meta = (ClampMin = "300", ClampMax = "850"))
	int32 CreditScore = 680;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyRent = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyUtilities = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyFood = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyInternet = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyTransportation = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float MonthlyEntertainment = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float LastMonthIncome = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finance")
	float LastMonthExpenses = 0.f;

	APARTMENTLIFEWORLDSIM_API float GetMonthlyFixedExpenses() const;
	APARTMENTLIFEWORLDSIM_API float GetDisposableIncome(float MonthlyIncome) const;
	APARTMENTLIFEWORLDSIM_API bool CanAfford(float Amount) const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeRelationshipRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	FName OtherCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	FName RelationshipStage = FName(TEXT("stage.stranger"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Familiarity = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Friendship = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Trust = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Respect = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Comfort = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Admiration = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float Compatibility = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float SharedInterests = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta = (ClampMin = "0", ClampMax = "100"))
	float RomanceAttraction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	int32 ConversationCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship")
	int32 SharedActivityCount = 0;

	APARTMENTLIFEWORLDSIM_API float GetInvitationScore() const;
	APARTMENTLIFEWORLDSIM_API void RecordConversation(float Quality);
	APARTMENTLIFEWORLDSIM_API void RecordSharedActivity(float Quality);
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeMemoryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FGuid MemoryId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	EApartmentLifeMemoryCategory Category = EApartmentLifeMemoryCategory::Conversation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FName SubjectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FApartmentLifeGameTime Timestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory", meta = (ClampMin = "0", ClampMax = "1"))
	float Strength = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	float EmotionalWeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	FText SummaryText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
	int32 ReinforcementCount = 0;

	APARTMENTLIFEWORLDSIM_API float GetRecencyWeight(const FApartmentLifeGameTime& CurrentTime) const;
	APARTMENTLIFEWORLDSIM_API void ApplyDecay(float DecayRate);
	APARTMENTLIFEWORLDSIM_API void Reinforce(float Amount);
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeDailyScheduleSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	EApartmentLifeScheduleBlock BlockType = EApartmentLifeScheduleBlock::Relax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	int32 StartHour = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	int32 EndHour = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	FName ActivityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	EApartmentLifeOccasion Occasion = EApartmentLifeOccasion::Everyday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	float Priority = 1.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeShoppingIntent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	EApartmentLifeShoppingCategory Category = EApartmentLifeShoppingCategory::Groceries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	float EstimatedCost = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	float Urgency = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	float DesireScore = 0.5f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeApartmentProgressState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	int32 ApartmentTier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	FName ApartmentUnitId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	TArray<FName> OwnedFurnitureIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	TArray<FName> UpgradeWishlist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment", meta = (ClampMin = "0", ClampMax = "100"))
	float Cleanliness = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float AvailableFloorSpace = 100.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWORLDSIM_API FApartmentLifeNPCNeeds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta = (ClampMin = "0", ClampMax = "100"))
	float Hunger = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta = (ClampMin = "0", ClampMax = "100"))
	float Hygiene = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta = (ClampMin = "0", ClampMax = "100"))
	float Social = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs", meta = (ClampMin = "0", ClampMax = "100"))
	float Fun = 50.f;
};

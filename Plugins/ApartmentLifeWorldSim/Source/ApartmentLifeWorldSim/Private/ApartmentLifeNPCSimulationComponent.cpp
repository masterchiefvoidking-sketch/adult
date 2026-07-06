// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeWorldSimLibrary.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeWorldEventSubsystem.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "JsonObjectConverter.h"

UApartmentLifeNPCSimulationComponent::UApartmentLifeNPCSimulationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CharacterId = FName(TEXT("npc.default"));
	Career.CareerId = FName(TEXT("career.office_worker"));
	Career.HourlyWage = 22.f;
	Finance.MonthlyRent = 950.f;
}

bool UApartmentLifeNPCSimulationComponent::HasCloseRelationship() const
{
	for (const FApartmentLifeRelationshipRecord& Relationship : Relationships)
	{
		if (Relationship.Friendship > 70.f && Relationship.Comfort > 60.f)
		{
			return true;
		}
	}
	return false;
}

bool UApartmentLifeNPCSimulationComponent::GetRelationshipWith(FName OtherCharacterId, FApartmentLifeRelationshipRecord& OutRecord) const
{
	for (const FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == OtherCharacterId)
		{
			OutRecord = Record;
			return true;
		}
	}
	return false;
}

void UApartmentLifeNPCSimulationComponent::ApplySocialFulfillment(float Amount)
{
	MoodInfluences.SocialFulfillment = FMath::Clamp(MoodInfluences.SocialFulfillment + Amount, 0.f, 100.f);
	Needs.Social = FMath::Clamp(Needs.Social - Amount * 0.5f, 0.f, 100.f);
}

void UApartmentLifeNPCSimulationComponent::ComputeSharedInterestsWith(FName OtherCharacterId, const FApartmentLifePersonalityTraits& OtherPersonality)
{
	const float Overlap =
		(FMath::Abs(Personality.CookingInterest - OtherPersonality.CookingInterest) < 0.3f ? 1.f : 0.f)
		+ (FMath::Abs(Personality.GamingInterest - OtherPersonality.GamingInterest) < 0.3f ? 1.f : 0.f)
		+ (FMath::Abs(Personality.ReadingInterest - OtherPersonality.ReadingInterest) < 0.3f ? 1.f : 0.f)
		+ (FMath::Abs(Personality.FitnessInterest - OtherPersonality.FitnessInterest) < 0.3f ? 1.f : 0.f)
		+ (FMath::Abs(Personality.MusicInterest - OtherPersonality.MusicInterest) < 0.3f ? 1.f : 0.f);

	for (FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == OtherCharacterId)
		{
			Record.SharedInterests = FMath::Clamp(Overlap * 20.f, 0.f, 100.f);
			Record.Compatibility = FMath::Clamp(50.f + Overlap * 8.f, 0.f, 100.f);
			return;
		}
	}
}

void UApartmentLifeNPCSimulationComponent::RecordConversationWith(FName OtherCharacterId, float Quality)
{
	for (FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == OtherCharacterId)
		{
			Record.RecordConversation(Quality);
			return;
		}
	}

	FApartmentLifeRelationshipRecord NewRecord;
	NewRecord.OtherCharacterId = OtherCharacterId;
	NewRecord.RecordConversation(Quality);
	Relationships.Add(NewRecord);
}

void UApartmentLifeNPCSimulationComponent::RecordSharedActivityWith(FName OtherCharacterId, float Quality)
{
	for (FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == OtherCharacterId)
		{
			Record.RecordSharedActivity(Quality);
			return;
		}
	}

	FApartmentLifeRelationshipRecord NewRecord;
	NewRecord.OtherCharacterId = OtherCharacterId;
	NewRecord.RecordSharedActivity(Quality);
	Relationships.Add(NewRecord);
}

void UApartmentLifeNPCSimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnMinuteAdvanced.AddDynamic(this, &UApartmentLifeNPCSimulationComponent::HandleMinuteAdvanced);
			TimeSubsystem->OnDayAdvanced.AddDynamic(this, &UApartmentLifeNPCSimulationComponent::HandleDayAdvanced);
			RegenerateDailySchedule(TimeSubsystem->GetCurrentTime());
		}

		if (UApartmentLifeWorldEventSubsystem* EventSubsystem = World->GetSubsystem<UApartmentLifeWorldEventSubsystem>())
		{
			EventSubsystem->OnWorldEventTriggered.AddDynamic(this, &UApartmentLifeNPCSimulationComponent::HandleRandomEvent);
		}
	}
}

void UApartmentLifeNPCSimulationComponent::SetCareerData(UApartmentLifeCareerData* InCareerData)
{
	CareerData = InCareerData;
	if (CareerData)
	{
		Career.CareerId = CareerData->AssetId;
		Career.bIsSalaried = CareerData->bIsSalaried;
		Career.HourlyWage = CareerData->BaseHourlyWage;
		Career.MonthlySalary = CareerData->BaseMonthlySalary;
	}
}

void UApartmentLifeNPCSimulationComponent::SetScheduleTemplate(UApartmentLifeScheduleTemplateData* InTemplate)
{
	ScheduleTemplate = InTemplate;
}

void UApartmentLifeNPCSimulationComponent::HandleMinuteAdvanced(const FApartmentLifeGameTime& NewTime)
{
	SimulateMinute(NewTime);
}

void UApartmentLifeNPCSimulationComponent::HandleDayAdvanced(const FApartmentLifeGameTime& NewTime)
{
	RegenerateDailySchedule(NewTime);
	ProcessDailyFinance();
	ProcessShoppingEvaluation();
	ProcessApartmentProgression();
	ProcessMemoryDecay();

	if (CareerData)
	{
		Career.Experience += CareerData->ExperiencePerWorkDay;
		if (UApartmentLifeWorldSimLibrary::TryPromoteCareer(Career, CareerData))
		{
			OnCareerPromoted.Broadcast(Career.PromotionTier);
			RecordMemory(EApartmentLifeMemoryCategory::LifeEvent, Career.CareerId, 0.8f);
		}
	}
}

void UApartmentLifeNPCSimulationComponent::HandleRandomEvent(FName EventId)
{
	RecordMemory(EApartmentLifeMemoryCategory::LifeEvent, EventId, 0.5f);
	Mood.OverallMood = FMath::Clamp(Mood.OverallMood - 5.f, 0.f, 100.f);
}

void UApartmentLifeNPCSimulationComponent::SimulateMinute(const FApartmentLifeGameTime& CurrentTime)
{
	UpdateNeedsAndMood(CurrentTime);
	EvaluateCurrentScheduleSlot(CurrentTime);

	++MinutesSinceShoppingEval;
	if (MinutesSinceShoppingEval >= 60)
	{
		MinutesSinceShoppingEval = 0;
		if (PendingShoppingIntents.Num() > 0 && CurrentTime.Hour >= 10 && CurrentTime.Hour <= 18)
		{
			ExecuteTopShoppingIntent();
		}
	}
}

void UApartmentLifeNPCSimulationComponent::RegenerateDailySchedule(const FApartmentLifeGameTime& CurrentTime)
{
	const int32 DayKey = CurrentTime.GetDayOfYear() + CurrentTime.Year * 400;
	if (DayKey == LastScheduleDay)
	{
		return;
	}
	LastScheduleDay = DayKey;

	if (!ScheduleTemplate)
	{
		TodaysSchedule = UApartmentLifeWorldSimLibrary::CreateBuiltinWeekdaySchedule();
		return;
	}

	bool bIsHoliday = false;
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			FName HolidayId;
			bIsHoliday = TimeSubsystem->IsHolidayToday(HolidayId);
			const FApartmentLifeWeatherState Weather = TimeSubsystem->GetCurrentWeather();
			TodaysSchedule = UApartmentLifeWorldSimLibrary::GenerateDailySchedule(
				ScheduleTemplate,
				Personality,
				Mood,
				Finance,
				Career,
				Weather,
				TimeSubsystem->IsWeekend(),
				bIsHoliday);
		}
	}
}

void UApartmentLifeNPCSimulationComponent::EvaluateCurrentScheduleSlot(const FApartmentLifeGameTime& CurrentTime)
{
	FName NewActivity = NAME_None;

	for (const FApartmentLifeDailyScheduleSlot& Slot : TodaysSchedule)
	{
		if (CurrentTime.Hour >= Slot.StartHour && CurrentTime.Hour < Slot.EndHour)
		{
			NewActivity = Slot.ActivityId;
			break;
		}
	}

	if (NewActivity != CurrentActivityId)
	{
		CurrentActivityId = NewActivity;
		OnActivityChanged.Broadcast(CurrentActivityId);
		StartActivityForCurrentSlot();
	}
}

void UApartmentLifeNPCSimulationComponent::UpdateNeedsAndMood(const FApartmentLifeGameTime& CurrentTime)
{
	Needs.Hunger = FMath::Clamp(Needs.Hunger + 0.02f, 0.f, 100.f);
	Needs.Hygiene = FMath::Clamp(Needs.Hygiene - 0.01f, 0.f, 100.f);
	Needs.Social = FMath::Clamp(Needs.Social + (1.f - Personality.Introversion) * 0.01f, 0.f, 100.f);
	Needs.Fun = FMath::Clamp(Needs.Fun + 0.015f, 0.f, 100.f);

	if (CurrentTime.Hour >= 23 || CurrentTime.Hour < 6)
	{
		Mood.Fatigue = FMath::Clamp(Mood.Fatigue + 0.05f, 0.f, 100.f);
	}

	MoodInfluences.ApartmentCleanliness = Apartment.Cleanliness;
	MoodInfluences.WorkSatisfaction = Career.WorkSatisfaction;
	MoodInfluences.FinancialSecurity = FMath::Clamp(Finance.Savings / 3000.f * 100.f, 0.f, 100.f);

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			const FApartmentLifeWeatherState Weather = TimeSubsystem->GetCurrentWeather();
			MoodInfluences.WeatherComfort = FMath::Clamp(100.f - FMath::Abs(Weather.TemperatureCelsius - 22.f) * 3.f, 0.f, 100.f);
		}
	}

	Mood = UApartmentLifeWorldSimLibrary::RecalculateMood(Mood, MoodInfluences, Personality);
}

void UApartmentLifeNPCSimulationComponent::ProcessDailyFinance()
{
	const float MonthlyIncome = Career.ComputeMonthlyIncome();
	const float FixedExpenses = Finance.GetMonthlyFixedExpenses();
	const float TotalExpenses = FixedExpenses + Finance.MonthlyEntertainment;

	Finance.LastMonthIncome = MonthlyIncome;
	Finance.LastMonthExpenses = TotalExpenses;

	const float DailyIncome = MonthlyIncome / 30.f;
	const float DailyExpenses = TotalExpenses / 30.f;
	const float Net = DailyIncome - DailyExpenses;

	Finance.Savings += Net;

	if (Net < 0.f)
	{
		Finance.CreditScore = FMath::Max(300, Finance.CreditScore - 2);
	}
	else if (Finance.Savings > Finance.EmergencyFund * 2.f)
	{
		Finance.CreditScore = FMath::Min(850, Finance.CreditScore + 1);
		const float Investable = Net * 0.1f;
		Finance.InvestmentAccount += Investable;
		Finance.Savings -= Investable;
	}

	if (Finance.Savings < 0.f)
	{
		Finance.Debt += FMath::Abs(Finance.Savings);
		Finance.Savings = 0.f;
		Finance.CreditScore = FMath::Max(300, Finance.CreditScore - 5);
	}
}

void UApartmentLifeNPCSimulationComponent::ProcessMemoryDecay()
{
	const float DecayRate = 0.01f;
	for (FApartmentLifeMemoryRecord& Memory : Memories)
	{
		Memory.ApplyDecay(DecayRate);
	}
	Memories.RemoveAll([](const FApartmentLifeMemoryRecord& M) { return M.Strength <= 0.01f; });
}

void UApartmentLifeNPCSimulationComponent::ProcessShoppingEvaluation()
{
	EApartmentLifeSeason Season = EApartmentLifeSeason::Summer;
	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Season = TimeSubsystem->GetCurrentSeason();
		}
	}

	PendingShoppingIntents = UApartmentLifeWorldSimLibrary::EvaluateShoppingNeeds(
		Personality, Mood, Finance, Needs, Apartment, Season, GetAvailableShoppingItems());
}

void UApartmentLifeNPCSimulationComponent::ProcessApartmentProgression()
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				const TArray<UApartmentLifePrimaryDataAsset*> TierAssets = Registry->GetAssetsByType(FName(TEXT("ApartmentTier")));
				for (UApartmentLifePrimaryDataAsset* Asset : TierAssets)
				{
					UApartmentLifeApartmentTierData* Tier = Cast<UApartmentLifeApartmentTierData>(Asset);
					if (!Tier || Tier->TierLevel <= Apartment.ApartmentTier)
					{
						continue;
					}

					if (Finance.Savings >= Tier->MinimumSavings && Finance.CreditScore >= Tier->MinimumCreditScore)
					{
						Apartment.ApartmentTier = Tier->TierLevel;
						Finance.MonthlyRent = Tier->MonthlyRent;
						Apartment.AvailableFloorSpace = Tier->FloorSpace;
						RecordMemory(EApartmentLifeMemoryCategory::LifeEvent, Tier->AssetId, 0.9f);
						break;
					}
				}
			}
		}
	}

	for (const FName& WishItem : Apartment.UpgradeWishlist)
	{
		if (Finance.CanAfford(200.f) && !Apartment.OwnedFurnitureIds.Contains(WishItem))
		{
			Apartment.OwnedFurnitureIds.Add(WishItem);
			Apartment.UpgradeWishlist.Remove(WishItem);
			Finance.Savings -= 200.f;
			break;
		}
	}
}

void UApartmentLifeNPCSimulationComponent::StartActivityForCurrentSlot()
{
	if (UApartmentLifeActivityComponent* ActivityComp = GetActivityComponent())
	{
		if (!CurrentActivityId.IsNone())
		{
			ActivityComp->StartActivity(CurrentActivityId);
		}
	}
}

void UApartmentLifeNPCSimulationComponent::RecordMemory(EApartmentLifeMemoryCategory Category, FName SubjectId, float EmotionalWeight)
{
	for (FApartmentLifeMemoryRecord& Memory : Memories)
	{
		if (Memory.Category == Category && Memory.SubjectId == SubjectId)
		{
			Memory.Reinforce(0.2f);
			Memory.EmotionalWeight = FMath::Max(Memory.EmotionalWeight, EmotionalWeight);
			return;
		}
	}

	FApartmentLifeMemoryRecord NewMemory;
	NewMemory.MemoryId = FGuid::NewGuid();
	NewMemory.Category = Category;
	NewMemory.SubjectId = SubjectId;
	NewMemory.EmotionalWeight = EmotionalWeight;
	NewMemory.Strength = 1.f;

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			NewMemory.Timestamp = TimeSubsystem->GetCurrentTime();
		}
	}

	Memories.Add(NewMemory);
}

void UApartmentLifeNPCSimulationComponent::AdjustRelationship(FName OtherCharacterId, float FriendshipDelta, float TrustDelta, float RespectDelta)
{
	for (FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == OtherCharacterId)
		{
			Record.Friendship = FMath::Clamp(Record.Friendship + FriendshipDelta, 0.f, 100.f);
			Record.Trust = FMath::Clamp(Record.Trust + TrustDelta, 0.f, 100.f);
			Record.Respect = FMath::Clamp(Record.Respect + RespectDelta, 0.f, 100.f);
			return;
		}
	}

	FApartmentLifeRelationshipRecord NewRecord;
	NewRecord.OtherCharacterId = OtherCharacterId;
	NewRecord.Friendship = FMath::Clamp(30.f + FriendshipDelta, 0.f, 100.f);
	NewRecord.Trust = FMath::Clamp(40.f + TrustDelta, 0.f, 100.f);
	NewRecord.Respect = FMath::Clamp(40.f + RespectDelta, 0.f, 100.f);
	Relationships.Add(NewRecord);
}

bool UApartmentLifeNPCSimulationComponent::TryInitiateSocialInvitation(FName TargetCharacterId) const
{
	for (const FApartmentLifeRelationshipRecord& Record : Relationships)
	{
		if (Record.OtherCharacterId == TargetCharacterId)
		{
			const float Score = Record.GetInvitationScore() * Mood.GetSocialDriveMultiplier();
			return Score > 0.45f && FMath::FRand() < Score;
		}
	}
	return Personality.Introversion < 0.4f && Mood.GetSocialDriveMultiplier() > 0.6f;
}

bool UApartmentLifeNPCSimulationComponent::ExecuteTopShoppingIntent()
{
	if (PendingShoppingIntents.Num() == 0)
	{
		return false;
	}

	const FApartmentLifeShoppingIntent& Intent = PendingShoppingIntents[0];
	if (!Finance.CanAfford(Intent.EstimatedCost))
	{
		PendingShoppingIntents.RemoveAt(0);
		return false;
	}

	Finance.Savings -= Intent.EstimatedCost;

	switch (Intent.Category)
	{
	case EApartmentLifeShoppingCategory::Groceries:
		Needs.Hunger = FMath::Max(0.f, Needs.Hunger - 40.f);
		MoodInfluences.MealQuality = 70.f;
		break;
	case EApartmentLifeShoppingCategory::Furniture:
	case EApartmentLifeShoppingCategory::Decor:
		Apartment.OwnedFurnitureIds.Add(Intent.ItemId);
		Apartment.Cleanliness = FMath::Min(Apartment.Cleanliness + 5.f, 100.f);
		break;
	case EApartmentLifeShoppingCategory::Clothing:
		break;
	default:
		Needs.Fun = FMath::Max(0.f, Needs.Fun - 30.f);
		break;
	}

	OnPurchasedItem.Broadcast(Intent.ItemId);
	PendingShoppingIntents.RemoveAt(0);
	return true;
}

UApartmentLifeActivityComponent* UApartmentLifeNPCSimulationComponent::GetActivityComponent() const
{
	if (const AActor* Owner = GetOwner())
	{
		return Owner->FindComponentByClass<UApartmentLifeActivityComponent>();
	}
	return nullptr;
}

TArray<UApartmentLifeShoppingItemData*> UApartmentLifeNPCSimulationComponent::GetAvailableShoppingItems() const
{
	TArray<UApartmentLifeShoppingItemData*> Items;
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				for (UApartmentLifePrimaryDataAsset* Asset : Registry->GetAssetsByType(FName(TEXT("ShoppingItem"))))
				{
					if (UApartmentLifeShoppingItemData* Item = Cast<UApartmentLifeShoppingItemData>(Asset))
					{
						Items.Add(Item);
					}
				}
			}
		}
	}
	return Items;
}

FString UApartmentLifeNPCSimulationComponent::GetSaveId_Implementation() const
{
	return FString::Printf(TEXT("npc_sim_%s"), *CharacterId.ToString());
}

void UApartmentLifeNPCSimulationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("CharacterId"), CharacterId.ToString());
	OutData.Add(TEXT("Savings"), FString::SanitizeFloat(Finance.Savings));
	OutData.Add(TEXT("CreditScore"), FString::FromInt(Finance.CreditScore));
	OutData.Add(TEXT("CareerTier"), FString::FromInt(Career.PromotionTier));
	OutData.Add(TEXT("CurrentActivity"), CurrentActivityId.ToString());
	OutData.Add(TEXT("ApartmentTier"), FString::FromInt(Apartment.ApartmentTier));

	FString PersonalityJson;
	FJsonObjectConverter::UStructToJsonObjectString(Personality, PersonalityJson);
	OutData.Add(TEXT("Personality"), PersonalityJson);

	FString MoodJson;
	FJsonObjectConverter::UStructToJsonObjectString(Mood, MoodJson);
	OutData.Add(TEXT("Mood"), MoodJson);

	FString MoodInfluencesJson;
	FJsonObjectConverter::UStructToJsonObjectString(MoodInfluences, MoodInfluencesJson);
	OutData.Add(TEXT("MoodInfluences"), MoodInfluencesJson);

	FString RelationshipsJson;
	FJsonObjectConverter::UStructToJsonObjectString(Relationships, RelationshipsJson);
	OutData.Add(TEXT("Relationships"), RelationshipsJson);

	FString MemoriesJson;
	FJsonObjectConverter::UStructToJsonObjectString(Memories, MemoriesJson);
	OutData.Add(TEXT("Memories"), MemoriesJson);
}

void UApartmentLifeNPCSimulationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Id = InData.Find(TEXT("CharacterId")))
	{
		CharacterId = FName(**Id);
	}
	if (const FString* Savings = InData.Find(TEXT("Savings")))
	{
		Finance.Savings = FCString::Atof(**Savings);
	}
	if (const FString* Credit = InData.Find(TEXT("CreditScore")))
	{
		Finance.CreditScore = FCString::Atoi(**Credit);
	}
	if (const FString* Tier = InData.Find(TEXT("CareerTier")))
	{
		Career.PromotionTier = FCString::Atoi(**Tier);
	}
	if (const FString* Activity = InData.Find(TEXT("CurrentActivity")))
	{
		CurrentActivityId = FName(**Activity);
	}
	if (const FString* AptTier = InData.Find(TEXT("ApartmentTier")))
	{
		Apartment.ApartmentTier = FCString::Atoi(**AptTier);
	}
	if (const FString* PersonalityJson = InData.Find(TEXT("Personality")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*PersonalityJson, &Personality);
	}
	if (const FString* MoodJson = InData.Find(TEXT("Mood")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*MoodJson, &Mood);
	}
	if (const FString* MoodInfluencesJson = InData.Find(TEXT("MoodInfluences")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*MoodInfluencesJson, &MoodInfluences);
	}
	if (const FString* RelationshipsJson = InData.Find(TEXT("Relationships")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*RelationshipsJson, &Relationships);
	}
	if (const FString* MemoriesJson = InData.Find(TEXT("Memories")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*MemoriesJson, &Memories);
	}
}

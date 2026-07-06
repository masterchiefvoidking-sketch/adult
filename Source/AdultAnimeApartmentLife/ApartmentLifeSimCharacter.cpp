// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeActivityComponent.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeYogaMinigameComponent.h"
#include "ApartmentLifeGroomingRoutineComponent.h"
#include "ApartmentLifeNPCStyleComponent.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeClothingFitComponent.h"
#include "ApartmentLifeConversationComponent.h"
#include "ApartmentLifeInteractionComponent.h"
#include "ApartmentLifeInteractionSelectionComponent.h"
#include "ApartmentLifeBedroomRoutineComponent.h"
#include "ApartmentLifeGirlLifeLibrary.h"
#include "ApartmentLifeActivityLibrary.h"
#include "ApartmentLifeRoutineChainComponent.h"
#include "ApartmentLifeWardrobeLibrary.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeSocialSubsystem.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeGameTimeSubsystem.h"

AApartmentLifeSimCharacter::AApartmentLifeSimCharacter()
{
	SimulationComponent = CreateDefaultSubobject<UApartmentLifeNPCSimulationComponent>(TEXT("Simulation"));
	WardrobeComponent = CreateDefaultSubobject<UApartmentLifeWardrobeComponent>(TEXT("Wardrobe"));
	ActivityComponent = CreateDefaultSubobject<UApartmentLifeActivityComponent>(TEXT("Activity"));
	AnimationComponent = CreateDefaultSubobject<UApartmentLifeAnimationComponent>(TEXT("Animation"));
	YogaComponent = CreateDefaultSubobject<UApartmentLifeYogaMinigameComponent>(TEXT("Yoga"));
	GroomingComponent = CreateDefaultSubobject<UApartmentLifeGroomingRoutineComponent>(TEXT("Grooming"));
	NPCStyleComponent = CreateDefaultSubobject<UApartmentLifeNPCStyleComponent>(TEXT("NPCStyle"));
	ClothingFitComponent = CreateDefaultSubobject<UApartmentLifeClothingFitComponent>(TEXT("ClothingFit"));
	ConversationComponent = CreateDefaultSubobject<UApartmentLifeConversationComponent>(TEXT("Conversation"));
	InteractionComponent = CreateDefaultSubobject<UApartmentLifeInteractionComponent>(TEXT("Interaction"));
	InteractionSelectionComponent = CreateDefaultSubobject<UApartmentLifeInteractionSelectionComponent>(TEXT("InteractionSelection"));
	BedroomRoutineComponent = CreateDefaultSubobject<UApartmentLifeBedroomRoutineComponent>(TEXT("BedroomRoutine"));
	RoutineChainComponent = CreateDefaultSubobject<UApartmentLifeRoutineChainComponent>(TEXT("RoutineChain"));
	ProgressionComponent = CreateDefaultSubobject<UApartmentLifeProgressionComponent>(TEXT("Progression"));
}

void AApartmentLifeSimCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationComponent)
	{
		SimulationComponent->OnActivityChanged.AddDynamic(this, &AApartmentLifeSimCharacter::HandleActivityChanged);
		SimulationComponent->OnPurchasedItem.AddDynamic(this, &AApartmentLifeSimCharacter::HandlePurchasedItem);
	}

	if (WardrobeComponent)
	{
		WardrobeComponent->OnWardrobeUpdated.AddDynamic(this, &AApartmentLifeSimCharacter::HandleWardrobeUpdated);
	}

	if (ActivityComponent)
	{
		ActivityComponent->OnActivityStarted.AddDynamic(this, &AApartmentLifeSimCharacter::HandleActivityStarted);
		ActivityComponent->OnActivityCompleted.AddDynamic(this, &AApartmentLifeSimCharacter::HandleActivityCompleted);
	}

	if (CreatorComponent)
	{
		CreatorComponent->OnCreatorStateUpdated.AddDynamic(this, &AApartmentLifeSimCharacter::HandleCreatorStateUpdated);
	}

	if (BodyCustomizationComponent)
	{
		BodyCustomizationComponent->OnBodyCustomizationUpdated.AddDynamic(this, &AApartmentLifeSimCharacter::HandleBodyFitProfileUpdated);
		RefreshClothingFitFromBody();
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnHourAdvanced.AddDynamic(this, &AApartmentLifeSimCharacter::HandleScheduleOccasion);
			TimeSubsystem->OnDayAdvanced.AddDynamic(this, &AApartmentLifeSimCharacter::HandleDayAdvanced);
		}
	}

	RefreshNPCStyleFromSimulation();

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSocialSubsystem* SocialSubsystem = GI->GetSubsystem<UApartmentLifeSocialSubsystem>())
		{
			SocialSubsystem->RegisterSimCharacter(this);
		}
	}
}

void AApartmentLifeSimCharacter::HandleActivityChanged(FName ActivityId)
{
	if (ActivityComponent && !ActivityId.IsNone())
	{
		ActivityComponent->StartActivity(ActivityId);
	}

	if (AnimationComponent)
	{
		AnimationComponent->SetAnimationGroup(UApartmentLifeActivityLibrary::GetAnimationGroupForActivity(ActivityId));
	}

	if (SimulationComponent)
	{
		UApartmentLifeGirlLifeLibrary::ApplyActivitySkillGain(SimulationComponent->Skills, ActivityId);
	}

	if (ActivityId.ToString().Contains(TEXT("social")) || ActivityId.ToString().Contains(TEXT("talk")))
	{
		if (AnimationComponent)
		{
			AnimationComponent->SetAnimationGroup(EApartmentLifeAnimationGroup::Conversation);
		}
	}
}

void AApartmentLifeSimCharacter::HandleActivityStarted(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();

	if (WardrobeComponent)
	{
		UApartmentLifeWardrobeLibrary::RecommendOutfitForActivity(WardrobeComponent, ActivityId);

		if (SimulationComponent)
		{
			const FApartmentLifeOutfitMoodEffect Effect = UApartmentLifeWardrobeLibrary::ComputeOutfitMoodEffect(WardrobeComponent, ActivityId);
			UApartmentLifeWardrobeLibrary::ApplyOutfitMoodEffect(SimulationComponent, Effect);
		}
	}

	if (Id.Contains(TEXT("laundry")))
	{
		if (WardrobeComponent)
		{
			WardrobeComponent->StartLaundryCycle();
		}
	}
	else if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("stretch")))
	{
		if (YogaComponent)
		{
			YogaComponent->StartYogaSession(FName(TEXT("pose.builtin.stretch")), true);
		}
	}
	else if (Id.Contains(TEXT("brush")) || Id.Contains(TEXT("wash_face")) || Id.Contains(TEXT("skincare")) || Id.Contains(TEXT("hair")) || Id.Contains(TEXT("makeup")))
	{
		if (GroomingComponent)
		{
			GroomingComponent->StartBuiltinMirrorRoutine();
		}
	}
	else if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("hygiene")))
	{
		if (GroomingComponent)
		{
			GroomingComponent->StartBuiltinShowerRoutine();
		}
	}
	else if (Id.Contains(TEXT("groom")) || Id.Contains(TEXT("mirror")))
	{
		if (GroomingComponent)
		{
			GroomingComponent->StartBuiltinMirrorRoutine();
		}
	}
	else if (Id.Contains(TEXT("sleep.bed")))
	{
		if (BedroomRoutineComponent)
		{
			BedroomRoutineComponent->StartRoutine(EApartmentLifeBedroomRoutineType::Sleep);
		}
	}
	else if (Id.Contains(TEXT("sleep.nap")))
	{
		if (BedroomRoutineComponent)
		{
			BedroomRoutineComponent->StartRoutine(EApartmentLifeBedroomRoutineType::Nap);
		}
	}
	else if (Id.Contains(TEXT("read")))
	{
		if (BedroomRoutineComponent)
		{
			BedroomRoutineComponent->StartRoutine(EApartmentLifeBedroomRoutineType::Read);
		}
	}
	else if (Id.Contains(TEXT("relax")))
	{
		if (BedroomRoutineComponent)
		{
			BedroomRoutineComponent->StartRoutine(EApartmentLifeBedroomRoutineType::Relax);
		}
	}
}

void AApartmentLifeSimCharacter::HandleActivityCompleted(FName ActivityId)
{
	if (WardrobeComponent)
	{
		WardrobeComponent->MarkEquippedWorn();

		const FString Id = ActivityId.ToString().ToLower();
		if (Id.Contains(TEXT("laundry")))
		{
			WardrobeComponent->AdvanceLaundryCycle();
		}
	}

	if (YogaComponent && ActivityId.ToString().Contains(TEXT("yoga")))
	{
		YogaComponent->EndYogaSession();
	}

	if (GroomingComponent && (ActivityId.ToString().Contains(TEXT("shower")) || ActivityId.ToString().Contains(TEXT("groom")) || ActivityId.ToString().Contains(TEXT("hygiene"))))
	{
		GroomingComponent->AdvanceStep();
	}

	if (BedroomRoutineComponent)
	{
		BedroomRoutineComponent->EndRoutine();
	}
}

void AApartmentLifeSimCharacter::HandleScheduleOccasion(const FApartmentLifeGameTime& NewTime)
{
	if (!WardrobeComponent || !SimulationComponent)
	{
		return;
	}

	EApartmentLifeOccasion Occasion = EApartmentLifeOccasion::Everyday;
	if (NewTime.Hour >= 9 && NewTime.Hour < 17)
	{
		Occasion = EApartmentLifeOccasion::Work;
	}
	else if (NewTime.Hour >= 22 || NewTime.Hour < 6)
	{
		Occasion = EApartmentLifeOccasion::Sleep;
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			WardrobeComponent->SelectOutfitForContext(Occasion, TimeSubsystem->GetCurrentWeather());
		}
	}

	RefreshNPCStyleFromSimulation();
}

void AApartmentLifeSimCharacter::HandleDayAdvanced(const FApartmentLifeGameTime& NewTime)
{
	(void)NewTime;
	if (ProgressionComponent && SimulationComponent)
	{
		UApartmentLifeFinanceLibrary::AdvanceDailyBudget(ProgressionComponent, SimulationComponent);
	}
}

void AApartmentLifeSimCharacter::HandleCreatorStateUpdated(const FApartmentLifeCharacterCreatorState& State)
{
	RefreshClothingFitFromBody();
	RefreshNPCStyleFromSimulation();
}

void AApartmentLifeSimCharacter::HandleBodyFitProfileUpdated(const FApartmentLifeBodyFitProfile& FitProfile)
{
	if (ClothingFitComponent)
	{
		ClothingFitComponent->RefreshClothingFit(FitProfile);
	}
}

void AApartmentLifeSimCharacter::RefreshClothingFitFromBody()
{
	if (BodyCustomizationComponent && ClothingFitComponent)
	{
		const FApartmentLifeBodyFitProfile& FitProfile = BodyCustomizationComponent->GetFitProfile();
		ClothingFitComponent->RefreshClothingFit(FitProfile);
		if (WardrobeComponent)
		{
			ClothingFitComponent->ValidateEquippedFit(WardrobeComponent, FitProfile);
		}
	}
}

void AApartmentLifeSimCharacter::HandlePurchasedItem(FName ItemId)
{
	if (!WardrobeComponent)
	{
		return;
	}

	FApartmentLifeBuiltinClothingItem Item;
	if (UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(ItemId, Item))
	{
		WardrobeComponent->AddOwnedClothing(ItemId);
	}
}

void AApartmentLifeSimCharacter::HandleWardrobeUpdated()
{
	RefreshClothingFitFromBody();
}

bool AApartmentLifeSimCharacter::StartMorningRoutine()
{
	return RoutineChainComponent && RoutineChainComponent->StartRoutineChain(FName(TEXT("routine.morning")));
}

bool AApartmentLifeSimCharacter::StartEveningRoutine()
{
	return RoutineChainComponent && RoutineChainComponent->StartRoutineChain(FName(TEXT("routine.evening")));
}

void AApartmentLifeSimCharacter::RefreshNPCStyleFromSimulation()
{
	if (!NPCStyleComponent || !SimulationComponent)
	{
		return;
	}

	FApartmentLifeNPCStyleContext Context;
	Context.Income = SimulationComponent->GetCareer().ComputeMonthlyIncome();
	Context.CareerId = SimulationComponent->GetCareer().CareerId;
	Context.Mood = SimulationComponent->GetMood().OverallMood;
	Context.bInRelationship = SimulationComponent->HasCloseRelationship();

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			Context.Weather = TimeSubsystem->GetCurrentWeather();
			const FApartmentLifeGameTime& Time = TimeSubsystem->GetCurrentTime();
			if (Time.Hour >= 9 && Time.Hour < 17)
			{
				Context.Occasion = EApartmentLifeOccasion::Work;
			}
			else if (Time.Hour >= 22 || Time.Hour < 6)
			{
				Context.Occasion = EApartmentLifeOccasion::Sleep;
			}
		}
	}

	NPCStyleComponent->ApplyStyleForContext(Context);
}

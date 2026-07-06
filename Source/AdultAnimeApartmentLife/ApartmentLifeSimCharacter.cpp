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
}

void AApartmentLifeSimCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationComponent)
	{
		SimulationComponent->OnActivityChanged.AddDynamic(this, &AApartmentLifeSimCharacter::HandleActivityChanged);
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
		AnimationComponent->SetAnimationGroup(UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(ActivityId));
	}

	if (ActivityId.ToString().Contains(TEXT("social")) || ActivityId.ToString().Contains(TEXT("talk")))
	{
		if (AnimationComponent)
		{
			AnimationComponent->SetAnimationGroup(EApartmentLifeAnimationGroup::Conversation);
		}
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
		ClothingFitComponent->RefreshClothingFit(BodyCustomizationComponent->GetFitProfile());
	}
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

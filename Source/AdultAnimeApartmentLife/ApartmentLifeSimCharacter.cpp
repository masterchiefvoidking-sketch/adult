// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSimCharacter.h"

AApartmentLifeSimCharacter::AApartmentLifeSimCharacter()
{
	SimulationComponent = CreateDefaultSubobject<UApartmentLifeNPCSimulationComponent>(TEXT("Simulation"));
	WardrobeComponent = CreateDefaultSubobject<UApartmentLifeWardrobeComponent>(TEXT("Wardrobe"));
	ActivityComponent = CreateDefaultSubobject<UApartmentLifeActivityComponent>(TEXT("Activity"));
}

void AApartmentLifeSimCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SimulationComponent)
	{
		SimulationComponent->OnActivityChanged.AddDynamic(this, &AApartmentLifeSimCharacter::HandleActivityChanged);
	}

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnHourAdvanced.AddDynamic(this, &AApartmentLifeSimCharacter::HandleScheduleOccasion);
		}
	}
}

void AApartmentLifeSimCharacter::HandleActivityChanged(FName ActivityId)
{
	if (ActivityComponent && !ActivityId.IsNone())
	{
		ActivityComponent->StartActivity(ActivityId);
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
}

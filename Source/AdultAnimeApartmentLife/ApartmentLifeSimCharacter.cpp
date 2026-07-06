// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeLifeSimulationComponent.h"
#include "ApartmentLifeActivityComponent.h"

AApartmentLifeSimCharacter::AApartmentLifeSimCharacter()
{
	WardrobeComponent = CreateDefaultSubobject<UApartmentLifeWardrobeComponent>(TEXT("Wardrobe"));
	LifeSimulationComponent = CreateDefaultSubobject<UApartmentLifeLifeSimulationComponent>(TEXT("LifeSimulation"));
	ActivityComponent = CreateDefaultSubobject<UApartmentLifeActivityComponent>(TEXT("Activity"));
}

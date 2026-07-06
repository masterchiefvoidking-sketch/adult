// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterBase.h"
#include "ApartmentLifeCharacterAppearanceComponent.h"

AApartmentLifeCharacterBase::AApartmentLifeCharacterBase()
{
	AppearanceComponent = CreateDefaultSubobject<UApartmentLifeCharacterAppearanceComponent>(TEXT("Appearance"));
}

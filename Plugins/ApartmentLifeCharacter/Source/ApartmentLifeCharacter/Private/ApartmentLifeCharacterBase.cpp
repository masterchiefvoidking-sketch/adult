// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterBase.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeBodyCustomizationComponent.h"

AApartmentLifeCharacterBase::AApartmentLifeCharacterBase()
{
	CreatorComponent = CreateDefaultSubobject<UApartmentLifeCharacterCreatorComponent>(TEXT("Creator"));
	BodyCustomizationComponent = CreateDefaultSubobject<UApartmentLifeBodyCustomizationComponent>(TEXT("Body"));
}

void AApartmentLifeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (BodyCustomizationComponent)
	{
		BodyCustomizationComponent->OnBodyCustomizationUpdated.AddDynamic(this, &AApartmentLifeCharacterBase::HandleBodyCustomizationUpdated);
		OnBodyFitProfileUpdated(BodyCustomizationComponent->GetFitProfile());
	}
}

void AApartmentLifeCharacterBase::HandleBodyCustomizationUpdated(const FApartmentLifeBodyFitProfile& FitProfile)
{
	OnBodyFitProfileUpdated(FitProfile);
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeNPCStyleComponent.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeClothingItemData.h"

void UApartmentLifeNPCStyleComponent::ApplyStyleForContext(const FApartmentLifeNPCStyleContext& Context)
{
	LastContext = Context;

	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeWardrobeComponent* Wardrobe = Owner->FindComponentByClass<UApartmentLifeWardrobeComponent>())
		{
			FApartmentLifeWardrobeStyleProfile Profile;
			Profile.MonthlyClothingBudget = Context.Income * 0.05f;
			Profile.IncomeTier = FMath::Clamp(Context.Income / 5000.f, 0.2f, 2.f);
			Profile.CareerId = Context.CareerId;

			if (UApartmentLifeCharacterCreatorComponent* Creator = Owner->FindComponentByClass<UApartmentLifeCharacterCreatorComponent>())
			{
				Profile.FavoriteColors = Creator->GetCreatorState().FavoriteColors;
				for (const FName& Tag : Creator->GetCreatorState().PersonalityPresetId.IsNone() ? TArray<FName>() : TArray<FName>{ Creator->GetCreatorState().PersonalityPresetId })
				{
					Profile.PreferredStyleTags.Add(Tag);
				}
			}

			Wardrobe->SetStyleProfile(Profile);

			EApartmentLifeOutfitContext OutfitContext = EApartmentLifeOutfitContext::Everyday;
			if (Context.Occasion == EApartmentLifeOccasion::Work) OutfitContext = EApartmentLifeOutfitContext::Work;
			else if (Context.Occasion == EApartmentLifeOccasion::Athletic) OutfitContext = EApartmentLifeOutfitContext::Athletic;
			else if (Context.Occasion == EApartmentLifeOccasion::FormalEvent) OutfitContext = EApartmentLifeOutfitContext::Formal;

			Wardrobe->SelectOutfitForOutfitContext(OutfitContext, Context.Weather);
		}
	}

	OnNPCStyleUpdated.Broadcast();
}

void UApartmentLifeNPCStyleComponent::RefreshWardrobeFromStyleAI()
{
	ApplyStyleForContext(LastContext);
}

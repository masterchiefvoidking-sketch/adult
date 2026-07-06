// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorLibrary.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeClothingFitComponent.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeCatalogLibrary.h"
#include "ApartmentLifeBodyCustomizationComponent.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeCharacterPipelineTypes.h"

FApartmentLifeCreatorCompatibilityReport UApartmentLifeCharacterCreatorLibrary::EvaluateWardrobeCompatibility(AActor* Character)
{
	FApartmentLifeCreatorCompatibilityReport Report;
	if (!Character)
	{
		return Report;
	}

	const UApartmentLifeClothingFitComponent* Fit = Character->FindComponentByClass<UApartmentLifeClothingFitComponent>();
	const UApartmentLifeWardrobeComponent* Wardrobe = Character->FindComponentByClass<UApartmentLifeWardrobeComponent>();
	const UApartmentLifeBodyCustomizationComponent* Body = Character->FindComponentByClass<UApartmentLifeBodyCustomizationComponent>();
	if (!Fit || !Wardrobe || !Body)
	{
		return Report;
	}

	const FApartmentLifeBodyFitProfile& Profile = Body->GetFitProfile();
	for (const FApartmentLifeEquippedClothingSlot& Slot : Wardrobe->GetEquippedSlots())
	{
		if (!Slot.ClothingItemId.IsNone() && !Fit->DoesBuiltinClothingFit(Slot.ClothingItemId, Profile))
		{
			Report.bHasClippingWarnings = true;
			Report.InvalidOutfitItemIds.Add(Slot.ClothingItemId);
			FApartmentLifeBuiltinClothingItem Item;
			const FString Label = UApartmentLifeWardrobeCatalogLibrary::TryGetBuiltinItem(Slot.ClothingItemId, Item)
				? Item.DisplayName.ToString() : Slot.ClothingItemId.ToString();
			Report.Warnings.Add(FText::FromString(FString::Printf(TEXT("Poor fit: %s"), *Label)));
		}
	}

	return Report;
}

void UApartmentLifeCharacterCreatorLibrary::PreviewAnimationGroup(AActor* Character, FName AnimationGroupName)
{
	if (!Character)
	{
		return;
	}

	if (UApartmentLifeAnimationComponent* Animation = Character->FindComponentByClass<UApartmentLifeAnimationComponent>())
	{
		const UEnum* Enum = StaticEnum<EApartmentLifeAnimationGroup>();
		if (Enum)
		{
			const int64 Value = Enum->GetValueByName(AnimationGroupName);
			if (Value != INDEX_NONE)
			{
				Animation->SetAnimationGroup(static_cast<EApartmentLifeAnimationGroup>(Value));
			}
		}
	}
}

TArray<FName> UApartmentLifeCharacterCreatorLibrary::GetCreatorPreviewAnimationGroups()
{
	return {
		FName(TEXT("Idle")),
		FName(TEXT("Walking")),
		FName(TEXT("Sitting")),
		FName(TEXT("Yoga")),
		FName(TEXT("Stretching")),
		FName(TEXT("MirrorRoutine")),
		FName(TEXT("Dressing")),
		FName(TEXT("FloorSitting"))
	};
}

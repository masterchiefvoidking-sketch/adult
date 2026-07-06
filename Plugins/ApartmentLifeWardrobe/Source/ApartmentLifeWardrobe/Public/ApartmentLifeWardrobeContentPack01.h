// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeWardrobeTypes.h"

/** MP19 Content Expansion Pack 01 — additional builtin wardrobe items. */
namespace ApartmentLifeWardrobeContentPack01
{
	TArray<FApartmentLifeBuiltinClothingItem> GetExpansionClothing();
	TArray<FName> GetExpansionClothingIds();
}

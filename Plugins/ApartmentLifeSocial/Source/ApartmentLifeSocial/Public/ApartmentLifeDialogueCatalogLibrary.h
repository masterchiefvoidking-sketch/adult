// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeSocialTypes.h"
#include "ApartmentLifeDialogueCatalogLibrary.generated.h"

UCLASS()
class APARTMENTLIFESOCIAL_API UApartmentLifeDialogueCatalogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Dialogue")
	static FApartmentLifeDialogueLine GenerateContextualPlayerLine(const FApartmentLifeDialogueContext& Context);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Dialogue")
	static int32 GetBuiltinDialogueLineCount();
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeNPCStyleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNPCStyleUpdated);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeNPCStyleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|NPC Style")
	void ApplyStyleForContext(const FApartmentLifeNPCStyleContext& Context);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|NPC Style")
	void RefreshWardrobeFromStyleAI();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|NPC Style")
	FOnNPCStyleUpdated OnNPCStyleUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FApartmentLifeNPCStyleContext LastContext;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeGameState.generated.h"

UCLASS()
class APARTMENTLIFECORE_API AApartmentLifeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	FApartmentLifeGameTime GetWorldTime() const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life")
	FApartmentLifeWeatherState GetWorldWeather() const;
};

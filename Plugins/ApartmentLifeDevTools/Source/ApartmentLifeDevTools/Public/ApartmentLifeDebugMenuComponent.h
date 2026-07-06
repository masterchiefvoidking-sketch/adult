// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeDebugMenuComponent.generated.h"

class AApartmentLifeApartmentUnit;
class APlayerController;

/** On-screen private dev overlay for the first vertical slice (F1). */
UCLASS(ClassGroup = Dev, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEDEVTOOLS_API UApartmentLifeDebugMenuComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UApartmentLifeDebugMenuComponent();

	void InitializeContext(
		APlayerController* InController,
		AApartmentLifeApartmentUnit* InApartment,
		AActor* InGirlCharacter);

	void ToggleMenu();
	bool IsMenuOpen() const { return bMenuOpen; }

	void HandleDebugAction(FName ActionName);

protected:
	void ShowMenuOverlay() const;
	AActor* GetGirlActor() const;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwnerController;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AActor> GirlCharacter;

	UPROPERTY(EditAnywhere, Category = "Debug")
	int32 DebugSaveSlot = 0;

	bool bMenuOpen = false;
	int32 OutfitContextIndex = 0;
};

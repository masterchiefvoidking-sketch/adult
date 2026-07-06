// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ApartmentLifePlayerController.h"
#include "ApartmentLifeSlicePlayerController.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;
class UApartmentLifeInteractionComponent;
class UApartmentLifeBuildModeComponent;

/**
 * Player controller for the private vertical slice. Binds interact, build mode,
 * wardrobe, save/load, and camera focus for the full daily loop.
 */
UCLASS()
class ADULTANIMEAPARTMENTLIFE_API AApartmentLifeSlicePlayerController : public AApartmentLifePlayerController
{
	GENERATED_BODY()

public:
	AApartmentLifeSlicePlayerController();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Slice")
	void SetSliceContext(AApartmentLifeApartmentUnit* InApartment, AApartmentLifeSimCharacter* InPlayerCharacter, AApartmentLifeSimCharacter* InPartnerCharacter);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Slice")
	AApartmentLifeSimCharacter* GetPlayerCharacter() const { return PlayerCharacter.Get(); }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Slice")
	AApartmentLifeSimCharacter* GetPartnerCharacter() const { return PartnerCharacter.Get(); }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnInteract();
	void OnToggleBuildMode();
	void OnBuildTopDown();
	void OnOpenWardrobe();
	void OnBuildUndo();
	void OnBuildRedo();
	void OnQuickSave();
	void OnQuickLoad();
	void OnTalkToPartner();

	UApartmentLifeBuildModeComponent* GetBuildMode() const;
	UApartmentLifeInteractionComponent* GetPlayerInteraction() const;

	void FocusCameraOnInteractable(AActor* Target);

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeSimCharacter> PlayerCharacter;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeSimCharacter> PartnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Slice|Save")
	int32 QuickSaveSlot = 0;

	bool bBuildModeActive = false;
};

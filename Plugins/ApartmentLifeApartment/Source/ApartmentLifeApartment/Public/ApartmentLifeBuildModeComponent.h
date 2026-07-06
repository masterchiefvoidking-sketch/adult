// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeBuildModeComponent.generated.h"

class AApartmentLifeApartmentUnit;
class UApartmentLifeFurnitureItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildModeSelectionChanged, FGuid, InstanceId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildLayoutChanged);

/**
 * Full build/decorate mode: select, move, rotate, snap, duplicate, sell, store, recolor, undo/redo.
 */
UCLASS(ClassGroup = Build, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEAPARTMENT_API UApartmentLifeBuildModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	void EnterBuildMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	void ExitBuildMode();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Build")
	bool IsBuildModeActive() const { return bBuildModeActive; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	void SetPlacementMode(EApartmentLifeBuildPlacementMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool SelectInstance(const FGuid& InstanceId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool PlaceItem(FName FurnitureItemId, const FTransform& Transform, EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool MoveSelected(const FTransform& NewTransform);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool RotateSelected(float YawDelta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool DuplicateSelected();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	float SellSelected();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool StoreSelected();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool RecolorSelected(const FLinearColor& NewColor);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool Undo();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Build")
	bool Redo();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Build")
	FOnBuildModeSelectionChanged OnSelectionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Build")
	FOnBuildLayoutChanged OnLayoutChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build")
	float GridSnapSize = 25.f;

protected:
	AApartmentLifeApartmentUnit* GetApartmentUnit() const;
	void PushUndoAction(const FString& ActionType, const FApartmentLifePlacedFurnitureInstance& Before, const FApartmentLifePlacedFurnitureInstance& After);
	FTransform ApplyPlacementMode(const FTransform& Transform) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build")
	bool bBuildModeActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build")
	EApartmentLifeBuildPlacementMode PlacementMode = EApartmentLifeBuildPlacementMode::SnapToGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build")
	FGuid SelectedInstanceId;

	TArray<FApartmentLifeBuildAction> UndoStack;
	TArray<FApartmentLifeBuildAction> RedoStack;
	static constexpr int32 MaxUndoDepth = 50;
};

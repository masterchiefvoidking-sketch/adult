// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "ApartmentLifeInventoryComponent.h"

void UApartmentLifeBuildModeComponent::EnterBuildMode()
{
	bBuildModeActive = true;
}

void UApartmentLifeBuildModeComponent::ExitBuildMode()
{
	bBuildModeActive = false;
	SelectedInstanceId = FGuid();
}

void UApartmentLifeBuildModeComponent::SetPlacementMode(EApartmentLifeBuildPlacementMode Mode)
{
	PlacementMode = Mode;
}

bool UApartmentLifeBuildModeComponent::SelectInstance(const FGuid& InstanceId)
{
	SelectedInstanceId = InstanceId;
	OnSelectionChanged.Broadcast(InstanceId);
	return InstanceId.IsValid();
}

bool UApartmentLifeBuildModeComponent::PlaceItem(FName FurnitureItemId, const FTransform& Transform, EApartmentLifeRoomType Room)
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit)
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Instance;
	Instance.InstanceId = FGuid::NewGuid();
	Instance.FurnitureItemId = FurnitureItemId;
	Instance.Transform = ApplyPlacementMode(Transform);
	Instance.Room = Room;
	Instance.State = EApartmentLifeInventoryItemState::Placed;

	FApartmentLifePlacedFurnitureInstance EmptyBefore;
	PushUndoAction(TEXT("Place"), EmptyBefore, Instance);

	const bool bSuccess = Unit->PlaceFurnitureInstance(Instance);
	if (bSuccess)
	{
		if (UApartmentLifeInventoryComponent* Inventory = Unit->FindComponentByClass<UApartmentLifeInventoryComponent>())
		{
			const FGuid EntryId = Inventory->AddItem(FurnitureItemId, EApartmentLifeInventoryItemState::Placed);
			Inventory->LinkToPlacement(EntryId, Instance.InstanceId);
		}
		OnLayoutChanged.Broadcast();
	}
	return bSuccess;
}

bool UApartmentLifeBuildModeComponent::MoveSelected(const FTransform& NewTransform)
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Before;
	if (!Unit->GetFurnitureInstance(SelectedInstanceId, Before))
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance After = Before;
	After.Transform = ApplyPlacementMode(NewTransform);
	PushUndoAction(TEXT("Move"), Before, After);

	const bool bSuccess = Unit->UpdateFurnitureInstance(After);
	if (bSuccess)
	{
		OnLayoutChanged.Broadcast();
	}
	return bSuccess;
}

bool UApartmentLifeBuildModeComponent::RotateSelected(float YawDelta)
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Before;
	if (!Unit->GetFurnitureInstance(SelectedInstanceId, Before))
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance After = Before;
	FRotator Rotation = After.Transform.Rotator();
	Rotation.Yaw += YawDelta;
	After.Transform.SetRotation(Rotation.Quaternion());
	After.Transform = ApplyPlacementMode(After.Transform);

	PushUndoAction(TEXT("Rotate"), Before, After);
	return Unit->UpdateFurnitureInstance(After);
}

bool UApartmentLifeBuildModeComponent::DuplicateSelected()
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Source;
	if (!Unit->GetFurnitureInstance(SelectedInstanceId, Source))
	{
		return false;
	}

	FTransform OffsetTransform = Source.Transform;
	OffsetTransform.AddToTranslation(FVector(GridSnapSize, 0.f, 0.f));
	return PlaceItem(Source.FurnitureItemId, OffsetTransform, Source.Room);
}

float UApartmentLifeBuildModeComponent::SellSelected()
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return 0.f;
	}

	const float SellValue = Unit->SellFurnitureInstance(SelectedInstanceId);
	if (SellValue > 0.f)
	{
		SelectedInstanceId = FGuid();
		OnLayoutChanged.Broadcast();
	}
	return SellValue;
}

bool UApartmentLifeBuildModeComponent::StoreSelected()
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Before;
	if (!Unit->GetFurnitureInstance(SelectedInstanceId, Before))
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance After = Before;
	After.State = EApartmentLifeInventoryItemState::Stored;
	PushUndoAction(TEXT("Store"), Before, After);

	Unit->RemoveFurnitureInstance(SelectedInstanceId, false);

	if (UApartmentLifeInventoryComponent* Inventory = Unit->FindComponentByClass<UApartmentLifeInventoryComponent>())
	{
		for (const FApartmentLifeInventoryEntry& Entry : Inventory->GetAllItems())
		{
			if (Entry.LinkedPlacementId == Before.InstanceId)
			{
				Inventory->SetItemState(Entry.EntryId, EApartmentLifeInventoryItemState::Stored);
				break;
			}
		}
	}

	SelectedInstanceId = FGuid();
	OnLayoutChanged.Broadcast();
	return true;
}

bool UApartmentLifeBuildModeComponent::RecolorSelected(const FLinearColor& NewColor)
{
	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit || !SelectedInstanceId.IsValid())
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance Before;
	if (!Unit->GetFurnitureInstance(SelectedInstanceId, Before))
	{
		return false;
	}

	FApartmentLifePlacedFurnitureInstance After = Before;
	After.ColorOverride = NewColor;
	PushUndoAction(TEXT("Recolor"), Before, After);
	return Unit->UpdateFurnitureInstance(After);
}

bool UApartmentLifeBuildModeComponent::Undo()
{
	if (UndoStack.Num() == 0)
	{
		return false;
	}

	FApartmentLifeBuildAction Action = UndoStack.Pop();
	RedoStack.Add(Action);

	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit)
	{
		return false;
	}

	if (Action.ActionType == TEXT("Place"))
	{
		Unit->RemoveFurnitureInstance(Action.AfterState.InstanceId, false);
	}
	else if (Action.BeforeState.InstanceId.IsValid())
	{
		Unit->UpdateFurnitureInstance(Action.BeforeState);
	}

	OnLayoutChanged.Broadcast();
	return true;
}

bool UApartmentLifeBuildModeComponent::Redo()
{
	if (RedoStack.Num() == 0)
	{
		return false;
	}

	FApartmentLifeBuildAction Action = RedoStack.Pop();
	UndoStack.Add(Action);

	AApartmentLifeApartmentUnit* Unit = GetApartmentUnit();
	if (!Unit)
	{
		return false;
	}

	if (Action.ActionType == TEXT("Place"))
	{
		Unit->PlaceFurnitureInstance(Action.AfterState);
	}
	else
	{
		Unit->UpdateFurnitureInstance(Action.AfterState);
	}

	OnLayoutChanged.Broadcast();
	return true;
}

AApartmentLifeApartmentUnit* UApartmentLifeBuildModeComponent::GetApartmentUnit() const
{
	return Cast<AApartmentLifeApartmentUnit>(GetOwner());
}

void UApartmentLifeBuildModeComponent::PushUndoAction(const FString& ActionType, const FApartmentLifePlacedFurnitureInstance& Before, const FApartmentLifePlacedFurnitureInstance& After)
{
	FApartmentLifeBuildAction Action;
	Action.ActionType = ActionType;
	Action.BeforeState = Before;
	Action.AfterState = After;
	Action.bValid = true;
	UndoStack.Add(Action);
	RedoStack.Empty();

	if (UndoStack.Num() > MaxUndoDepth)
	{
		UndoStack.RemoveAt(0);
	}
}

FTransform UApartmentLifeBuildModeComponent::ApplyPlacementMode(const FTransform& Transform) const
{
	if (PlacementMode == EApartmentLifeBuildPlacementMode::SnapToGrid)
	{
		return UApartmentLifeBuilderLibrary::SnapTransformToGrid(Transform, GridSnapSize);
	}
	return Transform;
}

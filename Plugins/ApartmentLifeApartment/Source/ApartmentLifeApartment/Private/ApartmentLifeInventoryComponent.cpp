// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeInventoryComponent.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "ApartmentLifeFurnitureData.h"
#include "JsonObjectConverter.h"

FGuid UApartmentLifeInventoryComponent::AddItem(FName FurnitureItemId, EApartmentLifeInventoryItemState InitialState)
{
	FApartmentLifeInventoryEntry Entry;
	Entry.EntryId = FGuid::NewGuid();
	Entry.FurnitureItemId = FurnitureItemId;
	Entry.State = InitialState;
	Items.Add(Entry);
	OnInventoryChanged.Broadcast(Entry.EntryId);
	return Entry.EntryId;
}

bool UApartmentLifeInventoryComponent::RemoveItem(const FGuid& EntryId)
{
	const int32 Removed = Items.RemoveAll([&EntryId](const FApartmentLifeInventoryEntry& E) { return E.EntryId == EntryId; });
	if (Removed > 0)
	{
		OnInventoryChanged.Broadcast(EntryId);
		return true;
	}
	return false;
}

bool UApartmentLifeInventoryComponent::SetItemState(const FGuid& EntryId, EApartmentLifeInventoryItemState NewState)
{
	if (FApartmentLifeInventoryEntry* Entry = FindEntry(EntryId))
	{
		Entry->State = NewState;
		OnInventoryChanged.Broadcast(EntryId);
		return true;
	}
	return false;
}

bool UApartmentLifeInventoryComponent::LinkToPlacement(const FGuid& EntryId, const FGuid& PlacementId)
{
	if (FApartmentLifeInventoryEntry* Entry = FindEntry(EntryId))
	{
		Entry->LinkedPlacementId = PlacementId;
		Entry->State = EApartmentLifeInventoryItemState::Placed;
		OnInventoryChanged.Broadcast(EntryId);
		return true;
	}
	return false;
}

TArray<FApartmentLifeInventoryEntry> UApartmentLifeInventoryComponent::GetItemsByState(EApartmentLifeInventoryItemState State) const
{
	TArray<FApartmentLifeInventoryEntry> Result;
	for (const FApartmentLifeInventoryEntry& Entry : Items)
	{
		if (Entry.State == State)
		{
			Result.Add(Entry);
		}
	}
	return Result;
}

TArray<FApartmentLifeInventoryEntry> UApartmentLifeInventoryComponent::GetItemsByCategory(EApartmentLifeFurnitureCategory Category) const
{
	TArray<FApartmentLifeInventoryEntry> Result;
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				for (const FApartmentLifeInventoryEntry& Entry : Items)
				{
					if (UApartmentLifeFurnitureItemData* Data = Cast<UApartmentLifeFurnitureItemData>(Registry->FindAsset(Entry.FurnitureItemId)))
					{
						if (Data->Category == Category)
						{
							Result.Add(Entry);
						}
					}
				}
			}
		}
	}
	return Result;
}

TArray<FApartmentLifeInventoryEntry> UApartmentLifeInventoryComponent::SearchItems(const FString& Query) const
{
	TArray<FApartmentLifeInventoryEntry> Result;
	const FString LowerQuery = Query.ToLower();
	for (const FApartmentLifeInventoryEntry& Entry : Items)
	{
		if (Entry.FurnitureItemId.ToString().Contains(LowerQuery))
		{
			Result.Add(Entry);
		}
	}
	Result.Sort([](const FApartmentLifeInventoryEntry& A, const FApartmentLifeInventoryEntry& B)
	{
		return A.FurnitureItemId.LexicalLess(B.FurnitureItemId);
	});
	return Result;
}

FString UApartmentLifeInventoryComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("inventory_%s"), *Owner->GetName());
	}
	return TEXT("inventory_unknown");
}

void UApartmentLifeInventoryComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("ItemCount"), FString::FromInt(Items.Num()));
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(Items, Json);
	OutData.Add(TEXT("Items"), Json);
}

void UApartmentLifeInventoryComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Json = InData.Find(TEXT("Items")))
	{
		FJsonObjectConverter::JsonArrayStringToUStruct(*Json, &Items);
	}
}

FApartmentLifeInventoryEntry* UApartmentLifeInventoryComponent::FindEntry(const FGuid& EntryId)
{
	for (FApartmentLifeInventoryEntry& Entry : Items)
	{
		if (Entry.EntryId == EntryId)
		{
			return &Entry;
		}
	}
	return nullptr;
}

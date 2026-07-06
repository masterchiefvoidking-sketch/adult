// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeFurniturePlacementComponent.h"
#include "ApartmentLifeInventoryComponent.h"
#include "ApartmentLifeBuildModeComponent.h"
#include "ApartmentLifeCleaningComponent.h"
#include "ApartmentLifeDecorationComponent.h"
#include "ApartmentLifeBuilderLibrary.h"
#include "ApartmentLifeGameTimeSubsystem.h"
#include "ApartmentLifeDataRegistrySubsystem.h"
#include "Components/SceneComponent.h"
#include "JsonObjectConverter.h"

AApartmentLifeApartmentUnit::AApartmentLifeApartmentUnit()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PlacementComponent = CreateDefaultSubobject<UApartmentLifeFurniturePlacementComponent>(TEXT("Placement"));
	PlacementComponent->SetupAttachment(RootComponent);
	InventoryComponent = CreateDefaultSubobject<UApartmentLifeInventoryComponent>(TEXT("Inventory"));
	BuildModeComponent = CreateDefaultSubobject<UApartmentLifeBuildModeComponent>(TEXT("BuildMode"));
	CleaningComponent = CreateDefaultSubobject<UApartmentLifeCleaningComponent>(TEXT("Cleaning"));
	DecorationComponent = CreateDefaultSubobject<UApartmentLifeDecorationComponent>(TEXT("Decoration"));
	ApartmentId = FName(TEXT("Apartment_Default"));
	Metadata.LayoutType = EApartmentLifeLayoutType::OneBedroom;
	Metadata.SquareFootage = 650.f;
}

void AApartmentLifeApartmentUnit::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UApartmentLifeGameTimeSubsystem* TimeSubsystem = World->GetSubsystem<UApartmentLifeGameTimeSubsystem>())
		{
			TimeSubsystem->OnHourAdvanced.AddDynamic(this, &AApartmentLifeApartmentUnit::HandleHourAdvanced);
		}
	}

	RecalculateRoomScores();
}

void AApartmentLifeApartmentUnit::InitializeFromLayout(UApartmentLifeApartmentLayoutData* InLayoutData)
{
	LayoutData = InLayoutData;
	if (LayoutData)
	{
		Metadata = LayoutData->Metadata;
		if (BuildModeComponent)
		{
			BuildModeComponent->GridSnapSize = LayoutData->GridSnapSize;
		}
	}
}

bool AApartmentLifeApartmentUnit::PlaceFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance)
{
	FApartmentLifePlacedFurnitureInstance NewInstance = Instance;
	if (!NewInstance.InstanceId.IsValid())
	{
		NewInstance.InstanceId = FGuid::NewGuid();
	}

	UApartmentLifeFurnitureItemData* ItemData = ResolveFurnitureData(NewInstance.FurnitureItemId);
	FBox RoomBounds(FVector(-300.f), FVector(300.f));
	if (LayoutData)
	{
		for (UApartmentLifeRoomDefinitionData* Room : LayoutData->Rooms)
		{
			if (Room && Room->RoomType == NewInstance.Room)
			{
				RoomBounds = Room->Bounds;
				break;
			}
		}
	}

	if (ItemData && !UApartmentLifeBuilderLibrary::ValidatePlacement(ItemData, NewInstance.Transform, NewInstance.Room, RoomBounds, PlacedFurniture))
	{
		return false;
	}

	PlacedFurniture.Add(NewInstance);
	if (ItemData)
	{
		UsedFloorSpace += ItemData->FootprintSize.X * ItemData->FootprintSize.Y;
	}

	if (PlacementComponent)
	{
		PlacementComponent->SpawnFurnitureInstance(NewInstance);
	}

	RecalculateRoomScores();
	return true;
}

bool AApartmentLifeApartmentUnit::UpdateFurnitureInstance(const FApartmentLifePlacedFurnitureInstance& Instance)
{
	for (FApartmentLifePlacedFurnitureInstance& Existing : PlacedFurniture)
	{
		if (Existing.InstanceId == Instance.InstanceId)
		{
			Existing = Instance;
			if (PlacementComponent)
			{
				PlacementComponent->UpdateFurnitureInstance(Instance);
			}
			RecalculateRoomScores();
			return true;
		}
	}
	return false;
}

bool AApartmentLifeApartmentUnit::RemoveFurnitureInstance(const FGuid& InstanceId, bool bAddToInventory)
{
	for (int32 Index = 0; Index < PlacedFurniture.Num(); ++Index)
	{
		if (PlacedFurniture[Index].InstanceId == InstanceId)
		{
			if (UApartmentLifeFurnitureItemData* ItemData = ResolveFurnitureData(PlacedFurniture[Index].FurnitureItemId))
			{
				UsedFloorSpace -= ItemData->FootprintSize.X * ItemData->FootprintSize.Y;
			}

			if (PlacementComponent)
			{
				PlacementComponent->DestroyFurnitureInstance(InstanceId);
			}

			if (bAddToInventory && InventoryComponent)
			{
				InventoryComponent->AddItem(PlacedFurniture[Index].FurnitureItemId, EApartmentLifeInventoryItemState::Stored);
			}

			PlacedFurniture.RemoveAt(Index);
			RecalculateRoomScores();
			return true;
		}
	}
	return false;
}

bool AApartmentLifeApartmentUnit::GetFurnitureInstance(const FGuid& InstanceId, FApartmentLifePlacedFurnitureInstance& OutInstance) const
{
	for (const FApartmentLifePlacedFurnitureInstance& Instance : PlacedFurniture)
	{
		if (Instance.InstanceId == InstanceId)
		{
			OutInstance = Instance;
			return true;
		}
	}
	return false;
}

float AApartmentLifeApartmentUnit::SellFurnitureInstance(const FGuid& InstanceId)
{
	FApartmentLifePlacedFurnitureInstance Instance;
	if (!GetFurnitureInstance(InstanceId, Instance))
	{
		return 0.f;
	}

	UApartmentLifeFurnitureItemData* ItemData = ResolveFurnitureData(Instance.FurnitureItemId);
	const float SellValue = UApartmentLifeBuilderLibrary::GetSellPrice(ItemData, Instance.Condition);
	RemoveFurnitureInstance(InstanceId, false);

	if (InventoryComponent)
	{
		for (const FApartmentLifeInventoryEntry& Entry : InventoryComponent->GetAllItems())
		{
			if (Entry.LinkedPlacementId == InstanceId)
			{
				InventoryComponent->SetItemState(Entry.EntryId, EApartmentLifeInventoryItemState::Sold);
				break;
			}
		}
	}

	return SellValue;
}

void AApartmentLifeApartmentUnit::ApplyUpgrade(const UApartmentLifeApartmentUpgradeData* Upgrade)
{
	if (!Upgrade)
	{
		return;
	}

	for (EApartmentLifeRoomType Room : Upgrade->AffectedRooms)
	{
		if (FApartmentLifeRoomScore* Score = RoomScores.Find(Room))
		{
			Score->Comfort += Upgrade->ComfortBonus;
			Score->LightingQuality += Upgrade->LightingBonus;
			Score->Functionality += Upgrade->FunctionalityBonus;
			OnRoomScoresUpdated.Broadcast(Room);
		}
	}
}

void AApartmentLifeApartmentUnit::RecalculateRoomScores()
{
	const TArray<UApartmentLifeFurnitureItemData*> Resolved = ResolveAllPlacedItems();

	static const EApartmentLifeRoomType AllRooms[] = {
		EApartmentLifeRoomType::LivingRoom, EApartmentLifeRoomType::Kitchen, EApartmentLifeRoomType::Bedroom,
		EApartmentLifeRoomType::Bathroom, EApartmentLifeRoomType::Office, EApartmentLifeRoomType::Balcony
	};

	for (EApartmentLifeRoomType Room : AllRooms)
	{
		float BaseLighting = 50.f;
		if (LayoutData)
		{
			for (UApartmentLifeRoomDefinitionData* RoomDef : LayoutData->Rooms)
			{
				if (RoomDef && RoomDef->RoomType == Room)
				{
					BaseLighting = RoomDef->BaseLighting;
					break;
				}
			}
		}

		const FApartmentLifeRoomScore Score = UApartmentLifeBuilderLibrary::CalculateRoomScore(
			Room, PlacedFurniture, Resolved, PreferredStyles, BaseLighting, Metadata.NaturalLight);
		RoomScores.Add(Room, Score);
		OnRoomScoresUpdated.Broadcast(Room);
	}
}

FApartmentLifeRoomScore AApartmentLifeApartmentUnit::GetRoomScore(EApartmentLifeRoomType Room) const
{
	if (const FApartmentLifeRoomScore* Found = RoomScores.Find(Room))
	{
		return *Found;
	}
	return FApartmentLifeRoomScore();
}

float AApartmentLifeApartmentUnit::GetRemainingFloorSpace() const
{
	const float TotalSpace = Metadata.SquareFootage * 10.f;
	return FMath::Max(0.f, TotalSpace - UsedFloorSpace);
}

void AApartmentLifeApartmentUnit::PlaceFurniture(const FApartmentLifeFurniturePlacement& Placement)
{
	FApartmentLifePlacedFurnitureInstance Instance;
	Instance.InstanceId = Placement.PlacementGuid.IsValid() ? Placement.PlacementGuid : FGuid::NewGuid();
	Instance.FurnitureItemId = Placement.FurnitureItemId;
	Instance.Transform = Placement.Transform;
	Instance.Room = Placement.Room;
	Instance.ColorOverride = Placement.Recolor;
	PlaceFurnitureInstance(Instance);
}

bool AApartmentLifeApartmentUnit::RemoveFurniture(const FGuid& PlacementGuid)
{
	return RemoveFurnitureInstance(PlacementGuid, true);
}

void AApartmentLifeApartmentUnit::HandleHourAdvanced(const FApartmentLifeGameTime& NewTime)
{
	if (!CleaningComponent)
	{
		return;
	}

	const float Tidiness = DecorationComponent ? DecorationComponent->GetDecorationProfile().Tidiness : 0.5f;
	for (FApartmentLifePlacedFurnitureInstance& Instance : PlacedFurniture)
	{
		UApartmentLifeBuilderLibrary::ApplyWearAndDirt(Instance.Condition, 1.f, Tidiness);
	}

	CleaningComponent->AutoCleanByTidiness(PlacedFurniture, Tidiness, 60.f);
	RecalculateRoomScores();
}

UApartmentLifeFurnitureItemData* AApartmentLifeApartmentUnit::ResolveFurnitureData(FName ItemId) const
{
	if (const UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UApartmentLifeDataRegistrySubsystem* Registry = GI->GetSubsystem<UApartmentLifeDataRegistrySubsystem>())
			{
				return Cast<UApartmentLifeFurnitureItemData>(Registry->FindAsset(ItemId));
			}
		}
	}
	return nullptr;
}

TArray<UApartmentLifeFurnitureItemData*> AApartmentLifeApartmentUnit::ResolveAllPlacedItems() const
{
	TArray<UApartmentLifeFurnitureItemData*> Result;
	for (const FApartmentLifePlacedFurnitureInstance& Instance : PlacedFurniture)
	{
		Result.Add(ResolveFurnitureData(Instance.FurnitureItemId));
	}
	return Result;
}

FString AApartmentLifeApartmentUnit::GetSaveId_Implementation() const
{
	return FString::Printf(TEXT("apartment_%s"), *ApartmentId.ToString());
}

void AApartmentLifeApartmentUnit::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("ApartmentId"), ApartmentId.ToString());

	FString FurnitureJson;
	FJsonObjectConverter::UStructToJsonObjectString(PlacedFurniture, FurnitureJson);
	OutData.Add(TEXT("PlacedFurniture"), FurnitureJson);

	FString MetadataJson;
	FJsonObjectConverter::UStructToJsonObjectString(Metadata, MetadataJson);
	OutData.Add(TEXT("Metadata"), MetadataJson);
}

void AApartmentLifeApartmentUnit::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Id = InData.Find(TEXT("ApartmentId")))
	{
		ApartmentId = FName(**Id);
	}
	if (const FString* FurnitureJson = InData.Find(TEXT("PlacedFurniture")))
	{
		FJsonObjectConverter::JsonArrayStringToUStruct(*FurnitureJson, &PlacedFurniture);
		for (const FApartmentLifePlacedFurnitureInstance& Instance : PlacedFurniture)
		{
			if (PlacementComponent)
			{
				PlacementComponent->SpawnFurnitureInstance(Instance);
			}
		}
	}
	if (const FString* MetadataJson = InData.Find(TEXT("Metadata")))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(*MetadataJson, &Metadata);
	}

	RecalculateRoomScores();
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeFurnitureData.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeFurnitureAction : uint8
{
	Move		UMETA(DisplayName = "Move"),
	Rotate		UMETA(DisplayName = "Rotate"),
	Scale		UMETA(DisplayName = "Scale"),
	Replace		UMETA(DisplayName = "Replace"),
	Recolor		UMETA(DisplayName = "Recolor")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeFurniturePlacement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
	FName FurnitureItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
	FLinearColor Recolor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
	EApartmentLifeRoomType Room = EApartmentLifeRoomType::LivingRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
	FGuid PlacementGuid;
};

UCLASS(BlueprintType)
class APARTMENTLIFEAPARTMENT_API UApartmentLifeFurnitureItemData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("FurnitureItem")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
	bool bAllowScale = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
	TArray<EApartmentLifeFurnitureAction> SupportedActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
	TArray<EApartmentLifeRoomType> AllowedRooms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
	float Price = 100.f;
};

UCLASS(BlueprintType)
class APARTMENTLIFEAPARTMENT_API UApartmentLifeRoomDefinitionData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("RoomDefinition")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	EApartmentLifeRoomType RoomType = EApartmentLifeRoomType::LivingRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	FBox Bounds = FBox(FVector(-200.f), FVector(200.f));
};

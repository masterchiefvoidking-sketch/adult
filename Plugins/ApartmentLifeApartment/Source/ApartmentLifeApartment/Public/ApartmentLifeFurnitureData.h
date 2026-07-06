// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifePrimaryDataAsset.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeFurnitureData.generated.h"

USTRUCT(BlueprintType)
struct FApartmentLifeColorOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FName ColorId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor Color = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FApartmentLifeMaterialOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	FName MaterialId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterialInterface* Material = nullptr;
};

/** Full data definition for a furnishable object. All gameplay values are authored here. */
UCLASS(BlueprintType)
class APARTMENTLIFEAPARTMENT_API UApartmentLifeFurnitureItemData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("FurnitureItem")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EApartmentLifeFurnitureCategory Category = EApartmentLifeFurnitureCategory::Chair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	float Price = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FVector FootprintSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	EApartmentLifePlacementRule PlacementRule = EApartmentLifePlacementRule::Floor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	TArray<EApartmentLifeRoomType> AllowedRooms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	bool bAllowRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement")
	bool bAllowScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	TArray<EApartmentLifeStyleTag> StyleTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	TArray<FApartmentLifeColorOption> ColorOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style")
	TArray<FApartmentLifeMaterialOption> MaterialOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float ComfortValue = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float CleanlinessImpact = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float DurabilityMax = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float MaintenanceIntervalDays = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float EntertainmentValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float LightingContribution = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<FApartmentLifeInteractionPoint> InteractionPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FName DefaultMontagePrefix;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	float BaseLighting = 50.f;
};

UCLASS(BlueprintType)
class APARTMENTLIFEAPARTMENT_API UApartmentLifeApartmentLayoutData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ApartmentLayout")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	FApartmentLifeApartmentMetadata Metadata;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	TArray<UApartmentLifeRoomDefinitionData*> Rooms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout")
	float GridSnapSize = 25.f;
};

UCLASS(BlueprintType)
class APARTMENTLIFEAPARTMENT_API UApartmentLifeApartmentUpgradeData : public UApartmentLifePrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FName GetPrimaryAssetType() const override { return FName(TEXT("ApartmentUpgrade")); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	EApartmentLifeUpgradeType UpgradeType = EApartmentLifeUpgradeType::Lighting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	float Cost = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	float ComfortBonus = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	float LightingBonus = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	float FunctionalityBonus = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	TArray<EApartmentLifeRoomType> AffectedRooms;
};

// Legacy compatibility
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

UENUM(BlueprintType)
enum class EApartmentLifeFurnitureAction : uint8
{
	Move	UMETA(DisplayName = "Move"),
	Rotate	UMETA(DisplayName = "Rotate"),
	Scale	UMETA(DisplayName = "Scale"),
	Replace	UMETA(DisplayName = "Replace"),
	Recolor	UMETA(DisplayName = "Recolor")
};

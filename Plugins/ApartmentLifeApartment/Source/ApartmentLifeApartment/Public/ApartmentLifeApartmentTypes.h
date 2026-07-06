// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeApartmentTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeLayoutType : uint8
{
	Studio			UMETA(DisplayName = "Studio"),
	OneBedroom		UMETA(DisplayName = "One Bedroom"),
	TwoBedroom		UMETA(DisplayName = "Two Bedroom"),
	Luxury			UMETA(DisplayName = "Luxury"),
	Shared			UMETA(DisplayName = "Shared Apartment")
};

UENUM(BlueprintType)
enum class EApartmentLifeFurnitureCategory : uint8
{
	Bed				UMETA(DisplayName = "Bed"),
	Sofa			UMETA(DisplayName = "Sofa"),
	Chair			UMETA(DisplayName = "Chair"),
	Table			UMETA(DisplayName = "Table"),
	Desk			UMETA(DisplayName = "Desk"),
	TV				UMETA(DisplayName = "TV"),
	Computer		UMETA(DisplayName = "Computer"),
	Bookshelf		UMETA(DisplayName = "Bookshelf"),
	Dresser			UMETA(DisplayName = "Dresser"),
	Closet			UMETA(DisplayName = "Closet"),
	Mirror			UMETA(DisplayName = "Mirror"),
	Rug				UMETA(DisplayName = "Rug"),
	Curtain			UMETA(DisplayName = "Curtain"),
	Plant			UMETA(DisplayName = "Plant"),
	WallArt			UMETA(DisplayName = "Wall Art"),
	Lamp			UMETA(DisplayName = "Lamp"),
	KitchenAppliance UMETA(DisplayName = "Kitchen Appliance"),
	BathroomFixture UMETA(DisplayName = "Bathroom Fixture"),
	LaundryMachine	UMETA(DisplayName = "Laundry Machine"),
	WorkoutEquipment UMETA(DisplayName = "Workout Equipment"),
	StorageBin		UMETA(DisplayName = "Storage Bin"),
	Decoration		UMETA(DisplayName = "Decoration")
};

UENUM(BlueprintType)
enum class EApartmentLifeStyleTag : uint8
{
	Minimalist	UMETA(DisplayName = "Minimalist"),
	Cozy		UMETA(DisplayName = "Cozy"),
	Luxury		UMETA(DisplayName = "Luxury"),
	Gamer		UMETA(DisplayName = "Gamer"),
	Academic	UMETA(DisplayName = "Academic"),
	Artistic	UMETA(DisplayName = "Artistic"),
	Fitness		UMETA(DisplayName = "Fitness"),
	Romantic	UMETA(DisplayName = "Romantic"),
	Traditional	UMETA(DisplayName = "Traditional"),
	Modern		UMETA(DisplayName = "Modern"),
	Budget		UMETA(DisplayName = "Budget"),
	Designer	UMETA(DisplayName = "Designer")
};

UENUM(BlueprintType)
enum class EApartmentLifePlacementRule : uint8
{
	Floor			UMETA(DisplayName = "Floor"),
	Wall			UMETA(DisplayName = "Wall"),
	Ceiling			UMETA(DisplayName = "Ceiling"),
	Surface			UMETA(DisplayName = "Surface"),
	CounterTop		UMETA(DisplayName = "Counter Top"),
	Free			UMETA(DisplayName = "Free Placement")
};

UENUM(BlueprintType)
enum class EApartmentLifeInventoryItemState : uint8
{
	Owned		UMETA(DisplayName = "Owned"),
	Placed		UMETA(DisplayName = "Placed"),
	Stored		UMETA(DisplayName = "Stored"),
	Sold		UMETA(DisplayName = "Sold"),
	Gifted		UMETA(DisplayName = "Gifted"),
	Broken		UMETA(DisplayName = "Broken"),
	Dirty		UMETA(DisplayName = "Dirty"),
	Upgraded	UMETA(DisplayName = "Upgraded")
};

UENUM(BlueprintType)
enum class EApartmentLifeBuildPlacementMode : uint8
{
	SnapToGrid	UMETA(DisplayName = "Snap To Grid"),
	Free		UMETA(DisplayName = "Free Placement"),
	Wall		UMETA(DisplayName = "Wall Placement"),
	Surface		UMETA(DisplayName = "Surface Placement")
};

UENUM(BlueprintType)
enum class EApartmentLifeFurnitureInteraction : uint8
{
	Sleep			UMETA(DisplayName = "Sleep"),
	Nap				UMETA(DisplayName = "Nap"),
	Sit				UMETA(DisplayName = "Sit"),
	Read			UMETA(DisplayName = "Read"),
	WatchTV			UMETA(DisplayName = "Watch TV"),
	Talk			UMETA(DisplayName = "Talk"),
	Relax			UMETA(DisplayName = "Relax"),
	UseComputer		UMETA(DisplayName = "Use Computer"),
	Study			UMETA(DisplayName = "Study"),
	WorkFromHome	UMETA(DisplayName = "Work From Home"),
	Groom			UMETA(DisplayName = "Groom"),
	CheckOutfit		UMETA(DisplayName = "Check Outfit"),
	ChangeOutfit	UMETA(DisplayName = "Change Outfit"),
	ManageWardrobe	UMETA(DisplayName = "Manage Wardrobe"),
	Cook			UMETA(DisplayName = "Cook"),
	Eat				UMETA(DisplayName = "Eat"),
	CleanDishes		UMETA(DisplayName = "Clean Dishes"),
	Shower			UMETA(DisplayName = "Shower"),
	BrushTeeth		UMETA(DisplayName = "Brush Teeth"),
	SkinCare		UMETA(DisplayName = "Skin Care"),
	HairCare		UMETA(DisplayName = "Hair Care"),
	Yoga			UMETA(DisplayName = "Yoga"),
	Stretch			UMETA(DisplayName = "Stretch"),
	Exercise		UMETA(DisplayName = "Exercise"),
	DoLaundry		UMETA(DisplayName = "Do Laundry")
};

UENUM(BlueprintType)
enum class EApartmentLifeDirtType : uint8
{
	Dust		UMETA(DisplayName = "Dust"),
	Laundry		UMETA(DisplayName = "Laundry"),
	Dishes		UMETA(DisplayName = "Dishes"),
	Trash		UMETA(DisplayName = "Trash"),
	Wear		UMETA(DisplayName = "Wear"),
	Broken		UMETA(DisplayName = "Broken")
};

UENUM(BlueprintType)
enum class EApartmentLifeUpgradeType : uint8
{
	Lighting		UMETA(DisplayName = "Better Lighting"),
	Appliances		UMETA(DisplayName = "Improved Appliances"),
	Flooring		UMETA(DisplayName = "New Flooring"),
	Paint			UMETA(DisplayName = "Fresh Paint"),
	SmartHome		UMETA(DisplayName = "Smart Home"),
	Storage			UMETA(DisplayName = "Better Storage"),
	PremiumBed		UMETA(DisplayName = "Premium Bed"),
	Workspace		UMETA(DisplayName = "Better Workspace"),
	ExerciseCorner	UMETA(DisplayName = "Exercise Corner"),
	Entertainment	UMETA(DisplayName = "Entertainment Setup")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeApartmentMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	EApartmentLifeLayoutType LayoutType = EApartmentLifeLayoutType::OneBedroom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float MonthlyRent = 950.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float SquareFootage = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment", meta = (ClampMin = "0", ClampMax = "100"))
	float NeighborhoodQuality = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment", meta = (ClampMin = "0", ClampMax = "100"))
	float NoiseLevel = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment", meta = (ClampMin = "0", ClampMax = "100"))
	float NaturalLight = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float StorageSpace = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float MonthlyUtilityCost = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment", meta = (ClampMin = "0", ClampMax = "100"))
	float SafetyRating = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	float CommuteDistanceKm = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apartment")
	TArray<FName> BuildingRules;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeInteractionPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EApartmentLifeFurnitureInteraction InteractionType = EApartmentLifeFurnitureInteraction::Sit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FName ActivityId;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeObjectCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ClampMin = "0", ClampMax = "100"))
	float Durability = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ClampMin = "0", ClampMax = "100"))
	float Cleanliness = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition", meta = (ClampMin = "0", ClampMax = "100"))
	float DirtLevel = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	bool bNeedsMaintenance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	bool bIsBroken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	TArray<EApartmentLifeDirtType> ActiveDirtTypes;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifePlacedFurnitureInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FGuid InstanceId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FName FurnitureItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	EApartmentLifeRoomType Room = EApartmentLifeRoomType::LivingRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FLinearColor ColorOverride = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FName MaterialVariantId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	EApartmentLifeInventoryItemState State = EApartmentLifeInventoryItemState::Placed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FApartmentLifeObjectCondition Condition;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FGuid EntryId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName FurnitureItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EApartmentLifeInventoryItemState State = EApartmentLifeInventoryItemState::Owned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FApartmentLifeObjectCondition Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FGuid LinkedPlacementId;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeRoomScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	EApartmentLifeRoomType Room = EApartmentLifeRoomType::LivingRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float Comfort = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float Cleanliness = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float StyleMatch = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float Functionality = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float Clutter = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float LightingQuality = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score", meta = (ClampMin = "0", ClampMax = "100"))
	float EntertainmentValue = 50.f;

	APARTMENTLIFEAPARTMENT_API float GetOverallScore() const;
	APARTMENTLIFEAPARTMENT_API float GetMoodImpact() const;
	APARTMENTLIFEAPARTMENT_API float GetSleepQualityImpact() const;
	APARTMENTLIFEAPARTMENT_API float GetProductivityImpact() const;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeDecorationProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float MonthlyBudget = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float Tidiness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float Creativity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float SpendingHabits = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float GamingInterest = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	float FitnessInterest = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoration")
	TArray<EApartmentLifeStyleTag> PreferredStyles;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEAPARTMENT_API FApartmentLifeBuildAction
{
	GENERATED_BODY()

	UPROPERTY()
	FString ActionType;

	UPROPERTY()
	FApartmentLifePlacedFurnitureInstance BeforeState;

	UPROPERTY()
	FApartmentLifePlacedFurnitureInstance AfterState;

	UPROPERTY()
	bool bValid = false;
};

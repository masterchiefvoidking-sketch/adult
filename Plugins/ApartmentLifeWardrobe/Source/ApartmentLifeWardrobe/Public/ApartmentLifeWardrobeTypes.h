// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeWardrobeTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeClothingCategory : uint8
{
	Casual		UMETA(DisplayName = "Casual"),
	Business	UMETA(DisplayName = "Business"),
	Formal		UMETA(DisplayName = "Formal"),
	Athletic	UMETA(DisplayName = "Athletic"),
	Sleepwear	UMETA(DisplayName = "Sleepwear"),
	Outerwear	UMETA(DisplayName = "Outerwear"),
	Shoes		UMETA(DisplayName = "Shoes"),
	Jewelry		UMETA(DisplayName = "Jewelry"),
	Glasses		UMETA(DisplayName = "Glasses"),
	Bags		UMETA(DisplayName = "Bags"),
	Nude		UMETA(DisplayName = "Nude")
};

UENUM(BlueprintType)
enum class EApartmentLifeClothingLayer : uint8
{
	Underwear	UMETA(DisplayName = "Underwear"),
	Base		UMETA(DisplayName = "Base"),
	Mid			UMETA(DisplayName = "Mid"),
	Outer		UMETA(DisplayName = "Outer"),
	Accessory	UMETA(DisplayName = "Accessory")
};

UENUM(BlueprintType)
enum class EApartmentLifeLaundryState : uint8
{
	Clean		UMETA(DisplayName = "Clean"),
	Worn		UMETA(DisplayName = "Worn"),
	Dirty		UMETA(DisplayName = "Dirty"),
	InWash		UMETA(DisplayName = "In Wash"),
	Drying		UMETA(DisplayName = "Drying")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeClothingColorOverride
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName MaterialSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FLinearColor Color = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeEquippedClothingSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName ClothingItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeClothingLayer Layer = EApartmentLifeClothingLayer::Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FApartmentLifeClothingColorOverride> ColorOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeLaundryState LaundryState = EApartmentLifeLaundryState::Clean;
};

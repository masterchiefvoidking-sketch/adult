// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeTypes.h"
#include "ApartmentLifeWardrobeTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeClothingCategory : uint8
{
	Tops			UMETA(DisplayName = "Tops"),
	Bottoms			UMETA(DisplayName = "Bottoms"),
	Dresses			UMETA(DisplayName = "Dresses"),
	Jackets			UMETA(DisplayName = "Jackets"),
	Sleepwear		UMETA(DisplayName = "Sleepwear"),
	AthleticWear	UMETA(DisplayName = "Athletic Wear"),
	FormalWear		UMETA(DisplayName = "Formal Wear"),
	Shoes			UMETA(DisplayName = "Shoes"),
	Socks			UMETA(DisplayName = "Socks"),
	Hats			UMETA(DisplayName = "Hats"),
	Glasses			UMETA(DisplayName = "Glasses"),
	Jewelry			UMETA(DisplayName = "Jewelry"),
	Bags			UMETA(DisplayName = "Bags"),
	// Legacy aliases
	Casual			UMETA(DisplayName = "Casual"),
	Business		UMETA(DisplayName = "Business"),
	Formal			UMETA(DisplayName = "Formal"),
	Athletic		UMETA(DisplayName = "Athletic"),
	Outerwear		UMETA(DisplayName = "Outerwear")
};

UENUM(BlueprintType)
enum class EApartmentLifeClothingLayer : uint8
{
	Underwear	UMETA(DisplayName = "Underwear"),
	Socks		UMETA(DisplayName = "Socks"),
	Bottoms		UMETA(DisplayName = "Bottoms"),
	Tops		UMETA(DisplayName = "Tops"),
	Dress		UMETA(DisplayName = "Dress"),
	Jacket		UMETA(DisplayName = "Jacket"),
	Shoes		UMETA(DisplayName = "Shoes"),
	Hat			UMETA(DisplayName = "Hat"),
	Accessory	UMETA(DisplayName = "Accessory"),
	Glasses		UMETA(DisplayName = "Glasses"),
	Jewelry		UMETA(DisplayName = "Jewelry"),
	Bag			UMETA(DisplayName = "Bag")
};

UENUM(BlueprintType)
enum class EApartmentLifeOutfitContext : uint8
{
	Everyday	UMETA(DisplayName = "Everyday"),
	Work		UMETA(DisplayName = "Work"),
	Lounge		UMETA(DisplayName = "Lounge"),
	Athletic	UMETA(DisplayName = "Athletic"),
	Formal		UMETA(DisplayName = "Formal"),
	Date		UMETA(DisplayName = "Date"),
	Sleep		UMETA(DisplayName = "Sleep")
};

UENUM(BlueprintType)
enum class EApartmentLifeLaundryState : uint8
{
	Clean	UMETA(DisplayName = "Clean"),
	Worn	UMETA(DisplayName = "Worn"),
	Dirty	UMETA(DisplayName = "Dirty"),
	InWash	UMETA(DisplayName = "In Wash"),
	Drying	UMETA(DisplayName = "Drying")
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
struct APARTMENTLIFEWARDROBE_API FApartmentLifeClothingMaterialOverride
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName MaterialSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName MaterialVariantId;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeEquippedClothingSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName ClothingItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeClothingLayer Layer = EApartmentLifeClothingLayer::Tops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FApartmentLifeClothingColorOverride> ColorOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FApartmentLifeClothingMaterialOverride> MaterialOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeLaundryState LaundryState = EApartmentLifeLaundryState::Clean;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeFavoriteOutfit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	FName OutfitNameId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	TArray<FApartmentLifeEquippedClothingSlot> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	EApartmentLifeOutfitContext Context = EApartmentLifeOutfitContext::Everyday;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeWardrobeStyleProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	float MonthlyClothingBudget = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	TArray<FLinearColor> FavoriteColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	TArray<FName> PreferredStyleTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FName CareerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	float IncomeTier = 0.5f;
};

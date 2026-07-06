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

/** MP11 wardrobe category tabs for UI filtering. */
UENUM(BlueprintType)
enum class EApartmentLifeWardrobeCategoryTab : uint8
{
	Casual			UMETA(DisplayName = "Casual"),
	WorkFromHome	UMETA(DisplayName = "Work From Home"),
	Lounge			UMETA(DisplayName = "Lounge"),
	Sleepwear		UMETA(DisplayName = "Sleepwear"),
	Athletic		UMETA(DisplayName = "Athletic / Yoga"),
	Formal			UMETA(DisplayName = "Formal"),
	Seasonal		UMETA(DisplayName = "Seasonal"),
	Accessories		UMETA(DisplayName = "Accessories")
};

/** MP11 clothing slots (maps to layers). */
UENUM(BlueprintType)
enum class EApartmentLifeClothingSlot : uint8
{
	HairAccessory	UMETA(DisplayName = "Hair Accessory"),
	Glasses			UMETA(DisplayName = "Glasses"),
	Earrings		UMETA(DisplayName = "Earrings"),
	Necklace		UMETA(DisplayName = "Necklace"),
	Top				UMETA(DisplayName = "Top"),
	Jacket			UMETA(DisplayName = "Jacket"),
	Bottom			UMETA(DisplayName = "Bottom"),
	Dress			UMETA(DisplayName = "Dress"),
	Socks			UMETA(DisplayName = "Socks"),
	Shoes			UMETA(DisplayName = "Shoes"),
	BraceletWatch	UMETA(DisplayName = "Bracelet / Watch"),
	Bag				UMETA(DisplayName = "Bag")
};

UENUM(BlueprintType)
enum class EApartmentLifeClothingStyleTag : uint8
{
	Cute		UMETA(DisplayName = "Cute"),
	Cozy		UMETA(DisplayName = "Cozy"),
	Elegant		UMETA(DisplayName = "Elegant"),
	Casual		UMETA(DisplayName = "Casual"),
	Sporty		UMETA(DisplayName = "Sporty"),
	Professional UMETA(DisplayName = "Professional"),
	Minimalist	UMETA(DisplayName = "Minimalist"),
	Luxury		UMETA(DisplayName = "Luxury"),
	Artistic	UMETA(DisplayName = "Artistic"),
	Gamer		UMETA(DisplayName = "Gamer"),
	Romantic	UMETA(DisplayName = "Romantic"),
	Seasonal	UMETA(DisplayName = "Seasonal")
};

UENUM(BlueprintType)
enum class EApartmentLifeClothingIncomeTier : uint8
{
	Budget		UMETA(DisplayName = "Budget"),
	MidRange	UMETA(DisplayName = "Mid Range"),
	Premium		UMETA(DisplayName = "Premium"),
	Luxury		UMETA(DisplayName = "Luxury")
};

UENUM(BlueprintType)
enum class EApartmentLifeOutfitPresetType : uint8
{
	Morning		UMETA(DisplayName = "Morning Outfit"),
	Work		UMETA(DisplayName = "Work Outfit"),
	Lounge		UMETA(DisplayName = "Lounge Outfit"),
	Sleep		UMETA(DisplayName = "Sleep Outfit"),
	Yoga		UMETA(DisplayName = "Yoga Outfit"),
	GoingOut	UMETA(DisplayName = "Going Out Outfit"),
	Custom		UMETA(DisplayName = "Custom Favorite")
};

UENUM(BlueprintType)
enum class EApartmentLifeClothingSortMode : uint8
{
	None		UMETA(DisplayName = "None"),
	PriceAsc	UMETA(DisplayName = "Price Low to High"),
	PriceDesc	UMETA(DisplayName = "Price High to Low"),
	Style		UMETA(DisplayName = "Style Match"),
	Owned		UMETA(DisplayName = "Owned First"),
	Favorites	UMETA(DisplayName = "Favorites First")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	EApartmentLifeOutfitPresetType PresetType = EApartmentLifeOutfitPresetType::Custom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	FText DisplayName;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeBuiltinClothingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeClothingLayer Layer = EApartmentLifeClothingLayer::Tops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeWardrobeCategoryTab CategoryTab = EApartmentLifeWardrobeCategoryTab::Casual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	float Price = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	float ComfortRating = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	float WarmthRating = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EApartmentLifeClothingIncomeTier IncomeTier = EApartmentLifeClothingIncomeTier::Budget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<EApartmentLifeClothingStyleTag> StyleTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<EApartmentLifeOutfitContext> SuitableContexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FName> SuitableActivities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<EApartmentLifeSeason> SeasonalTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	bool bYogaCompatible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FName> ColorOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FName> PatternOptions;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeShoppingRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	FName ClothingItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	float PricePaid = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
	int32 DayPurchased = 0;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeOutfitMoodEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float MoodDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float ConfidenceDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float ComfortDelta = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mood")
	float EnergyDelta = 0.f;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEWARDROBE_API FApartmentLifeWardrobeFilterQuery
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	EApartmentLifeWardrobeCategoryTab CategoryTab = EApartmentLifeWardrobeCategoryTab::Casual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bAllCategories = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	EApartmentLifeClothingLayer LayerFilter = EApartmentLifeClothingLayer::Tops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bFilterByLayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	FString SearchText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	EApartmentLifeClothingSortMode SortMode = EApartmentLifeClothingSortMode::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bOwnedOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bFavoritesOnly = false;
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

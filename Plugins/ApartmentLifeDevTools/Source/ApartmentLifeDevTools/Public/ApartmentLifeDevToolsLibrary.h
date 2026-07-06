// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeApartmentTypes.h"
#include "ApartmentLifeWardrobeTypes.h"
#include "ApartmentLifeDevToolsLibrary.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeSimCharacter;
class UApartmentLifeNPCSimulationComponent;

/**
 * Non-shipping development helpers for rapid iteration on the vertical slice.
 */
UCLASS()
class APARTMENTLIFEDEVTOOLS_API UApartmentLifeDevToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool AddMoney(UObject* WorldContextObject, AActor* Character, float Amount);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool SetGameHour(UObject* WorldContextObject, int32 Hour);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool SetMood(AActor* Character, float Mood);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool SetEnergy(AActor* Character, float Energy);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool SetHygiene(AActor* Character, float Hygiene);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool EquipOutfitContext(AActor* Character, EApartmentLifeOutfitContext Context);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool TeleportToRoom(AActor* Character, EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool TriggerActivity(AActor* Character, FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool SpawnFurnitureAtCursor(
		UObject* WorldContextObject,
		AApartmentLifeApartmentUnit* Apartment,
		FName FurnitureItemId,
		EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool ResetApartmentLayout(AApartmentLifeApartmentUnit* Apartment);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool TeleportCharacter(AActor* Character, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool PreviewAnimation(AActor* Character, FName AnimationGroupName);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev")
	static bool PreviewDialogue(AActor* Speaker, AActor* Listener);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool SaveTest(UObject* WorldContextObject, int32 SlotIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Dev", meta = (WorldContext = "WorldContextObject"))
	static bool LoadTest(UObject* WorldContextObject, int32 SlotIndex = 0);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Dev")
	static FVector GetRoomTeleportLocation(EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Dev")
	static FString BuildCharacterDebugSummary(AActor* Character);

	static UApartmentLifeNPCSimulationComponent* GetSimulation(AActor* Character);
};

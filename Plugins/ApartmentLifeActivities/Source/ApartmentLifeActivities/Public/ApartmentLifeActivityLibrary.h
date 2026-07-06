// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeActivityTypes.h"
#include "ApartmentLifeActivityLibrary.generated.h"

class UApartmentLifeActivityComponent;
class UApartmentLifeNPCSimulationComponent;

UCLASS()
class APARTMENTLIFEACTIVITIES_API UApartmentLifeActivityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static bool TryGetDefinition(FName ActivityId, FApartmentLifeActivityDefinition& OutDefinition);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static int32 GetDurationMinutes(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static FText GetDisplayName(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static bool CanStartActivity(
		FName ActivityId,
		EApartmentLifeRoomType CurrentRoom,
		const UApartmentLifeActivityComponent* ActivityComponent);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	static FApartmentLifeActivityCompletionResult ApplyActivityCompletion(
		UApartmentLifeNPCSimulationComponent* Simulation,
		FName ActivityId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Activity")
	static void ApplyActivitySkillGain(class FApartmentLifeSkillSet& Skills, FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static FApartmentLifeActivityCameraFrame GetCameraFrameForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static EApartmentLifePrimaryCameraMode GetCameraModeForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static EApartmentLifeAnimationGroup GetAnimationGroupForActivity(FName ActivityId);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static EApartmentLifeActivityRoom RoomTypeToActivityRoom(EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static EApartmentLifeRoomType ActivityRoomToRoomType(EApartmentLifeActivityRoom Room);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Activity")
	static EApartmentLifeRoomType GetPreferredRoomForActivity(FName ActivityId);
};

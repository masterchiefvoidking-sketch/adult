// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCharacterPipelineTypes.h"
#include "ApartmentLifeYogaMinigameComponent.generated.h"

class UApartmentLifeYogaPoseData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYogaSessionUpdated, const FApartmentLifeYogaSessionState&, State);

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeYogaMinigameComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Yoga")
	bool StartYogaSession(FName PoseId, bool bOnMat);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Yoga")
	void UpdatePoseInput(float InputAccuracy, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Yoga")
	void EndYogaSession();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Yoga")
	const FApartmentLifeYogaSessionState& GetSessionState() const { return SessionState; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Yoga")
	FOnYogaSessionUpdated OnYogaSessionUpdated;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY()
	TObjectPtr<UApartmentLifeYogaPoseData> ActivePose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Yoga")
	FApartmentLifeYogaSessionState SessionState;
};

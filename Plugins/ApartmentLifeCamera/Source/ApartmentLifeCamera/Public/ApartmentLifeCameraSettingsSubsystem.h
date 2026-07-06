// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeCameraTypes.h"
#include "ApartmentLifeCameraSettingsSubsystem.generated.h"

class AApartmentLifeCameraPawn;

UCLASS()
class APARTMENTLIFECAMERA_API UApartmentLifeCameraSettingsSubsystem : public UGameInstanceSubsystem, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void ApplySettingsToPawn(AApartmentLifeCameraPawn* CameraPawn) const;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void CaptureFromPawn(const AApartmentLifeCameraPawn* CameraPawn);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Camera")
	const FApartmentLifeCameraUserSettings& GetSettings() const { return Settings; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Camera")
	void SetSettings(const FApartmentLifeCameraUserSettings& InSettings) { Settings = InSettings; }

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY()
	FApartmentLifeCameraUserSettings Settings;
};

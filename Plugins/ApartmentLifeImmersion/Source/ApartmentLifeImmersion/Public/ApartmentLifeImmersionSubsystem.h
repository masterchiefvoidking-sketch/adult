// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeImmersionTypes.h"
#include "ApartmentLifeImmersionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImmersionStateChanged, const FApartmentLifeImmersionState&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMusicCategoryChanged, EApartmentLifeMusicCategory, NewCategory, FName, MusicId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivitySfxTriggered, FName, SfxId);

UCLASS()
class APARTMENTLIFEIMMERSION_API UApartmentLifeImmersionSubsystem : public UGameInstanceSubsystem, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void ApplyUserSettings(const FApartmentLifeImmersionUserSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	const FApartmentLifeImmersionUserSettings& GetUserSettings() const { return UserSettings; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	FApartmentLifeImmersionUserSettings& GetMutableUserSettings() { return UserSettings; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Immersion")
	const FApartmentLifeImmersionState& GetImmersionState() const { return State; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void SetRoomAmbience(EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void SetMusicCategory(EApartmentLifeMusicCategory Category, bool bCrossfade = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void UpdateFromGameTime(int32 Hour, EApartmentLifeWeather Weather);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void HandleActivityStarted(FName ActivityId, EApartmentLifeRoomType Room);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void PlayMovementSfx(FName SfxId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void UpdateMicroAnimation(float Mood, float Energy, float Comfort, bool bIsSitting);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void SavePhotoBookmark(const FApartmentLifePhotoBookmark& Bookmark);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	bool ApplyPhotoBookmark(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void ApplyGraphicsSettings();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Immersion")
	void UpdateCrossfade(float DeltaTime);

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Immersion")
	FOnImmersionStateChanged OnImmersionStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Immersion")
	FOnMusicCategoryChanged OnMusicCategoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Immersion")
	FOnActivitySfxTriggered OnActivitySfxTriggered;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	void BroadcastState();
	void RefreshLighting();

	UPROPERTY()
	FApartmentLifeImmersionUserSettings UserSettings;

	UPROPERTY()
	FApartmentLifeImmersionState State;

	float MusicCrossfadeTimer = 0.f;
	EApartmentLifeMusicCategory PendingMusicCategory = EApartmentLifeMusicCategory::Afternoon;
	int32 CachedHour = 12;
	EApartmentLifeWeather CachedWeather = EApartmentLifeWeather::Clear;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ApartmentLifeDeveloperTypes.h"
#include "ApartmentLifeDeveloperLibrary.generated.h"

class AApartmentLifeApartmentUnit;
class AApartmentLifeCameraPawn;
class APlayerController;

UCLASS()
class APARTMENTLIFEDEVTOOLS_API UApartmentLifeDeveloperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static TArray<FApartmentLifeDeveloperContentEntry> QueryContentBrowser(
		EApartmentLifeDeveloperContentCategory Category,
		const FString& SearchQuery,
		bool bFavoritesOnly,
		const TArray<FName>& FavoriteIds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool PreviewContentEntry(
		UObject* WorldContextObject,
		AActor* Character,
		AApartmentLifeApartmentUnit* Apartment,
		const FApartmentLifeDeveloperContentEntry& Entry);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool ToggleContentFavorite(FName EntryId, TArray<FName>& InOutFavoriteIds);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer", meta = (WorldContext = "WorldContextObject"))
	static FApartmentLifeDeveloperValidationReport RunDataValidation(UObject* WorldContextObject, AActor* Character);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static FString FormatValidationReport(const FApartmentLifeDeveloperValidationReport& Report);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer", meta = (WorldContext = "WorldContextObject"))
	static bool ApplyLightingLabPreset(UObject* WorldContextObject, EApartmentLifeDeveloperLightingLabPreset Preset);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static FApartmentLifeDeveloperPerformanceSnapshot CapturePerformanceSnapshot(
		UObject* WorldContextObject,
		AActor* Character,
		int32 SaveSlotIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static FString FormatPerformanceSnapshot(const FApartmentLifeDeveloperPerformanceSnapshot& Snapshot);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static TArray<FApartmentLifeDeveloperDashboardCategory> BuildDefaultDashboard();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool ExecuteQuickAction(
		UObject* WorldContextObject,
		AActor* Character,
		AApartmentLifeApartmentUnit* Apartment,
		FName ActionId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer", meta = (WorldContext = "WorldContextObject"))
	static bool ExecuteBackupAction(
		UObject* WorldContextObject,
		FName ActionId,
		int32 SourceSlot,
		int32 TargetSlot);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool AdjustLiveCharacterStat(AActor* Character, FName StatId, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool SetLiveCharacterStat(AActor* Character, FName StatId, float Value);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer", meta = (WorldContext = "WorldContextObject"))
	static bool AdjustLiveApartmentSetting(UObject* WorldContextObject, FName SettingId, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool AdjustLiveCameraSetting(APlayerController* Controller, FName SettingId, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool ApplyAnimationPreviewControls(
		AActor* Character,
		const FApartmentLifeDeveloperAnimationPreviewState& State);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool EnterScreenshotStudio(APlayerController* Controller, bool bHideUi = true);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	static bool TakeStudioScreenshot(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer", meta = (WorldContext = "WorldContextObject"))
	static bool AdvanceGameDay(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	static FText GetHubSectionLabel(EApartmentLifeDeveloperHubSection Section);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	static FText GetContentCategoryLabel(EApartmentLifeDeveloperContentCategory Category);
};

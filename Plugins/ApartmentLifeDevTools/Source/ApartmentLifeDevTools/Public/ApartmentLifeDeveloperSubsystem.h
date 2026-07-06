// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ApartmentLifeDeveloperTypes.h"
#include "ApartmentLifeDeveloperSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApartmentLifeDeveloperModeChanged);

UCLASS()
class APARTMENTLIFEDEVTOOLS_API UApartmentLifeDeveloperSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void SetDeveloperModeEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	bool IsDeveloperModeEnabled() const { return bDeveloperModeEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void ToggleDeveloperMode();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void SetPerformanceOverlayVisible(bool bVisible);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	bool IsPerformanceOverlayVisible() const { return bPerformanceOverlayVisible; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void TogglePerformanceOverlay();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void TickPerformance(float DeltaSeconds, UObject* WorldContextObject, AActor* Character, int32 SaveSlotIndex);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	const FApartmentLifeDeveloperPerformanceSnapshot& GetPerformanceSnapshot() const { return PerformanceSnapshot; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	FApartmentLifeDeveloperHubState& GetMutableHubState() { return HubState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	const FApartmentLifeDeveloperHubState& GetHubState() const { return HubState; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	FApartmentLifeDeveloperAnimationPreviewState& GetMutableAnimationPreviewState() { return AnimationPreviewState; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	const FApartmentLifeDeveloperAnimationPreviewState& GetAnimationPreviewState() const { return AnimationPreviewState; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	TArray<FName>& GetMutableFavoriteContentIds() { return FavoriteContentIds; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	const TArray<FName>& GetFavoriteContentIds() const { return FavoriteContentIds; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	TArray<FApartmentLifeDeveloperDashboardCategory>& GetMutableDashboard() { return DashboardCategories; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer")
	const TArray<FApartmentLifeDeveloperDashboardCategory>& GetDashboard() const { return DashboardCategories; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void SetDashboardNote(int32 CategoryIndex, const FString& Notes);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	FApartmentLifeDeveloperValidationReport GetLastValidationReport() const { return LastValidationReport; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer")
	void StoreValidationReport(const FApartmentLifeDeveloperValidationReport& Report) { LastValidationReport = Report; }

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Developer")
	FOnApartmentLifeDeveloperModeChanged OnDeveloperModeChanged;

protected:
	UPROPERTY()
	bool bDeveloperModeEnabled = false;

	UPROPERTY()
	bool bPerformanceOverlayVisible = false;

	UPROPERTY()
	FApartmentLifeDeveloperHubState HubState;

	UPROPERTY()
	FApartmentLifeDeveloperAnimationPreviewState AnimationPreviewState;

	UPROPERTY()
	FApartmentLifeDeveloperPerformanceSnapshot PerformanceSnapshot;

	UPROPERTY()
	FApartmentLifeDeveloperValidationReport LastValidationReport;

	UPROPERTY()
	TArray<FName> FavoriteContentIds;

	UPROPERTY()
	TArray<FApartmentLifeDeveloperDashboardCategory> DashboardCategories;

	float PerformanceAccumulator = 0.f;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeProgressionTypes.h"
#include "ApartmentLifeProgressionComponent.generated.h"

class UApartmentLifeNPCSimulationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkSessionCompleted, const FApartmentLifeWorkSessionResult&, Result);

UCLASS(ClassGroup = Simulation, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEWORLDSIM_API UApartmentLifeProgressionComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const FApartmentLifeComputerQualityState& GetComputerQuality() const { return ComputerQuality; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const FApartmentLifeDailyBudget& GetDailyBudget() const { return DailyBudget; }

	FApartmentLifeDailyBudget& GetMutableDailyBudget() { return DailyBudget; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const TArray<FApartmentLifeWorkSessionResult>& GetWorkHistory() const { return WorkHistory; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const FApartmentLifeWorkSessionResult& GetLastWorkResult() const { return LastWorkResult; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const TArray<FName>& GetOwnedShopItems() const { return OwnedShopItems; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const TArray<FName>& GetUnlockedCatalogItems() const { return UnlockedCatalogItems; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	bool OwnsShopItem(FName ItemId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	const TArray<FName>& GetUnlockedWorkTypes() const { return UnlockedWorkTypes; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	bool IsWorkTypeUnlocked(FName WorkTypeId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	bool IsShopItemUnlocked(FName ItemId) const;

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Progression")
	bool IsShopTierUnlocked(EApartmentLifeFurnitureTier Tier) const;

	bool UnlockWorkType(FName WorkTypeId);
	bool UnlockShopItem(FName ItemId);
	bool UnlockShopTier(EApartmentLifeFurnitureTier Tier);

	void RecordWorkSession(const FApartmentLifeWorkSessionResult& Result);
	void AddDailyIncome(float Amount);
	void ApplyUpgrade(const FApartmentLifeBuiltinShopItem& Item);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Progression")
	bool TryPurchaseShopItem(UApartmentLifeNPCSimulationComponent* Simulation, FName ItemId);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Progression")
	void EvaluateUnlocks(UApartmentLifeNPCSimulationComponent* Simulation);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Progression")
	void SeedStarterUnlocks();

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Progression")
	FOnWorkSessionCompleted OnWorkSessionCompleted;

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	FApartmentLifeComputerQualityState ComputerQuality;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	FApartmentLifeDailyBudget DailyBudget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	FApartmentLifeWorkSessionResult LastWorkResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	TArray<FApartmentLifeWorkSessionResult> WorkHistory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	TArray<FName> OwnedShopItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	TArray<FName> UnlockedCatalogItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	TArray<FName> UnlockedWorkTypes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	TArray<uint8> UnlockedShopTiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Progression")
	FApartmentLifeApartmentUpgradeState UpgradeState;
};

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeUnlockLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "JsonObjectConverter.h"

void UApartmentLifeProgressionComponent::SeedStarterUnlocks()
{
	UnlockedWorkTypes = {
		FName(TEXT("work.computer")),
		FName(TEXT("work.data_entry")),
		FName(TEXT("work.virtual_assistant"))
	};
	UnlockedShopTiers = { static_cast<uint8>(EApartmentLifeFurnitureTier::Starter), static_cast<uint8>(EApartmentLifeFurnitureTier::Budget) };
}

bool UApartmentLifeProgressionComponent::IsWorkTypeUnlocked(FName WorkTypeId) const
{
	return UnlockedWorkTypes.Contains(WorkTypeId);
}

bool UApartmentLifeProgressionComponent::IsShopItemUnlocked(FName ItemId) const
{
	FApartmentLifeBuiltinShopItem Item;
	if (!UApartmentLifeShoppingCatalogLibrary::TryGetShopItem(ItemId, Item))
	{
		return false;
	}
	if (Item.SavingsUnlockThreshold <= 0.f)
	{
		return true;
	}
	return UnlockedCatalogItems.Contains(ItemId);
}

bool UApartmentLifeProgressionComponent::OwnsShopItem(FName ItemId) const
{
	return OwnedShopItems.Contains(ItemId);
}

bool UApartmentLifeProgressionComponent::IsShopTierUnlocked(EApartmentLifeFurnitureTier Tier) const
{
	return UnlockedShopTiers.Contains(static_cast<uint8>(Tier));
}

bool UApartmentLifeProgressionComponent::UnlockWorkType(FName WorkTypeId)
{
	return UnlockedWorkTypes.AddUnique(WorkTypeId) > 0;
}

bool UApartmentLifeProgressionComponent::UnlockShopItem(FName ItemId)
{
	return UnlockedCatalogItems.AddUnique(ItemId) > 0;
}

bool UApartmentLifeProgressionComponent::UnlockShopTier(EApartmentLifeFurnitureTier Tier)
{
	const uint8 Value = static_cast<uint8>(Tier);
	if (UnlockedShopTiers.Contains(Value))
	{
		return false;
	}
	UnlockedShopTiers.Add(Value);
	return true;
}

void UApartmentLifeProgressionComponent::RecordWorkSession(const FApartmentLifeWorkSessionResult& Result)
{
	LastWorkResult = Result;
	WorkHistory.Add(Result);
	if (WorkHistory.Num() > 20)
	{
		WorkHistory.RemoveAt(0);
	}
	AddDailyIncome(Result.TotalIncome);
	OnWorkSessionCompleted.Broadcast(Result);
}

void UApartmentLifeProgressionComponent::AddDailyIncome(float Amount)
{
	DailyBudget.DailyIncome += Amount;
}

void UApartmentLifeProgressionComponent::ApplyUpgrade(const FApartmentLifeBuiltinShopItem& Item)
{
	int32& Tier = UpgradeState.UpgradeTiers.FindOrAdd(Item.UpgradeType);
	Tier = FMath::Max(Tier, Item.UpgradeTier);

	switch (Item.UpgradeType)
	{
	case EApartmentLifeUpgradeType::Computer: ComputerQuality.ComputerTier = FMath::Max(ComputerQuality.ComputerTier, Item.UpgradeTier); break;
	case EApartmentLifeUpgradeType::Monitor: ComputerQuality.MonitorTier = FMath::Max(ComputerQuality.MonitorTier, Item.UpgradeTier); break;
	case EApartmentLifeUpgradeType::Chair: ComputerQuality.ChairTier = FMath::Max(ComputerQuality.ChairTier, Item.UpgradeTier); break;
	case EApartmentLifeUpgradeType::Desk: ComputerQuality.DeskTier = FMath::Max(ComputerQuality.DeskTier, Item.UpgradeTier); break;
	default: break;
	}
}

bool UApartmentLifeProgressionComponent::TryPurchaseShopItem(UApartmentLifeNPCSimulationComponent* Simulation, FName ItemId)
{
	if (!Simulation)
	{
		return false;
	}

	FApartmentLifeBuiltinShopItem Item;
	if (!UApartmentLifeShoppingCatalogLibrary::TryGetShopItem(ItemId, Item))
	{
		return false;
	}

	if (!IsShopTierUnlocked(Item.Tier))
	{
		return false;
	}

	if (OwnsShopItem(ItemId))
	{
		return false;
	}

	if (Item.SavingsUnlockThreshold > 0.f && !IsShopItemUnlocked(ItemId))
	{
		return false;
	}

	if (!Simulation->Finance.CanAfford(Item.Price))
	{
		Simulation->Mood.Stress = FMath::Clamp(Simulation->Mood.Stress + 3.f, 0.f, 100.f);
		return false;
	}

	Simulation->Finance.Savings -= Item.Price;
	OwnedShopItems.AddUnique(ItemId);
	ApplyUpgrade(Item);
	UApartmentLifeFinanceLibrary::RecordSpending(this, Item.Category, Item.Price);
	Simulation->Mood.Comfort = FMath::Clamp(Simulation->Mood.Comfort + Item.ComfortBonus, 0.f, 100.f);
	Simulation->Mood.OverallMood = FMath::Clamp(Simulation->Mood.OverallMood + Item.MoodBonus, 0.f, 100.f);
	EvaluateUnlocks(Simulation);
	return true;
}

void UApartmentLifeProgressionComponent::EvaluateUnlocks(UApartmentLifeNPCSimulationComponent* Simulation)
{
	UApartmentLifeUnlockLibrary::EvaluateUnlocks(Simulation, this);
}

FString UApartmentLifeProgressionComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner()) return FString::Printf(TEXT("progression_%s"), *Owner->GetName());
	return TEXT("progression_unknown");
}

void UApartmentLifeProgressionComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString ComputerJson, BudgetJson, LastWorkJson, HistoryJson, UpgradesJson;
	FJsonObjectConverter::UStructToJsonObjectString(ComputerQuality, ComputerJson);
	FJsonObjectConverter::UStructToJsonObjectString(DailyBudget, BudgetJson);
	FJsonObjectConverter::UStructToJsonObjectString(LastWorkResult, LastWorkJson);
	FJsonObjectConverter::UStructToJsonObjectString(WorkHistory, HistoryJson);
	FJsonObjectConverter::UStructToJsonObjectString(UpgradeState, UpgradesJson);
	OutData.Add(TEXT("ComputerQuality"), ComputerJson);
	OutData.Add(TEXT("DailyBudget"), BudgetJson);
	OutData.Add(TEXT("LastWorkResult"), LastWorkJson);
	OutData.Add(TEXT("WorkHistory"), HistoryJson);
	OutData.Add(TEXT("UpgradeState"), UpgradesJson);

	auto JoinIds = [](const TArray<FName>& Ids)
	{
		TArray<FString> Strings;
		for (const FName& Id : Ids) Strings.Add(Id.ToString());
		return FString::Join(Strings, TEXT(","));
	};

	OutData.Add(TEXT("OwnedShopItems"), JoinIds(OwnedShopItems));
	OutData.Add(TEXT("UnlockedCatalogItems"), JoinIds(UnlockedCatalogItems));
	OutData.Add(TEXT("UnlockedWorkTypes"), JoinIds(UnlockedWorkTypes));

	TArray<FString> TierStrings;
	for (uint8 Tier : UnlockedShopTiers) TierStrings.Add(FString::FromInt(Tier));
	OutData.Add(TEXT("UnlockedShopTiers"), FString::Join(TierStrings, TEXT(",")));
}

void UApartmentLifeProgressionComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	auto RestoreIds = [](const FString& Joined, TArray<FName>& OutIds)
	{
		OutIds.Empty();
		TArray<FString> Strings;
		Joined.ParseIntoArray(Strings, TEXT(","), true);
		for (const FString& Id : Strings)
		{
			if (!Id.IsEmpty()) OutIds.Add(FName(*Id));
		}
	};

	if (const FString* J = InData.Find(TEXT("ComputerQuality"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &ComputerQuality);
	if (const FString* J = InData.Find(TEXT("DailyBudget"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &DailyBudget);
	if (const FString* J = InData.Find(TEXT("LastWorkResult"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &LastWorkResult);
	if (const FString* J = InData.Find(TEXT("WorkHistory"))) FJsonObjectConverter::JsonArrayStringToUStruct(*J, &WorkHistory);
	if (const FString* J = InData.Find(TEXT("UpgradeState"))) FJsonObjectConverter::JsonObjectStringToUStruct(*J, &UpgradeState);
	if (const FString* V = InData.Find(TEXT("OwnedShopItems"))) RestoreIds(*V, OwnedShopItems);
	if (const FString* V = InData.Find(TEXT("UnlockedCatalogItems"))) RestoreIds(*V, UnlockedCatalogItems);
	if (const FString* V = InData.Find(TEXT("UnlockedWorkTypes"))) RestoreIds(*V, UnlockedWorkTypes);

	UnlockedShopTiers.Empty();
	if (const FString* V = InData.Find(TEXT("UnlockedShopTiers")))
	{
		TArray<FString> Strings;
		V->ParseIntoArray(Strings, TEXT(","), true);
		for (const FString& S : Strings)
		{
			if (!S.IsEmpty()) UnlockedShopTiers.Add(static_cast<uint8>(FCString::Atoi(*S)));
		}
	}
}

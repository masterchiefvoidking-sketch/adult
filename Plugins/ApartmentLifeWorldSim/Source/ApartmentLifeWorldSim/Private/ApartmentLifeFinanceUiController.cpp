// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeFinanceUiController.h"
#include "ApartmentLifeFinanceLibrary.h"
#include "ApartmentLifeShoppingCatalogLibrary.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeProgressionComponent.h"
#include "Engine/Engine.h"

void UApartmentLifeFinanceUiController::InitializeContext(
	UApartmentLifeNPCSimulationComponent* InSimulation,
	UApartmentLifeProgressionComponent* InProgression)
{
	Simulation = InSimulation;
	Progression = InProgression;
}

void UApartmentLifeFinanceUiController::ToggleBudgetOverlay()
{
	bBudgetOpen = !bBudgetOpen;
	bShoppingOpen = false;
	RefreshOverlay();
}

void UApartmentLifeFinanceUiController::ToggleShoppingOverlay(EApartmentLifeShopCategory Category)
{
	bShoppingOpen = !bShoppingOpen || ShoppingCategory != Category;
	bBudgetOpen = false;
	ShoppingCategory = Category;
	VisibleShopItems = UApartmentLifeShoppingCatalogLibrary::GetItemsForCategory(Category);

		if (Progression.IsValid())
		{
			VisibleShopItems.RemoveAll([this](const FApartmentLifeBuiltinShopItem& Item)
			{
				return !Progression->IsShopTierUnlocked(Item.Tier)
					|| (Item.SavingsUnlockThreshold > 0.f && !Progression->IsShopItemUnlocked(Item.ItemId));
			});
		}
	RefreshOverlay();
}

bool UApartmentLifeFinanceUiController::PurchaseShopItemIndex(int32 Index)
{
	if (!Progression.IsValid() || !VisibleShopItems.IsValidIndex(Index))
	{
		return false;
	}

	const bool bPurchased = Progression->TryPurchaseShopItem(Simulation.Get(), VisibleShopItems[Index].ItemId);
	RefreshOverlay();
	return bPurchased;
}

void UApartmentLifeFinanceUiController::RefreshOverlay() const
{
	if (!GEngine || (!bBudgetOpen && !bShoppingOpen))
	{
		return;
	}

	if (bBudgetOpen && Simulation.IsValid() && Progression.IsValid())
	{
		const FApartmentLifeDailyBudget& Budget = UApartmentLifeFinanceLibrary::GetCurrentBudget(Progression.Get());
		GEngine->AddOnScreenDebugMessage(9100, 0.f, FColor::Cyan, TEXT("Budget Overview"));
		GEngine->AddOnScreenDebugMessage(9101, 0.f, FColor::White,
			FString::Printf(TEXT("Savings: $%.0f | Today: +$%.0f | Week: +$%.0f"),
				Simulation->Finance.Savings, Budget.DailyIncome, Budget.WeeklyIncome));
		GEngine->AddOnScreenDebugMessage(9102, 0.f, FColor::Silver,
			FString::Printf(TEXT("Spent — Clothing: $%.0f | Furniture: $%.0f | Food: $%.0f"),
				Budget.ClothingSpent, Budget.FurnitureSpent, Budget.FoodSpent));
		GEngine->AddOnScreenDebugMessage(9103, 0.f, FColor::Silver,
			FString::Printf(TEXT("Rent/day: $%.0f | Utilities/day: $%.0f | Stress: %.0f"),
				Simulation->Finance.MonthlyRent / 30.f, Simulation->Finance.MonthlyUtilities / 30.f, Simulation->Mood.Stress));
	}

	if (bShoppingOpen)
	{
		GEngine->AddOnScreenDebugMessage(9200, 0.f, FColor::Cyan, TEXT("Apartment Shop"));
		for (int32 Index = 0; Index < FMath::Min(VisibleShopItems.Num(), 6); ++Index)
		{
			const FApartmentLifeBuiltinShopItem& Item = VisibleShopItems[Index];
			const bool bOwned = Progression.IsValid() && Progression->OwnsShopItem(Item.ItemId);
			GEngine->AddOnScreenDebugMessage(9210 + Index, 0.f, bOwned ? FColor::Green : FColor::White,
				FString::Printf(TEXT("  [%d] %s — $%.0f %s"),
					Index + 1, *Item.DisplayName.ToString(), Item.Price, bOwned ? TEXT("[owned]") : TEXT("")));
		}
	}
}

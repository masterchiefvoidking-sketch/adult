# Income, Computer Work, Shopping & Apartment Progression (MP13)

Computer-based income and progression for the private single-character apartment simulator. Local offline save/load only — no multiplayer, marketplace, or city simulation.

## Done Criteria

1. Girl can work at the computer
2. Work consumes time and energy
3. Work earns money
4. Skills improve payouts
5. Stress increases from overwork
6. Self-care reduces stress (via existing activity effects)
7. Money buys clothes, furniture, and upgrades
8. Upgrades affect stats
9. Shopping is local-only and save/load compatible
10. Progression feels rewarding without city or multiple NPCs

## Core Loop

1. Choose computer work activity
2. Spend time and energy
3. Earn income
4. Manage stress and mood
5. Buy clothing, decor, furniture, groceries, and upgrades
6. Improve apartment comfort and style
7. Improve confidence, comfort, and routine quality
8. Repeat with better options over time

## Architecture

| Component | Role |
|-----------|------|
| `FApartmentLifeWorkTypeDefinition` | Work type schema (skill, payout, duration, stress, unlock threshold) |
| `UApartmentLifeWorkLibrary` | 10 builtin work types, session result calculation, apply effects |
| `UApartmentLifeProgressionComponent` | Computer quality, budget, work history, unlocks, shop ownership, save/load |
| `UApartmentLifeFinanceLibrary` | Daily budget tracking, spending categories, day rollover |
| `UApartmentLifeShoppingCatalogLibrary` | ~16 builtin apartment shop items (furniture, electronics, decor, etc.) |
| `UApartmentLifeUnlockLibrary` | Savings-threshold unlocks for work types, shop items, furniture tiers |
| `UApartmentLifeWorkUiController` | Work selection + result overlay (on-screen debug) |
| `UApartmentLifeFinanceUiController` | Budget overview + apartment shopping overlay |

## Work Types

| Work Type | Activity ID | Skill | Base Payout |
|-----------|-------------|-------|-------------|
| Remote Work | `activity.work.computer` | Programming | $45 |
| Programming Task | `activity.work.programming` | Programming | $55 |
| Digital Art | `activity.work.digital_art` | Art | $48 |
| Online Tutoring | `activity.work.tutoring` | Communication | $42 |
| Content Editing | `activity.work.content_edit` | Organization | $35 |
| Virtual Assistant | `activity.work.virtual_assistant` | Organization | $30 |
| Data Entry | `activity.work.data_entry` | Organization | $28 |
| Freelance Design | `activity.work.design` | Art | $50 |
| Writing Task | `activity.work.writing` | Creativity | $38 |
| Freelance Task | `activity.work.freelance` | Creativity | $40 |

Work session results include base income, skill/mood/energy multipliers, bonus chance, stress change, XP gained, and new unlocks.

## Computer Quality

`FApartmentLifeComputerQualityState` tracks computer, monitor, chair, desk, and internet tiers. Better equipment improves productivity, stress reduction, and skill XP gain. Upgrades come from apartment shop purchases.

## Shopping

**Clothing** — via `UApartmentLifeWardrobeComponent::PurchaseClothing` (MP11), now tracked in daily budget.

**Apartment items** — via `UApartmentLifeProgressionComponent::TryPurchaseShopItem` with categories: Furniture, Decorations, Electronics, Kitchen, Groceries, Fitness, Bathroom.

Furniture tiers: Starter → Budget → Standard → Premium → Luxury. Higher tiers unlock at savings thresholds ($2k, $5k, $10k).

## UI Controls

| Key | Action |
|-----|--------|
| W | Toggle work selection menu |
| Enter | Start selected work |
| Numpad 1–4 | Select work task |
| K | Toggle budget overview |
| J | Toggle apartment shop (furniture) |
| G | Wardrobe (MP11) |

## Save / Load

`UApartmentLifeProgressionComponent` persists:

- Computer quality state
- Daily budget and work history
- Owned shop items and unlocked catalog items
- Unlocked work types and shop tiers
- Apartment upgrade tiers

## Tests

Automation tests in `ApartmentLifeProgressionTests.cpp`:

- `ApartmentLife.Progression.WorkTypeCatalog`
- `ApartmentLife.Progression.WorkSessionIncome`
- `ApartmentLife.Progression.ShopPurchase`
- `ApartmentLife.Progression.SavingsUnlocks`
- `ApartmentLife.Progression.DailyBudget`

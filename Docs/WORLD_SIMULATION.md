# World Simulation Engine

The **ApartmentLifeWorldSim** plugin implements the living-world simulation layer. The world runs whether the player is present or not.

## Core Principle

Every NPC has an independent life. The player enters an already-running world.

## Systems Overview

| System | Class | Responsibility |
|--------|-------|----------------|
| Time | `UApartmentLifeGameTimeSubsystem` | Clock, pause, 1x/2x/4x/8x, calendar, seasons, holidays, birthdays |
| NPC Life | `UApartmentLifeNPCSimulationComponent` | Master orchestrator per character |
| Schedule | `UApartmentLifeWorldSimLibrary` | Daily schedule generation from templates + modifiers |
| Career | `FApartmentLifeCareerState` + `UApartmentLifeCareerData` | Jobs, XP, promotions, satisfaction |
| Finance | `FApartmentLifeFinancialLedger` | Income, rent, utilities, debt, credit score, investments |
| Mood | `FApartmentLifeMoodState` | Influenced by sleep, work, finances, social, weather |
| Memory | `FApartmentLifeMemoryRecord` | Decay + reinforcement |
| Relationships | `FApartmentLifeRelationshipRecord` | Friendship, trust, respect, invitations |
| Shopping | `UApartmentLifeWorldSimLibrary::EvaluateShoppingNeeds` | Budget-aware purchasing AI |
| Apartment | `FApartmentLifeApartmentProgressState` | Tier upgrades, furniture wishlist |
| Events | `UApartmentLifeWorldEventSubsystem` | Random daily events from data assets |
| Save | `UApartmentLifeSaveSubsystem` | Aggregates all `IApartmentLifeSaveable` state |

## Time Controls

```cpp
TimeSubsystem->SetTimeAcceleration(EApartmentLifeTimeAcceleration::Speed4x);
TimeSubsystem->SetPaused(true);
```

| Preset | Effect |
|--------|--------|
| Paused | Simulation stops |
| 1x | 1 game minute per real minute |
| 2x | 2 game minutes per real minute |
| 4x | 4 game minutes per real minute |
| 8x | 8 game minutes per real minute |

## Schedule Generation

Schedules are generated daily from `UApartmentLifeScheduleTemplateData` and modified by:

- Personality traits (introversion → less social, ambition → more work)
- Mood (stress → more relax, fatigue → earlier sleep)
- Weather (rain → less exercise)
- Finances (low disposable income → less shopping/entertainment)
- Weekend/holiday flags (skip work blocks)

Without a template asset, `CreateBuiltinWeekdaySchedule()` provides a full day loop.

## Career Data Assets

Create `DA_Career_*` inheriting `UApartmentLifeCareerData`:

```
career.teacher
career.nurse
career.office_worker
career.programmer
career.artist
career.barista
career.retail
career.student
career.freelancer
career.restaurant_worker
```

## Financial Model

Daily tick:
- Income = `Career.ComputeMonthlyIncome() / 30`
- Expenses = fixed bills + entertainment / 30
- Net applied to savings; debt and credit score adjust on negative balance
- 10% of positive net flows to investment account when savings exceed 2× emergency fund

## Shopping AI

`EvaluateShoppingNeeds()` scores items by:
- Category urgency (groceries when hungry, decor when creative)
- Personality appeal map on `UApartmentLifeShoppingItemData`
- Seasonal tags
- Budget factor from disposable income
- Apartment floor space for furniture

## Random Events

Register `UApartmentLifeRandomEventData` assets. `UApartmentLifeWorldEventSubsystem` rolls once per day with season/weekend filters.

## Save / Load

```cpp
SaveSubsystem->SaveToSlot(0);
SaveSubsystem->LoadFromSlot(0);
```

Persists: world time, weather, all `IApartmentLifeSaveable` actors/components (NPC sim, apartments, wardrobe, appearance).

## Unit Tests

Run in UE Session Frontend → Automation:

- `ApartmentLife.WorldSim.Calendar.WeekdayCalculation`
- `ApartmentLife.WorldSim.Finance.LedgerCalculations`
- `ApartmentLife.WorldSim.Mood.Recalculation`
- `ApartmentLife.WorldSim.Memory.DecayAndReinforce`
- `ApartmentLife.WorldSim.Schedule.BuiltinSchedule`

## Adding a New Job

1. Create `DA_Career_YourJob` (UApartmentLifeCareerData)
2. Set wages, work hours, activity ID, promotion thresholds
3. Assign to NPC via `SetCareerData()`
4. No C++ changes required

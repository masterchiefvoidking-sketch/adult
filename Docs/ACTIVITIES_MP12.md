# Apartment Interactions, Routines & Activity System (MP12)

Universal activity framework for the private single-character apartment simulator. All activities are non-explicit daily-life routines.

## Done Criteria

1. Every major room has useful activities
2. Activities consume time (minute-based timer)
3. Activities change stats via catalog-defined effects
4. Activities trigger correct camera framing (MP10 + expanded rules)
5. Activities trigger outfit recommendations when needed (MP11)
6. Yoga mini-game prototype with pose match, balance, breathing, streak
7. Shower/grooming routines are privacy-aware and non-explicit
8. Computer work earns income
9. Sleep restores energy
10. Save/load preserves activity, cooldown, and routine chain state

## Architecture

| Component | Role |
|-----------|------|
| `FApartmentLifeActivityDefinition` | Universal activity schema (ID, room, outfit, animation, duration, camera, stats, money, cooldown) |
| `UApartmentLifeActivityCatalogLibrary` | ~50 builtin activities + morning/evening routine chains |
| `UApartmentLifeActivityLibrary` | Resolver: duration, requirements, completion effects, camera/animation lookup |
| `UApartmentLifeActivityComponent` | Timer execution, cooldowns, completion results, save/load |
| `UApartmentLifeRoutineChainComponent` | Ordered multi-step routines (morning/evening) |

## Rooms & Activities

| Room | Example activities |
|------|------------------|
| Bedroom | Sleep, nap, read, phone browse, plan tomorrow |
| Bathroom | Shower, brush teeth, wash face, hair/skin care, makeup, mirror check, laundry hamper |
| Closet | Open wardrobe, change outfit, save preset, organize closet, laundry |
| Office | Remote work, freelance, digital art, programming, tutoring, budget review, online shopping, skill practice |
| Kitchen | Cook breakfast/lunch/dinner, snack, drink, eat, clean dishes, grocery check |
| Living room | Watch TV, listen to music, play game, talk, stretch, decorate, relax |
| Yoga area | Yoga session, stretch, exercise, breathing exercise |

## Routine Chains

**Morning** (`routine.morning`): Shower → Brush teeth → Wardrobe → Breakfast → Computer work

**Evening** (`routine.evening`): Dinner → Relax → Change outfit → Skin care → Read → Sleep

Start via `AApartmentLifeSimCharacter::StartMorningRoutine()` / `StartEveningRoutine()`.

## Stat Effects

Each activity defines deltas for mood, energy, hygiene, hunger, comfort, confidence, stress, affection, and skill XP. Computer work uses the income formula from `UApartmentLifeGirlLifeLibrary::ComputeComputerWorkPayout`.

Completion is applied automatically by `UApartmentLifeActivityComponent` and broadcast via `OnActivityCompletedWithResult`.

## Camera Rules

`UApartmentLifeActivityCameraLibrary` provides per-activity framing:

- Yoga/fitness: full-body, orbit allowed
- Computer: desk over-shoulder
- Shower: privacy-aware framing
- Wardrobe: character preview mode
- Bedroom sleep/relax: soft wide framing

## Yoga Mini-Game

`UApartmentLifeYogaMinigameComponent` tracks pose match score, balance, breathing rhythm, pose streak, flexibility XP, fitness XP, and session duration.

## Save / Load

`UApartmentLifeActivityComponent` persists active activity, remaining minutes, and cooldowns.

`UApartmentLifeRoutineChainComponent` persists chain ID and step index.

NPC simulation, wardrobe, and game time persist via existing save subsystems.

## Tests

Automation tests in `ApartmentLifeActivityTests.cpp`:

- `ApartmentLife.Activities.CatalogCoverage`
- `ApartmentLife.Activities.CompletionEffects`
- `ApartmentLife.Activities.ComponentTimer`
- `ApartmentLife.Activities.RoutineChain`
- `ApartmentLife.Activities.SaveRoundTrip`

## Integration

`AApartmentLifeSimCharacter` orchestrates activity start/complete side effects:

- Wardrobe recommendations and mood effects on start
- Yoga, grooming, bedroom routines on start
- Laundry and worn-state on complete
- Routine chain auto-advance on complete

# MP19 — Content Expansion Pack 01

Expands the private single-character apartment vertical slice with more wardrobe, furniture, routines, dialogue, yoga poses, work types, and activities — without changing core architecture.

## Content Summary

| Category | Base | Pack 01 Added | Total |
|----------|------|---------------|-------|
| Clothing items | 37 | 43 | 80 |
| Shop / furniture upgrades | 16 | 30 | 46 |
| Activities | 55 | 11 | 66 |
| Routine chains | 2 | 17 | 19 |
| Yoga poses | 1 fallback | 8 builtin | 8 |
| Hairstyles | 6 | 5 | 11 |
| Color palettes | — | 5 | 5 |
| Work types | 10 | 2 | 12 |
| Contextual dialogue lines | 0 | 56+ pools | contextual |

## File Layout

Content Pack 01 uses per-module expansion files merged into existing catalogs:

| Module | Expansion File |
|--------|----------------|
| Wardrobe | `ApartmentLifeWardrobeContentPack01.cpp` |
| Activities | `ApartmentLifeActivityContentPack01.cpp` |
| WorldSim (shop) | `ApartmentLifeShoppingContentPack01.cpp` |
| Social | `ApartmentLifeDialogueCatalogLibrary.cpp` |
| CharacterPipeline | `ApartmentLifeYogaPoseCatalogLibrary.cpp` |
| Character (creator) | extended `ApartmentLifeCharacterCreatorCatalogLibrary.cpp` |

## Wardrobe

43 new items across all requested categories:

- 10 casual, 5 work-from-home, 5 lounge, 5 sleepwear, 5 athletic, 3 formal, 10 accessories
- 5 new outfit presets in bootstrap (teal day, cozy work, soft evening, silk sleep)
- Vertical-slice starter inventory includes all pack items

Items support preview, equip, favorites, save/load, activity suitability (via `SuitableContexts`), and mood/confidence (via `ComfortRating` + `ComputeOutfitMoodEffect`).

## Furniture / Shop

30 new shop items across bedroom, bathroom, living room, kitchen, computer area, and yoga area. Light unlock thresholds ($300–$1800 savings) keep progression accessible.

Upgrade types: bed, desk, chair, monitor, computer, yoga mat, lighting, decor, kitchen tools.

## Routines

17 new routine chains:

- **Morning:** quick, relaxed, productive
- **Work:** focus session, creative session, short freelance, long project
- **Self-care:** quick grooming, full grooming, skincare, hair care, mirror check
- **Evening:** cozy, reading night, TV night, early sleep, late work night

## Dialogue

`UApartmentLifeDialogueCatalogLibrary::GenerateContextualPlayerLine` selects lines based on:

- Time of day (morning greetings, late night)
- Mood, energy, stress
- Weather (rainy day)
- Savings (low money)
- Apartment comfort (upgrades)
- Personality interests (fitness, cooking)

Player talk (`StartConversationWithPlayer`) now auto-generates an opening line and broadcasts via `OnConversationLineSpoken`.

## Yoga

8 builtin poses with montage IDs (`montage.yoga.<poseId>`), difficulty, flexibility reward, and stress reduction. `UApartmentLifeYogaMinigameComponent` falls back to builtin catalog when no data asset is registered.

## Activities / Animations

11 new activities including outfit preview, hair adjustment, wake-up, journal, focus/creative work, yoga pose variants, and tidying. Montage IDs follow convention (`montage.<activity_suffix>`).

## Progression

2 new work types (`work.focus`, `work.creative`) with modest savings unlocks ($280–$350). Shop expansion adds tiered computer, bed, chair, yoga mat, and wardrobe upgrades.

## Tests

- `ApartmentLife.ContentPack01.WardrobeExpansion`
- `ApartmentLife.ContentPack01.ActivitiesAndRoutines`
- `ApartmentLife.ContentPack01.ShopExpansion`
- `ApartmentLife.ContentPack01.DialogueAndYoga`

## Balance

Unlock thresholds stay low-to-mid range. Most pack content is available early; premium tiers unlock at $900–$3000 savings. No new grind loops introduced.

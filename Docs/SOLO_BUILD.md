# Solo Build Pipeline

Private single-developer workflow for **Adult Anime Apartment Life**. This document describes how to extend the game without building enterprise systems.

## Scope

Build the smallest powerful version first:

1. One apartment
2. One customizable main character
3. One customizable partner NPC
4. One wardrobe system
5. One furniture/build mode
6. One daily routine system
7. One relationship/dialogue system
8. One yoga mini-game
9. One shower/grooming routine (non-explicit)
10. One bedroom routine: sleep, relax, read, change outfit

Do **not** build yet: multiplayer, accounts, marketplace, cloud backend, mod browser, live-service economy.

## Folder Structure

Create these Content Browser folders under `/Game/`:

```
Characters/
  Body/
  Hair/
  Clothing/
  Animations/
Apartments/
  Rooms/
  Furniture/
  Materials/
Gameplay/
  Camera/
  Interaction/
  AI/
  Schedules/
  Relationships/
  Activities/
UI/
  CharacterCreator/
  Wardrobe/
  BuildMode/
  Dialogue/
  Inventory/
Data/
  Characters/
  Clothing/
  Furniture/
  Jobs/
  Activities/
  Dialogues/
SaveSystem/
DeveloperTools/
Tests/
```

## Content Workflow

All gameplay content is added through **Primary Data Assets**, not hardcoded C++.

Each content item should define:

| Field | Purpose |
|-------|---------|
| ID | Stable save-compatible identifier |
| Display name | UI label |
| Category | Filtering and rules |
| Description | Tooltips and debug |
| Tags | Search, AI, weather/occasion |
| Price | Economy when applicable |
| Requirements | Unlock gates |
| Preview image/icon | UI and build mode |
| Data references | Meshes, animations, activities |

## Plugin Map (Vertical Slice)

| Plugin | Role in solo build |
|--------|----------------------|
| **ApartmentLifeCore** | Time, save, data registry |
| **ApartmentLifeCamera** | Orbit, build, character creator cameras |
| **ApartmentLifeCharacter** | Creator and body |
| **ApartmentLifeWardrobe** | Clothing layers and presets |
| **ApartmentLifeApartment** | Apartment, furniture, build mode |
| **ApartmentLifeInteraction** | Universal interact → activity wiring |
| **ApartmentLifeActivities** | Activity execution |
| **ApartmentLifeWorldSim** | NPC schedules, mood, finance |
| **ApartmentLifeCharacterPipeline** | Animation, yoga, grooming |
| **ApartmentLifeSocial** | Dialogue and relationships |
| **ApartmentLifeDevTools** | Private debug helpers (non-shipping) |

## Developer Tools

`UApartmentLifeDevToolsLibrary` (non-shipping) provides:

- Add money
- Change time of day
- Spawn furniture
- Reset apartment layout
- Teleport character
- Preview animation group
- Preview dialogue
- Save/load test slots

Use these from Blueprint or the console during iteration. They are stripped in shipping builds.

## Save / Load Rule

**No feature is complete unless it survives save/load.**

Persist through `IApartmentLifeSaveable` and `UApartmentLifeSaveSubsystem`:

- Character appearance and wardrobe
- Apartment furniture placement
- NPC schedule and simulation state
- Relationship values
- Money and inventory
- Time of day and activity progress
- Social and city subsystem state (via save registry)

## Testing Checklist

Run automation tests under `ApartmentLife.Interaction.*` and the builder/social/world sim suites.

Manual vertical slice loop:

1. Create / spawn character
2. Move camera (orbit, pan, zoom)
3. Change outfit (G)
4. Place furniture (B build mode)
5. Interact with bed, shower, yoga mat (E)
6. Talk to partner (Q)
7. Quick save (F5)
8. Quick load (F9)
9. Continue after loading

## Next Expansion (When Ready)

After the slice is stable, add content in this order:

1. More clothing data assets
2. More furniture and one layout data asset
3. Dialogue topic data assets
4. Schedule template for partner NPC
5. UI widgets bound to existing components

Keep each addition data-driven and save-compatible.

# Solo Build Pipeline

Private single-developer workflow for **Adult Anime Apartment Life** — **one girl, one apartment** (MP08).

## Scope (Current)

Build around:

1. One apartment
2. One customizable girl character
3. One player-controlled camera
4. Wardrobe, furniture, daily routine, dialogue
5. Yoga, grooming, shower (non-explicit), bedroom routines
6. Computer-based income
7. Local save/load

## Deferred (Do Not Build Yet)

- City simulation and open world
- Multiple NPCs and social networks
- Large career / traffic / public events systems
- Multiplayer, accounts, marketplace, cloud, mod browser

See [SINGLE_CHARACTER.md](SINGLE_CHARACTER.md) for the active build spec.

## Folder Structure

Create these Content Browser folders under `/Game/`:

```
Characters/Body, Hair, Clothing, Animations
Apartments/Rooms, Furniture, Materials
Gameplay/Camera, Interaction, Schedules, Activities
UI/CharacterCreator, Wardrobe, BuildMode, Dialogue
Data/Characters, Clothing, Furniture, Activities, Dialogues
SaveSystem/
DeveloperTools/
Tests/
```

## Active Plugins

| Plugin | Role |
|--------|------|
| ApartmentLifeCore | Time, save, data registry |
| ApartmentLifeCamera | Orbit, build, character focus cameras |
| ApartmentLifeCharacter | Creator and body |
| ApartmentLifeWardrobe | Clothing and presets |
| ApartmentLifeApartment | Furniture and build mode |
| ApartmentLifeInteraction | Interact → activity |
| ApartmentLifeActivities | Activity execution + fallbacks |
| ApartmentLifeWorldSim | Girl life sim, mood, finance, affection |
| ApartmentLifeCharacterPipeline | Animation, yoga, grooming |
| ApartmentLifeSocial | Player dialogue |
| ApartmentLifeDevTools | Debug helpers (non-shipping) |

**Disabled by default:** `ApartmentLifeCity`

## Developer Tools

`UApartmentLifeDevToolsLibrary` — add money, change time, spawn furniture, save/load tests. Non-shipping only.

## Save / Load Rule

No feature is complete unless it survives save/load. Girl state includes affection, hygiene, energy, savings, wardrobe, and apartment layout.

## Test Checklist

1. Spawn girl + apartment
2. Camera orbit / pan / zoom / face / outfit focus
3. Wardrobe (G) and closet interact
4. Build mode furniture placement
5. Bed, shower, desk, yoga mat interactions
6. Computer work → savings increase
7. Talk (Q) → affection changes
8. F5 save → F9 load → continue

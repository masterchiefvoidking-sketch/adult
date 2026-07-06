# Getting Started

## 1. Environment Setup

### Windows

1. Install **Unreal Engine 5.6** via Epic Games Launcher
2. Install **Visual Studio 2022** with:
   - Desktop development with C++
   - Game development with C++
   - Windows 10/11 SDK
3. Clone this repository to a path **without spaces** (recommended)

### Generate Project Files

Right-click `AdultAnimeApartmentLife.uproject` → **Generate Visual Studio project files**.

Or from UE root:

```bat
UnrealVersionSelector.exe /projectfiles "C:\Path\To\AdultAnimeApartmentLife.uproject"
```

## 2. First Launch

1. Open `AdultAnimeApartmentLife.uproject`
2. When prompted, allow **rebuild modules** — this compiles all plugins
3. Create placeholder maps if missing:
   - `/Game/Maps/DevSandbox`
   - `/Game/Maps/MainMenu`

> Maps referenced in `Config/DefaultEngine.ini` are not shipped in this repo. Create them in the editor or retarget defaults.

## 3. Recommended Editor Settings

### Primary Asset Manager

Edit **Project Settings → Asset Manager** and add Primary Asset Types:

| Type | Base Class |
|------|------------|
| Activity | `UApartmentLifeActivityData` |
| ClothingItem | `UApartmentLifeClothingItemData` |
| OutfitPreset | `UApartmentLifeOutfitPresetData` |
| FurnitureItem | `UApartmentLifeFurnitureItemData` |
| RoomDefinition | `UApartmentLifeRoomDefinitionData` |
| CharacterCustomization | `UApartmentLifeCharacterCustomizationData` |

### Content Settings

**Project Settings → Apartment Life → Content Settings**

- Set maturity rating for your build target
- Disable explicit content for safe development builds

## 4. Dev Sandbox Workflow

### Place a Sim Character

1. Create Blueprint `BP_SimCharacter` parented to `AApartmentLifeSimCharacter`
2. Assign skeletal mesh and AnimBP
3. Add `UApartmentLifeCharacterCustomizationData` to appearance component
4. Populate `WeeklySchedule` on life simulation component

### Place an Apartment

1. Create Blueprint `BP_ApartmentUnit` parented to `AApartmentLifeApartmentUnit`
2. Define room volumes using `UApartmentLifeRoomDefinitionData`
3. Test furniture placement via `PlaceFurniture()` in editor utility or PIE

### Test Camera

1. Set Game Mode to `AAdultAnimeApartmentLifeGameModeBase` (or Blueprint child)
2. PIE — orbit with right mouse, zoom with scroll, photo mode with **P**

## 5. Time Scale Debugging

In PIE console:

```
ke * UApartmentLifeGameTimeSubsystem::SetTimeScale 120
```

(Use Blueprint or debug widget in practice — subsystem is accessible from world.)

Default time scale: **60×** (1 real second ≈ 1 game minute).

## 6. Building from Command Line

```bat
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" ^
  AdultAnimeApartmentLifeEditor Win64 Development ^
  -Project="C:\Path\To\AdultAnimeApartmentLife.uproject"
```

## 7. Next Steps

- Author data assets per [CONTENT_PIPELINE.md](CONTENT_PIPELINE.md)
- Configure animation per [ANIMATION.md](ANIMATION.md)
- Review system design in [ARCHITECTURE.md](ARCHITECTURE.md)

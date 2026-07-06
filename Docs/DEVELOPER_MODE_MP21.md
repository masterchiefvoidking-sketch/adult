# MP21 — Developer Mode, Content Editor & Iteration Tools

Internal developer toolkit for rapid iteration on the private single-character apartment vertical slice. Non-shipping only.

## Access

| Key | Action |
|-----|--------|
| **F1** | Toggle Developer Hub (Slate panel) |
| **`** (Grave) | Toggle performance overlay |
| **Esc** | Close hub / navigate back within hub |
| **Legacy debug hotkeys** | Active while Developer Mode is enabled (after opening hub once) |

## Developer Hub Sections

| Section | Capabilities |
|---------|-------------|
| **Character** | Live mood, energy, hygiene, stress, savings, confidence edits |
| **Wardrobe** | Opens content browser filtered to clothing |
| **Furniture** | Content browser filtered to furniture/decor |
| **Apartment** | Quick actions + lighting/time hooks |
| **Activities** | Content browser filtered to activities |
| **Dialogue** | Content browser filtered to conversation topics |
| **Economy** | Quick actions (max money, unlocks) |
| **Camera** | FOV, orbit/zoom/pan speed, photo mode, screenshots |
| **Lighting** | 9 lighting lab presets (morning, noon, sunset, night, rain, cloudy, warm/cool indoor, studio) |
| **Animation** | Preview idle/yoga/conversation, facial expressions, IK debug |
| **Save System** | Backup tools (duplicate, backup, restore, delete slots) |
| **Performance** | Snapshot + overlay toggle |
| **Content Browser** | Searchable catalog across clothing, hairstyles, furniture, activities, routines, animations, yoga poses, work types, music |
| **Data Validation** | Duplicate ID scan, outfit reference check, save slot health |
| **Screenshot Studio** | Photo mode + high-res capture from camera section |
| **Quick Actions** | Max money, unlock all, reset apartment/appearance, advance time, empty inventory |
| **Backup** | Duplicate/backup/restore/delete save slots |
| **Project Dashboard** | Completion % per major system with editable notes |

## Architecture

```
UApartmentLifeDeveloperSubsystem     — dev mode flag, perf overlay, favorites, dashboard, validation cache
UApartmentLifeDeveloperLibrary       — content browser, validation, lighting lab, quick/backup actions
UApartmentLifeDeveloperUiController  — hub navigation + content list state
UApartmentLifeUiBridgeComponent      — PushDeveloperHubPanel(), activation routing
```

Extends existing `UApartmentLifeDevToolsLibrary` (40+ APIs) rather than replacing it.

## Content Browser

- Queries merged builtin catalogs (wardrobe, shop, activities, routines, yoga, work, hairstyles, animation groups, music)
- Filter by category, search text, favorites
- Single-click select, double-click preview (equip clothing, spawn furniture, trigger activity, etc.)

## Data Validation

Scans for:
- Duplicate clothing/shop/activity IDs
- Missing display names
- Invalid equipped outfit references
- Unreadable save slot metadata

Reports formatted via `FormatValidationReport()`.

## Performance Overlay

Shows in HUD when toggled:
- FPS, frame time, memory usage
- Active animation/interaction counts
- Save slot size estimate

## Automation Tests

| Test | Path |
|------|------|
| Content browser queries | `ApartmentLife.Developer.ContentBrowser` |
| Data validation | `ApartmentLife.Developer.DataValidation` |
| Project dashboard | `ApartmentLife.Developer.Dashboard` |

## Done Criteria

A solo developer can:
1. Press F1 to open the hub
2. Browse and one-click preview any catalog item
3. Live-edit character stats and camera settings
4. Switch lighting presets instantly
5. Run validation and read a report
6. Toggle perf overlay during play
7. Backup/restore saves without file digging
8. Use legacy hotkeys while dev mode is active

## Out of Scope

- UObject property grid inspector (future editor module)
- External JSON content export pipeline
- Shipping builds (all APIs guarded with `#if UE_BUILD_SHIPPING`)

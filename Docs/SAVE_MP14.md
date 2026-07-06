# Save / Load, Debug Tools, Testing & Stability (MP14)

Stability foundation for the private single-character apartment simulator. Local offline play only.

## Done Criteria

1. Full play session can be saved and reloaded
2. Core systems persist correctly
3. Debug tools can test every major system
4. Activity state survives loading
5. Camera settings persist
6. Purchases persist
7. Furniture placement persists
8. Outfit state persists
9. Corrupt/missing saves fail safely
10. No new gameplay content added in this pass

## Save System

`UApartmentLifeSaveSubsystem` aggregates all `IApartmentLifeSaveable` components plus world time.

### Save Version 3 Metadata

| Field | Description |
|-------|-------------|
| `SaveVersion` | Schema version (current: 3) |
| `Metadata.GameVersion` | Project name |
| `Metadata.CreatedTimestamp` | First save time |
| `Metadata.LastPlayedTimestamp` | Updated on load/save |
| `Metadata.CharacterId` | Main character ID |
| `Metadata.ApartmentId` | Apartment unit ID |
| `Metadata.SlotDisplayName` | Manual/autosave label |

### Migration

Legacy saves (v1/v2) migrate automatically on load. Saves with unknown future versions are rejected safely.

### Slot Operations

| API | Purpose |
|-----|---------|
| `SaveToSlot` | Manual or autosave write |
| `LoadFromSlot` | Restore with migration + post-load refresh |
| `DeleteSaveSlot` | Remove a slot |
| `GetSlotMetadata` | Read slot info without applying |
| `RequestAutosave` | Debounced autosave (30s cooldown) |

### Autosave Triggers

Autosave fires after:

- Activity completion
- Work session completion
- Shop purchase (apartment or clothing)
- Furniture placement

### Persisted Systems

| System | Saveable |
|--------|----------|
| Character appearance | `UApartmentLifeCharacterCreatorComponent` |
| Body customization | `UApartmentLifeBodyCustomizationComponent` |
| Wardrobe / outfit | `UApartmentLifeWardrobeComponent` |
| NPC stats / finance / career | `UApartmentLifeNPCSimulationComponent` |
| Activities / cooldowns | `UApartmentLifeActivityComponent` |
| Routine chains | `UApartmentLifeRoutineChainComponent` |
| Progression / upgrades | `UApartmentLifeProgressionComponent` |
| Apartment layout | `AApartmentLifeApartmentUnit` |
| Yoga session | `UApartmentLifeYogaMinigameComponent` |
| Camera preferences | `UApartmentLifeCameraSettingsSubsystem` |
| World time / weather / season | Top-level save game fields |

### Post-Load Refresh

`AApartmentLifeSingleCharacterPlayerController::ApplyPostLoadState()` runs after quick-load and auto-load:

- Applies camera settings to pawn
- Reapplies owned apartment upgrade effects

## Debug Menu (F1)

Private developer overlay. Key actions:

| Key | Action |
|-----|--------|
| M | +$500 |
| Shift+M | -$200 |
| H / Shift+H | Advance hour / set 8am |
| [/] | Mood up/down |
| ;' | Energy up/down |
| -= | Hygiene up/down |
| ,. | Stress up/down |
| <> | Hunger up/down |
| \\ / | Comfort up/down |
| O | Cycle outfit |
| U | Unlock all clothing |
| I | Unlock all shop items |
| X | Force-complete activity |
| Y | Start yoga |
| 3-7 | Teleport to room |
| S / L | Save / load |
| Shift+S / Shift+L | Delete slot / clear all saves |
| R / Shift+R | Reset apartment / reset character |
| F4 | Spawn desk at cursor |

## Tests

| Test | Location |
|------|----------|
| `ApartmentLife.Save.VersionAndMigration` | Core |
| `ApartmentLife.Save.MetadataFields` | Core |
| `ApartmentLife.Progression.NpcFinanceSaveRoundTrip` | WorldSim |
| `ApartmentLife.Progression.UpgradeReapplyOnLoad` | WorldSim |
| `ApartmentLife.CharacterPipeline.YogaSaveRoundTrip` | CharacterPipeline |
| Existing wardrobe/activity round-trip tests | MP11/MP12 |

## Full Loop Checklist

1. Start new game
2. Customize girl / change outfit
3. Place furniture
4. Computer work → earn money
5. Buy clothing / furniture
6. Yoga / shower / sleep
7. Save (F5) or autosave
8. Quit and reload (F9 or continue)
9. Verify stats, outfit, layout, savings, activity state

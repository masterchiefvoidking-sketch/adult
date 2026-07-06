# MP16 — Character Creator, Appearance Presets & Body Customization

Polished character creator for the private single-character apartment simulator. One customizable girl, deep appearance editing, local presets, wardrobe compatibility, animation preview, and reliable save/load.

## Architecture

| Layer | Responsibility |
|-------|----------------|
| `UApartmentLifeCharacterCreatorComponent` | Creator state, morphs, colors, presets, save/load (`CreatorState` JSON) |
| `UApartmentLifeCharacterCreatorCatalogLibrary` | Builtin defaults, hairstyles, voices, personalities, starter presets |
| `UApartmentLifeCharacterCreatorPresetLibrary` | Load/save presets under `Saved/CharacterPresets/*.json` |
| `UApartmentLifeCharacterCreatorUiController` | Creator UI state machine (tabs, entries, session backup, confirm/cancel) |
| `UApartmentLifeCharacterCreatorLibrary` | Wardrobe compatibility report, animation preview groups |
| `UApartmentLifeUiBridgeComponent` | Character Creator Slate panel, list routing |
| `AApartmentLifeCameraPawn` | Creator camera mode, lighting mode storage, character rotation |

## Flow

- **New game** — `InitializeFromBuiltinCatalog()` seeds the girl from builtin defaults.
- **Edit existing** — Profile → **Edit Character**, or press **C** in gameplay.
- **Confirm** — Applies changes and closes; session backup is discarded.
- **Cancel** — Esc closes creator and restores the session backup (via `CloseCreator(false)` on back).
- **Presets** — Builtin presets in catalog; user presets saved to disk with compatibility version.

## Customization Tabs

| Tab | Contents |
|-----|----------|
| Face | All face-region morph sliders |
| Body | Height, shoulders, waist, hips, limbs, muscle tone |
| Hair | Builtin hairstyle list |
| Colors | Skin, hair, eye (double-click randomizes selected) |
| Makeup | Intensity, lip/nail color display |
| Accessories | Glasses, jewelry, accessory counts |
| Voice & Style | Voice + personality presets |
| Presets | Load builtin/user presets; save current look |
| Preview | Idle, walk, sit, yoga, stretch, mirror, wardrobe, relax poses |

## Body → Gameplay Integration

Body changes sync to `UApartmentLifeBodyCustomizationComponent`, which updates:

- Clothing fit (`FApartmentLifeBodyFitProfile`)
- Scale / collision capsule
- Walk and idle style IDs

`AApartmentLifeSimCharacter` listens to `OnCreatorStateUpdated` and refreshes clothing fit.

## Camera & Lighting

Creator camera (`EnterCharacterCreatorMode`):

- Orbit, pan, zoom (existing camera controls)
- Rotate character instead of camera
- Focus actions: Face, Upper Body, Full Body, Outfit

Lighting modes are stored on the camera pawn and in user settings (visual light actors are a future content hook):

- Apartment natural, bathroom mirror, bedroom soft, wardrobe studio, evening warm

## Randomization

Scoped randomize from the creator panel:

- Full character, face only, body only (panel actions)
- Hair/colors via tab actions and catalog believable color pools
- Face morphs clamped to 0.25–0.75; body height 155–178 cm

## Wardrobe Compatibility

`EvaluateWardrobeCompatibility` flags equipped items that no longer fit the body profile. Warnings appear in the creator panel footer.

## Save / Load

`UApartmentLifeCharacterCreatorComponent` implements `IApartmentLifeSaveable`:

- Key: `CreatorState` (full `FApartmentLifeCharacterCreatorState` JSON)
- Body component also saves separately; creator is authoritative for appearance on restore

## Controls

| Key | Action |
|-----|--------|
| C | Toggle character creator |
| F8 → Edit Character | Open creator from profile |
| `[` / `]` | Adjust selected slider (when creator open; otherwise debug mood) |
| Esc | Back / cancel creator (restores backup) |
| Mouse | Orbit / pan / zoom creator camera |

## Debug (F1 menu)

| Key | Action |
|-----|--------|
| Shift+P | Print creator JSON to screen |
| Shift+F | Reset creator face |
| Shift+B | Reset creator body |

## Tests

- `ApartmentLife.CharacterPipeline.CreatorPresetApply`
- `ApartmentLife.CharacterPipeline.CreatorRandomizeBounds`
- `ApartmentLife.CharacterPipeline.CreatorSaveRoundTrip`
- `ApartmentLife.CharacterPipeline.CreatorPreviewGroups`

## Done Criteria

1. Create one original girl — builtin catalog + creator UI
2. Appearance updates immediately — `ApplyCreatorState` + delegates
3. Body customization works with clothing — body sync + fit refresh
4. Camera rotate, pan, zoom, focus — creator camera mode
5. Presets save/load — preset library + UI
6. Randomization creates usable characters — scoped randomize + clamps
7. Animation preview — preview tab + `PreviewAnimationGroup`
8. Wardrobe compatibility — compatibility report + footer warnings
9. Character data survives save/load — `CreatorState` JSON round-trip
10. Edit later without corrupting save — session backup + confirm/cancel

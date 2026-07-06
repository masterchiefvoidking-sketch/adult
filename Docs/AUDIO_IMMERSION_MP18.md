# MP18 — Audio, Ambience, Music, Visual Polish & Immersion

Immersion layer for the private single-character apartment simulator. C++ drives ambience profiles, adaptive music state, lighting presets, environmental motion flags, micro-animation cues, photo bookmarks, and accessibility/graphics settings. Sound and VFX assets hook in via stable `FName` IDs.

## Architecture

| Layer | Responsibility |
|-------|----------------|
| `UApartmentLifeImmersionSubsystem` | Game-instance state, music crossfade, save/load (`immersion_user_settings`) |
| `UApartmentLifeImmersionLibrary` | Room profiles, weather layers, lighting values, SFX/music ID resolution |
| `UApartmentLifeUiBridgeComponent` | Settings sync, hour-advanced updates, HUD weather, photo HUD suppression |
| `AApartmentLifeSimCharacter` | Activity → room ambience + activity SFX |
| `AApartmentLifeCameraPawn` | Photo mode DOF, high-res screenshot |
| `AApartmentLifeSingleCharacterPlayerController` | Post-load immersion restore, photo hotkeys |

Delegates `OnImmersionStateChanged`, `OnMusicCategoryChanged`, and `OnActivitySfxTriggered` broadcast state for Blueprint/audio components.

## Room Ambience

Each `EApartmentLifeRoomType` maps to a `FApartmentLifeRoomAmbienceProfile`:

| Room | Tone ID | Layers (examples) |
|------|---------|-------------------|
| Bedroom | `audio.ambience.bedroom.soft` | window quiet, night crickets |
| Bathroom | `audio.ambience.bathroom.echo` | exhaust fan, sink water |
| Living Room | `audio.ambience.living.roomtone` | TV, AC, clock, traffic |
| Kitchen | `audio.ambience.kitchen.roomtone` | fridge, water, coffee maker |
| Office | `audio.ambience.office.roomtone` | computer fan, keyboard, mouse |

Yoga activities override layers with calm/nature/breathing IDs and switch music to `Yoga`.

## Adaptive Music

Categories: Morning, Afternoon, Evening, Night, Relaxation, Work, Yoga, Character Creator, Wardrobe, Build Mode, Main Menu.

- Hour-of-day selects default category via `GetMusicCategoryForHour`
- Activities override via `GetMusicCategoryForActivity`
- UI screens override via `ApplyMusicForScreen` (main menu, creator, wardrobe, build, yoga)
- Crossfade tracked by `MusicCrossfadeAlpha` (2s blend)

Music asset IDs: `audio.music.<CategoryName>`

## Weather Ambience

`GetWeatherAmbienceLayers` returns layers per weather + night extras:

- **Clear** — birds, light wind
- **Rain/Storm** — rain on windows, distant traffic, thunder (storm)
- **Snow** — soft wind
- **Night (any)** — crickets, distant city

## Lighting Presets

Presets: Morning, Midday, Golden Hour, Evening, Night, Rainy Day, Cloudy Day.

`FApartmentLifeLightingPresetValues` exposes sun/interior/window intensity, color temperature, shadow softness, and reflection strength for content/Blueprint post-process hookup.

## Environmental Motion

`FApartmentLifeEnvironmentalMotionState` flags: curtains, plants, ceiling fan, food steam, sunlight dust, TV glow, monitor glow, rain on windows, clock ticking. Built from room, activity, weather, and hour.

## Character Micro-Animations

`PickMicroAnimationCue` selects subtle idle behaviors from mood/energy/comfort: blink, breathing, weight shift, look around, stretch, hair/clothing adjust, relaxed smile, yawn.

UiBridge forwards cues to `UApartmentLifeAnimationComponent` via `montage.micro.<Cue>` and mapped facial expressions every ~3s (skipped when reduce-motion is enabled).

## Photo Mode

`FApartmentLifePhotoModeSettings` (camera plugin): FOV, aperture, blur, time-of-day hour, hide UI, pose/expression IDs, lighting preset index.

| Input | Action |
|-------|--------|
| Toggle Photo Mode | existing camera binding |
| Photo FOV up/down | adjust field of view |
| Photo Aperture up/down | adjust DOF |
| Photo Screenshot | `HighResShot 1920x1080` |
| Photo Save Bookmark | saves camera pose to immersion user settings |

Photo mode hide-UI suppresses HUD without changing the user's HUD preference.

## Accessibility & Graphics

`FApartmentLifeImmersionUserSettings` persists:

- **Audio** — master, music, effects, UI volume
- **Accessibility** — subtitles, subtitle scale, reduce motion, camera sensitivity
- **Graphics** — quality preset (Low–Ultra), high-quality effects, VSync

Settings panel indices 0–13 cover camera, volumes, subtitles, motion, graphics, autosave, and HUD.

## Save / Load

Save ID: `immersion_user_settings`

Captured: `UserSettings` (audio/accessibility/graphics/photo bookmarks) + `ImmersionState` (active room, music, lighting, ambience layers, weather layers, environmental motion).

Restored on load via `ApplyPostLoadState` on the player controller and save subsystem registry.

## Content Hookup

Subscribe to subsystem delegates or read `GetImmersionState()` each tick:

```cpp
// Example: play room tone when state changes
Immersion->OnImmersionStateChanged.AddLambda([](const FApartmentLifeImmersionState& State)
{
    // Load USoundBase from State.ActiveRoomToneId, crossfade State.ActiveAmbienceLayers, etc.
});
```

## Tests

- `ApartmentLife.Immersion.RoomAmbience`
- `ApartmentLife.Immersion.MusicByHour`
- `ApartmentLife.Immersion.LightingPreset`
- `ApartmentLife.Immersion.SettingsSaveRoundTrip`

## Debug

Immersion state is visible through subsystem getters. Use debug time advance (hour) to verify lighting/music transitions. Activity start triggers room ambience and activity SFX ID broadcast.

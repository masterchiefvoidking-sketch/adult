# Camera & Interaction Polish (MP10)

Premium camera and click-to-select interaction layer for the single-character apartment build.

## Primary Camera Modes

| Mode | Enter | Exit |
|------|-------|------|
| Apartment | Default | — |
| Character Focus | F2 face, F3 outfit, 8/9/0 upper body/hair/shoes, C creator | Home reset |
| Wardrobe | G or closet | Activity complete |
| Build Mode | B | B |
| Activity | E / 1–4 on selected object | Activity complete |
| Photo Mode | P | P |

## Controls

| Input | Action |
|-------|--------|
| LMB drag | Orbit |
| RMB drag | Rotate |
| MMB drag | Pan |
| Mouse wheel | Zoom |
| LMB click | Select interactable |
| LMB double-click | Focus target |
| Home | Reset view |
| E | Execute first action on selection |
| 1–4 | Execute action by index |
| P | Photo mode |
| Page Up/Down | Photo FOV (in photo mode) |
| F5 | Quick save (includes camera settings) |

## Interaction Selection

1. Left-click furniture to select
2. On-screen action list shows available routines with stat summaries
3. Press 1–4 or E to start an action
4. Camera auto-frames yoga, computer, mirror, shower, bedroom, and wardrobe activities

## Activity Framing

`UApartmentLifeActivityCameraLibrary` maps activity IDs to arm length, pitch, focus offset, and privacy framing (shower uses offset framing without explicit angles).

## Save Settings

`UApartmentLifeCameraSettingsSubsystem` persists:

- Orbit / zoom / pan sensitivity
- Invert Y
- Last primary mode
- Photo mode FOV, aperture, focus distance
- Preferred wardrobe zoom
- Build top-down preference

Saved via `UApartmentLifeSaveSubsystem` on quick save (F5).

## Safety

- Spring arm collision probe prevents wall clipping
- Apartment bounds clamp pan pivot
- FOV clamped 20–120°
- Smooth arm length interpolation on mode transitions

## Related

- [FIRST_VERTICAL_SLICE.md](FIRST_VERTICAL_SLICE.md) — MP09 playable loop
- [ARCHITECTURE.md](ARCHITECTURE.md) — plugin layout

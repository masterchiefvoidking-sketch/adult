# First Vertical Slice (MP09)

Private playable loop for one apartment and one customizable girl. Scope is intentionally frozen until save/load round-trips correctly.

## Done Criteria

The slice is complete when the player can:

1. Load into the apartment
2. Rotate, pan, zoom, and focus the camera
3. Customize the girl (C = character creator camera)
4. Change her outfit (G = wardrobe, closet interactable)
5. Interact with bed, shower, mirror, computer, yoga mat, sofa, TV, and kitchen
6. Earn money through computer work
7. Improve stats through routines (shower, grooming, yoga, sleep)
8. Save the game (F5)
9. Close and reload (F9)
10. Continue with correct state

## Controls

| Key | Action |
|-----|--------|
| Mouse drag | Orbit camera |
| Mouse wheel | Zoom |
| Middle mouse | Pan |
| Right mouse | Rotate |
| E | Interact |
| G | Open wardrobe |
| C | Character creator camera |
| B | Build mode |
| F5 / F9 | Quick save / load |
| F1 | Debug menu (dev builds) |

## Starter Content

**Rooms:** bedroom, bathroom, living room, kitchen, office.

**Furniture:** bed, closet, desk, sofa, TV, kitchen, fridge, shower, mirror, yoga mat.

**Wardrobe:** 12 favorite outfits (3 casual, 2 work, 2 lounge, 2 sleep, 2 athletic, 1 formal) seeded via `UApartmentLifeWardrobeBootstrapLibrary`.

## Systems

| System | Entry point |
|--------|-------------|
| Game mode | `AApartmentLifeSingleCharacterGameMode` |
| Player | `AApartmentLifeSingleCharacterPlayerController` |
| Character | `AApartmentLifeSimCharacter` |
| Interactions | `UApartmentLifeInteractionComponent` + `FApartmentLifeInteractionDescriptor` |
| Income | `UApartmentLifeGirlLifeLibrary::ApplyActivityCompletion` |
| Routines | Grooming, bedroom, yoga components on sim character |
| Save | `UApartmentLifeSaveSubsystem` (activity, wardrobe closet, schedule) |
| Debug | `UApartmentLifeDebugMenuComponent` (F1) |

## Debug Menu (F1)

When open: M money, H hour, [/] mood, ;' energy, -= hygiene, O outfit, 3–7 teleport rooms, Y yoga, S save, L load, R reset apartment.

## Build Order Reference

See MP09 master prompt for the full 18-step build order. This document tracks what shipped in the first vertical slice branch.

## Related Docs

- [SINGLE_CHARACTER.md](SINGLE_CHARACTER.md) — MP08 single-character spec
- [VERTICAL_SLICE.md](VERTICAL_SLICE.md) — MP07 → MP08 migration

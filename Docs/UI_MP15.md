# MP15 — UI / UX, Menus, HUD & Private Build Flow

Slate-based UI foundation for the private single-character apartment simulator. Replaces on-screen debug overlays for normal gameplay with a viewport-rooted UI layer.

## Architecture

- **`ApartmentLifeUI` plugin** — Slate root widget, game-instance UI subsystem, bridge component
- **`UApartmentLifeUiSubsystem`** — screen stack, HUD state, panel state, toasts, settings
- **`UApartmentLifeUiBridgeComponent`** — polls sim/controllers and routes list actions back to gameplay
- **`FApartmentLifeUiOverlayGate`** (Core) — disables legacy `AddOnScreenDebugMessage` overlays when Slate UI is active

## Screens

| Screen | Open | Close |
|--------|------|-------|
| Main Menu | Boot (no autosave) | New Game / Continue |
| HUD | Always (toggle Tab) | Tab |
| Context Menu | Select interactable | Esc / clear selection |
| Wardrobe | G | Esc / G |
| Work | W | Esc / W |
| Work Result | After work session | Esc / click Close |
| Finance / Shopping | K / J | Esc |
| Build Mode | B | Esc / B |
| Routines | F10 | Esc |
| Profile | F8 | Esc |
| Save / Load | F7 / Main Menu | Esc |
| Settings | Main Menu | Esc |
| Yoga | Yoga session active | Esc |

## Controls

| Key | Action |
|-----|--------|
| Esc | Back / close active screen |
| Tab | Toggle HUD |
| F7 | Save / Load |
| F8 | Profile |
| F10 | Routines |
| G | Wardrobe |
| W | Work |
| K | Budget |
| J | Shop |
| B | Build mode |
| F1 | Debug menu (unchanged, separate overlay) |

## Save Slots

Three manual slots (0–2). Save/Load UI shows version, timestamp, and character id from `GetSlotMetadata`.

## Tests

- `ApartmentLife.UI.ScreenEnumCoverage`
- `ApartmentLife.UI.OverlayGate`
- `ApartmentLife.UI.SubsystemDefaults`

## Done Criteria Mapping

1. Start/continue — Main menu + save/load UI
2. Core stats — HUD bar
3. Object selection — Context menu from interaction selection
4. Interaction menus — Context menu list
5. Wardrobe — Wardrobe panel bound to `WardrobeUiController`
6. Buy/equip clothing — Double-click in wardrobe/shopping panels
7. Place furniture — Build mode panel + existing build component
8. Work at computer — Work panel + result screen
9. Routines — Routines panel + routine chain component
10. Yoga mini-game — Yoga panel on session update
11. Save/load — F7 panel with slot metadata
12. Settings — Settings panel synced with camera/save subsystems
13. Debug menu — F1 dev overlay (unchanged, separate from gameplay UI)

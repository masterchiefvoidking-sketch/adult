# MP20 — Release Candidate Polish

Final quality pass for the private single-character apartment vertical slice before future expansion work.

## Goals

- Fix reproducible P0 bugs in wardrobe, build mode, save/load, and conversation flows
- Improve camera responsiveness, accessibility, and mode transitions
- Polish wardrobe preview, favorites display, and yoga session feedback
- Harden quick save/load with success and failure feedback

## Bug Fixes

| Issue | Fix |
|-------|-----|
| Wardrobe could not be closed with G / Esc | `CloseWardrobeSession()` tears down UI, camera, preview, and dress activity; G toggles wardrobe |
| Opening wardrobe auto-changed outfit | `ShouldAutoRecommendOutfitForActivity()` skips wardrobe/preview activities |
| Conversation had no exit | T / Esc ends active player conversation and restores apartment camera |
| Build mode Esc desynced camera/state | `CloseBuildModeSession()` syncs PC flag, component, and `ReturnToApartmentCamera()` |
| Quick load ignored failures | Load only calls `ApplyPostLoadState()` on success; toasts report result |

## Camera Polish

- Pan Y now clamps to apartment bounds (matches pan X)
- Orbit pivot uses tick delta instead of world delta
- Photo/free mode snapshot stores yaw, pitch, zoom, pivot, focus, and activity framing
- Reduce Motion snaps pivot instantly and restores camera faster
- Wardrobe character rotation is ~35% faster for outfit comparison

## UI / Wardrobe Polish

- Single-click wardrobe list entry previews outfit on character
- Double-click still equips or purchases
- Favorite items show a ★ marker in the wardrobe panel
- Save/load screen reports save, load, empty-slot, and failure toasts
- `RefreshAfterLoad()` refreshes HUD, active screen, and open wardrobe panel

## Activity Polish

- Yoga minigame receives pose input each UI tick while a mat session is active
- Breathing rhythm uses a dedicated phase timer (cycles every ~3s) instead of session minutes

## Session API

`AApartmentLifePlayerController` exposes virtual session helpers used by Slate UI:

- `CloseWardrobeSession()`
- `CloseBuildModeSession()`
- `CloseConversationSession()`

`AApartmentLifeSingleCharacterPlayerController` implements full teardown for the single-girl build.

## Automation Tests

| Test | Path |
|------|------|
| Wardrobe auto-recommend guard | `ApartmentLife.ReleaseCandidate.WardrobeAutoRecommend` |
| Yoga breathing phase | `ApartmentLife.ReleaseCandidate.YogaBreathingPhase` |

## Release Checklist

The RC build supports the full private loop:

1. Launch → create/load character
2. Explore apartment and interact with objects
3. Open wardrobe (G), preview on click, equip on double-click, close with G or Esc
4. Rearrange furniture in build mode (B), exit cleanly with B or Esc
5. Complete routines, work, yoga, grooming, and sleep activities
6. Talk with the girl (T), end conversation (T or Esc)
7. Quick save / quick load with feedback
8. Save/load slots with error toasts
9. Reload preserves apartment layout, wardrobe, progression, and character appearance

## Out of Scope (Future Updates)

- Additional apartments, clothing packs, furniture packs
- New hobbies, seasonal events, multiplayer/cloud features

Do not start expansion content until this RC is stable in playtesting.

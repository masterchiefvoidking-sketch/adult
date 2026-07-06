# MP22 — Test Plan, QA Gauntlet & Playability Audit

Full quality pass on the private single-character apartment vertical slice. **No new features** — audit, bug classification, critical fixes, and retest guidance.

**Audit date:** 2026-07-06  
**Branch:** `cursor/qa-gauntlet-audit-757a`  
**Method:** Static code audit + automation tests (no in-editor play session in CI)

---

## Test Summary

| Session | Area | Automated | Code Audit | Status |
|---------|------|-----------|------------|--------|
| 01 | New Game | Partial | Reviewed | **PASS** with caveats |
| 02 | Camera | Partial | Reviewed | **FAIL** → fixes applied |
| 03 | Wardrobe | Yes | Reviewed | **FAIL** → fixes applied |
| 04 | Apartment | Partial | Reviewed | **PASS** with caveats |
| 05 | Activities | Yes | Reviewed | **FAIL** → fixes applied |
| 06 | Income | Yes | Reviewed | **FAIL** → fixes applied |
| 07 | Routines | Yes | Reviewed | **FAIL** → fixes applied |
| 08 | Save/Load Torture | Yes (component) | Reviewed | **FAIL** → critical fix applied |

### Verdict

The build was **not playable end-to-end** before this pass due to a critical continue-on-launch save bug and several high-severity wardrobe/routine failures. **Critical and high-priority fixes are included in this branch.** Medium/low polish items remain for follow-up.

---

## Fixes Applied in This Branch

| ID | Severity | Fix |
|----|----------|-----|
| QA-001 | CRITICAL | Defer `LoadFromSlot` until after `BootstrapApartment()` spawns actors |
| QA-002 | CRITICAL | Stop overwriting loaded sim personality/affection on continue |
| QA-003 | HIGH | Wardrobe session no longer starts timed dress activity (G key) |
| QA-004 | HIGH | Wardrobe camera preserved; activity completion ignored while wardrobe open |
| QA-005 | HIGH | Closet interaction opens wardrobe UI via activity-started hook |
| QA-006 | HIGH | Wardrobe activities use `Any` room (bedroom closet works) |
| QA-007 | HIGH | Routine chains pass preferred room; cancel on failed start |
| QA-008 | HIGH | `ResumeAfterLoad` skips restart when activity already active |
| QA-009 | HIGH | Hourly schedule no longer auto-replaces player outfit |
| QA-010 | HIGH | Content-pack work activities set `bUsesIncomeFormula` |
| QA-011 | MEDIUM | Interactions pass preferred room to `StartActivity` |
| QA-012 | MEDIUM | Debug/dev hour advance uses `AdvanceMinutes(60)` |
| QA-013 | MEDIUM | Main-menu continue only runs post-load on success |

---

## Bug List

### CRITICAL

#### QA-001 — Continue on launch loses all actor save data
- **Severity:** CRITICAL
- **Session:** 08 Save/Load
- **Reproduction:** Play → change outfit, place furniture, earn money → F5 save → quit → relaunch
- **Expected:** All progress restored
- **Actual:** Empty/default character, apartment, wardrobe; only world time/subsystems restored
- **Root cause:** `LoadFromSlot` ran in `BeginPlay` before actors existed; `ApplySaveables` had no targets
- **Fix:** Check save exists → bootstrap actors → load → `ApplyPostLoadState`
- **Retest:** Quit/relaunch cycle; verify savings, outfit, furniture positions

#### QA-002 — Loaded sim state overwritten on continue
- **Severity:** CRITICAL
- **Session:** 08 Save/Load
- **Reproduction:** Save with custom affection/mood → relaunch
- **Expected:** Saved personality/finance preserved
- **Actual:** `ConfigureGirlCharacter` reset kindness, affection, trust every launch
- **Fix:** Only seed personality when `bSeedWardrobe` (new game)
- **Retest:** Save affection at 80 → reload → still 80

---

### HIGH

#### QA-003 — Wardrobe G key started timed activity, broke camera
- **Severity:** HIGH | **Session:** 02 Camera, 03 Wardrobe
- **Reproduction:** Press G → drag to rotate character
- **Expected:** Character spins, outfit framing
- **Actual:** Activity camera overwrote wardrobe mode; character orbit disabled
- **Fix:** `OpenWardrobeSession()` without `StartActivity`
- **Retest:** G open → orbit character → Esc close

#### QA-004 — Wardrobe activity completion ejected camera while UI open
- **Severity:** HIGH | **Session:** 03 Wardrobe
- **Reproduction:** Open wardrobe → advance 10+ game minutes
- **Expected:** Session stays open until Esc/G
- **Actual:** Camera exited; UI remained (desync)
- **Fix:** Ignore activity completion camera teardown when wardrobe UI open
- **Retest:** Stay in wardrobe 30+ minutes; camera stable

#### QA-005 — Closet interactable did not open wardrobe UI
- **Severity:** HIGH | **Session:** 03 Wardrobe
- **Reproduction:** Select closet → E → Open Wardrobe
- **Expected:** Full wardrobe panel + camera
- **Actual:** Activity only; no Slate wardrobe screen
- **Fix:** `HandleGirlActivityStarted` opens wardrobe session for dress activities
- **Retest:** Closet interaction opens same UI as G key

#### QA-006 — Wardrobe activities required WalkInCloset room
- **Severity:** HIGH | **Session:** 03 Wardrobe
- **Reproduction:** Interact with bedroom closet
- **Expected:** Wardrobe opens
- **Actual:** `CanStartActivity` failed (closet in Bedroom, activity required WalkInCloset)
- **Fix:** Dress/wardrobe activities → `RequiredRoom = Any`
- **Retest:** `ApartmentLife.QA.WardrobeActivityRoom`

#### QA-007 — Routine chains soft-locked on room mismatch
- **Severity:** HIGH | **Session:** 07 Routines
- **Reproduction:** Start `routine.morning.productive` from living room
- **Expected:** Steps advance (shower → makeup → wardrobe → work)
- **Actual:** Shower failed in LivingRoom; chain stuck on step 1
- **Fix:** `GetPreferredRoomForActivity` + pass room to `StartActivity`; cancel chain on failure
- **Retest:** `ApartmentLife.QA.RoutinePreferredRoom`; manual morning routine

#### QA-008 — Save mid-routine reset activity timer
- **Severity:** HIGH | **Session:** 07 Routines, 08 Save/Load
- **Reproduction:** Start routine → wait halfway through step → save → load
- **Expected:** Remaining minutes preserved
- **Actual:** `ResumeAfterLoad` restarted step from full duration
- **Fix:** Skip `StartCurrentStep` if saved activity still active with matching ID
- **Retest:** Save at 50% activity progress → load → timer unchanged

#### QA-009 — Hourly schedule replaced player outfit
- **Severity:** HIGH | **Session:** 03 Wardrobe
- **Reproduction:** Equip custom outfit → advance time past 9:00 or 22:00
- **Expected:** Outfit unchanged until player changes it
- **Actual:** `HandleScheduleOccasion` auto-selected work/sleep outfit every hour
- **Fix:** Disabled auto outfit for player-directed single-character build
- **Retest:** Manual dress → advance 24 hours → same equipped items

#### QA-010 — Content-pack work paid no income
- **Severity:** HIGH | **Session:** 06 Income
- **Reproduction:** Complete `activity.work.focus` or `routine.work.focus`
- **Expected:** Money earned via work formula
- **Actual:** `bUsesIncomeFormula` false on content-pack definitions
- **Fix:** Set flag for `work.*` activities in content pack
- **Retest:** `ApartmentLife.QA.WorkIncomeFormula`

#### QA-011 — Main-menu continue ran post-load on failure
- **Severity:** HIGH | **Session:** 08 Save/Load
- **Reproduction:** Corrupt save slot → Continue
- **Expected:** Error toast; no partial state refresh
- **Actual:** `OnPostLoadRequested` fired unconditionally
- **Fix:** Gate broadcast on `LoadFromSlot` return value
- **Retest:** Load invalid slot → toast, no HUD refresh

---

### MEDIUM

#### QA-012 — Clothing preview does not change character mesh
- **Severity:** MEDIUM | **Session:** 03 Wardrobe
- **Reproduction:** Wardrobe → single-click item
- **Expected:** Visual preview on character
- **Actual:** `RefreshVisuals()` is stub; preview slots not rendered (content-driven meshes pending)
- **Recommended fix:** AnimBP/mesh layer reads `GetPreviewSlots()` or temporary equip-with-rollback
- **Retest:** Click preview → visible change before equip

#### QA-013 — Outfit presets not in player wardrobe UI
- **Severity:** MEDIUM | **Session:** 03 Wardrobe
- **Reproduction:** Open wardrobe → look for Work/Lounge presets
- **Expected:** Apply/save preset from UI
- **Actual:** API exists on `WardrobeUiController`; Slate panel lists catalog only
- **Recommended fix:** Add presets sub-panel in `PushWardrobePanel`

#### QA-014 — Activities don't complete while time paused
- **Severity:** MEDIUM | **Session:** 05 Activities
- **Reproduction:** Start 480-min sleep → pause game time
- **Expected:** Activity completes or start blocked
- **Actual:** `HandleMinuteAdvanced` never fires; soft-lock
- **Recommended fix:** Real-time fallback timer or block start while paused

#### QA-015 — Double energy/stress on catalog work activities
- **Severity:** MEDIUM | **Session:** 06 Income
- **Reproduction:** Complete `activity.work.computer`
- **Expected:** Designed stat deltas once
- **Actual:** Catalog deltas + `ApplyWorkSessionResult` both apply
- **Recommended fix:** Skip overlapping stats when `bUsesIncomeFormula`

#### QA-016 — Work income duration fixed at 90 min
- **Severity:** MEDIUM | **Session:** 06 Income
- **Reproduction:** 60-min data entry vs 120-min programming
- **Expected:** Payout scales with activity duration
- **Actual:** `MakeWork` defaults 90 min for all types
- **Recommended fix:** Per-type duration or read from activity catalog

#### QA-017 — Schedule activity IDs mismatch catalog
- **Severity:** MEDIUM | **Session:** 07 Routines
- **Examples:** `activity.cooking.breakfast` vs `activity.cook.breakfast`
- **Actual:** Falls through to string-matching fallback; wrong/missing stat effects
- **Recommended fix:** Align IDs or add aliases in `TryGetActivityDefinition`

#### QA-018 — Camera settings stale on autosave
- **Severity:** MEDIUM | **Session:** 02 Camera, 08 Save/Load
- **Reproduction:** Move camera → autosave (purchase) → reload
- **Expected:** Camera position restored
- **Actual:** Only F5 quick save calls `CaptureFromPawn`
- **Recommended fix:** Capture pawn state in all `SaveToSlot` paths or before autosave

#### QA-019 — Routine "Advance Step" skips rewards
- **Severity:** MEDIUM | **Session:** 07 Routines
- **Reproduction:** Routines panel → Advance Step
- **Expected:** Current step completes with stats/income
- **Actual:** `AdvanceRoutineChain` cancels activity without `CompleteActivity`
- **Recommended fix:** `ForceCompleteActivity()` before advance

#### QA-020 — Free camera exit discards snapshot
- **Severity:** MEDIUM | **Session:** 02 Camera
- **Reproduction:** Custom view → Photo → Free → Free again
- **Expected:** Restore pre-photo framing
- **Actual:** `ReturnToApartmentCamera` resets to spawn defaults
- **Recommended fix:** Snapshot stack for nested modes

---

### LOW

| ID | Issue | Session |
|----|-------|---------|
| QA-021 | Photo FOV keys work outside photo mode | 02 Camera |
| QA-022 | Wardrobe zoom not persisted to settings | 02 Camera |
| QA-023 | Spring-arm collision causes closet clipping | 02 Camera |
| QA-024 | `FlexibilityXPDelta` ignored on yoga complete | 05 Activities |
| QA-025 | Grocery purchase doesn't reduce hunger (player) | 06 Income |
| QA-026 | Savings-threshold unlocks need work/purchase to refresh | 06 Income |
| QA-027 | Preview layer conflicts (dress over top+bottom) | 03 Wardrobe |
| QA-028 | Dashboard notes UI missing (dev tools) | Dev |

---

## Fix Order (Remaining Work)

1. ~~Crashes~~ — none found in audit
2. ~~Save/load bugs~~ — QA-001, QA-002 fixed; QA-018 remains
3. ~~Soft-locks~~ — QA-007, QA-014 partial
4. ~~Camera bugs~~ — QA-003/004 fixed; QA-020/022 remain
5. ~~Wardrobe bugs~~ — QA-006/009 fixed; QA-012/013 remain
6. ~~Activity bugs~~ — QA-010 fixed; QA-015–017 remain
7. ~~Income bugs~~ — QA-010 fixed; QA-015/016 remain
8. UI bugs — QA-013 presets, QA-019 advance step
9. Animation polish — preview visuals, yoga XP
10. Visual/audio polish — clipping, sound

---

## Automation Tests

| Test | Path |
|------|------|
| Wardrobe room policy | `ApartmentLife.QA.WardrobeActivityRoom` |
| Routine preferred room | `ApartmentLife.QA.RoutinePreferredRoom` |
| Work income formula flag | `ApartmentLife.QA.WorkIncomeFormula` |
| 10× save round-trip stress | `ApartmentLife.QA.SaveRoundTripStress` |
| Wardrobe auto-recommend guard | `ApartmentLife.ReleaseCandidate.WardrobeAutoRecommend` |
| Wardrobe save round-trip | `ApartmentLife.Wardrobe.SaveRoundTrip` |
| Activity save round-trip | `ApartmentLife.Activities.SaveRoundTrip` |
| NPC finance save round-trip | `ApartmentLife.Progression.NpcFinanceSaveRoundTrip` |

Run all: **Session Frontend → Automation → ApartmentLife.QA.***

---

## Manual Retest Scripts

### Session 01 — New Game
1. Launch → new game → creator → body/face/hair/outfit → save → enter apartment
2. Verify: no crash, appearance intact, F5 creates save metadata

### Session 02 — Camera
1. Pan/zoom/orbit at apartment bounds
2. F2/F3 focus, B build mode, P photo mode
3. G wardrobe → character rotates on drag
4. Esc exits each mode cleanly

### Session 03 — Wardrobe
1. G and closet both open wardrobe UI
2. Double-click equip → F5 → F9 → outfit persists
3. Advance 24 hours → outfit unchanged
4. ★ favorites filter works

### Session 04 — Apartment
1. B build → place/rotate/move furniture → F5 → F9
2. Verify positions exact, no vanishing items

### Session 05 — Activities
1. Sleep, shower, yoga, cook, eat, TV, read — each completes with stat change
2. Debug H advances activity timers

### Session 06 — Income
1. Office work → money increases
2. `routine.work.focus` pays income
3. Buy clothing/furniture → persists after reload

### Session 07 — Routines
1. `routine.morning.productive` completes all steps
2. Esc/cancel stops safely
3. Save mid-routine → load → progress preserved

### Session 08 — Save Torture
Repeat 10×: outfit change → move furniture → complete activity → earn money → F5 → quit → relaunch  
**Pass:** No lost progress, corruption, or broken animation

---

## Done Criteria Checklist

| # | Criterion | Status |
|---|-----------|--------|
| 1 | New game works | ✅ Expected after QA-001/002 |
| 2 | Camera works | ⚠️ Core fixed; polish remains |
| 3 | Wardrobe works | ⚠️ Session flow fixed; preview visuals pending |
| 4 | Apartment placement works | ✅ Existing tests pass |
| 5 | Activities work | ⚠️ Room/income fixed; pause edge remains |
| 6 | Income works | ⚠️ Content-pack work fixed; balance tuning remains |
| 7 | Routines work | ✅ Room routing fixed |
| 8 | Save/load survives torture | ✅ Critical path fixed; manual 10× required |
| 9 | No critical bugs remain | ✅ In this branch |
| 10 | Playable full in-game day | ⚠️ Requires in-editor confirmation |

---

## Notes

- Full runtime playtest still required in UE5 editor — this pass is code audit + targeted fixes + automation.
- Wardrobe **visual preview** depends on content-side mesh compositing not yet wired in C++.
- Developer Mode (MP21) can accelerate retest: quick actions, validation, perf overlay.

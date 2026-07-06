# MP17 — Animation, IK, Routine Transitions & Activity Polish

Animation foundation for the private single-character apartment simulator. Prioritizes smooth transitions, interaction alignment, facial expression, movement personality, and safe save/load over new content volume.

## Architecture

| Layer | Responsibility |
|-------|----------------|
| `UApartmentLifeAnimationComponent` | Animation state, groups, montages, IK targets, facial/movement styles, save/load |
| `UApartmentLifeAnimationLibrary` | Activity→animation resolution, alignment, transitions, grooming/yoga helpers |
| `FApartmentLifeInteractionAlignmentSet` | Entry/exit, hands/feet/seat/camera markers for furniture alignment |
| `FApartmentLifeAnimationIkTargets` | Foot/hand/head IK target locations for AnimBP |
| `AApartmentLifeSimCharacter` | Wires activities, grooming steps, routine chains → animation |
| `AApartmentLifeFurnitureActor` | Interaction points with socket names and local offsets |

AnimBP (`ABP_SimCharacter`) consumes `OnAnimationGroupChanged` and `OnAnimationStateChanged` delegates. C++ drives state; authored montages/blend spaces plug in via `MontageId` and style IDs.

## Animation Groups

Extended groups include: Idle, Walk, Turn In Place, Stand, sit variants (chair/couch/floor), lie/sleep/wake, stretch/yoga, work (computer/cooking/eating/cleaning), wardrobe/grooming/shower, relax/TV/read, conversation.

## Activity → Animation Flow

```
Player interact / simulation activity
  → UApartmentLifeActivityComponent::StartActivity
  → AApartmentLifeSimCharacter::HandleActivityStarted
  → UApartmentLifeAnimationLibrary::ApplyActivityAnimation
       ├── Resolve furniture alignment (if interactable selected)
       ├── Align character to entry transform
       ├── Pick transition kind (blend vs montage bridge)
       ├── Set montage ID (montage.<activity_suffix>)
       ├── Set facial expression from activity + mood/energy
       └── Apply walk/idle movement personality from body component
```

Grooming steps and routine chain steps also trigger animation updates via `OnGroomingStepChanged` and `OnRoutineChainStepChanged`.

## Interaction Alignment

`FApartmentLifeInteractionPoint` now includes:

- `LocalOffset`, `LocalRotation`
- `HandsTargetOffset`, `FeetTargetOffset`, `SeatTargetOffset`
- `CameraFocusOffset`, `ExitOffset`
- `SocketName` (defaults via `GetDefaultSocketForInteraction`)

Supported furniture categories: bed, sofa, chair, desk, mirror, closet, shower, yoga mat, stove/fridge, TV.

## Transitions

`GetTransitionBetweenGroups` returns:

- **MontageBridge** — walk→sit, sit→stand, lie→wake, walk→lie
- **Blend** — most other group changes
- **Instant** — same group

Transition montage IDs: `montage.transition.<From>_to_<To>`

## Facial Expressions

States: Neutral, Happy, Tired, Focused, Relaxed, Stressed, Embarrassed, Confident, Sleepy.

Derived from activity type plus mood/energy stats. Dev tool: `ForceFacialExpression`.

## Movement Personality

Presets: Soft, Confident, Energetic, Relaxed, Shy, Elegant, Sporty.

Mapped from `EApartmentLifeWalkStyle` on the body component. Exposed as `WalkStyleId` / `IdleStyleId` on animation state for AnimBP locomotion layers.

## Yoga

- `ApplyYogaPoseAnimation` sets yoga group + `montage.yoga.<poseId>`
- Full-body IK enabled during yoga
- Session state still driven by `UApartmentLifeYogaMinigameComponent`

## Routine Chains

Morning and evening chains advance on activity completion. `ResumeAfterLoad()` restarts the current step activity after save/load.

## Save / Load

`UApartmentLifeAnimationComponent` persists full `FApartmentLifeAnimationState` JSON plus `PreviousGroup`. Post-load: `RestoreAnimationAfterLoad()` on character re-broadcasts state to AnimBP.

## Debug Tools (F1 menu / DevToolsLibrary)

| API | Purpose |
|-----|---------|
| `PreviewAnimation` | Set animation group |
| `PreviewMontage` | Preview montage ID |
| `PreviewRoutineChain` | Start morning/evening chain |
| `ShowIkTargets` | Debug-draw foot/hand/head IK |
| `ShowInteractionMarkers` | Debug-draw furniture alignment |
| `ForceFacialExpression` | Override facial state |
| `ResetAnimationState` | Return to idle |

## Tests

- `ApartmentLife.CharacterPipeline.AnimationTransition`
- `ApartmentLife.CharacterPipeline.AnimationSaveRoundTrip`
- `ApartmentLife.CharacterPipeline.FacialExpression`

## Done Criteria

1. Core activities animate without major snapping — transition kinds + montage bridge IDs
2. Sit/stand/sleep/wake — alignment + transition mapping
3. Yoga smooth pose transitions — dedicated pose animation API
4. Computer work aligns with desk/chair — socket + alignment resolution
5. Mirror/wardrobe routines align — grooming step animation + closet/mirror sockets
6. Shower routine non-explicit — `ShowerRoutine` group, privacy-aware activity IDs unchanged
7. Hair/cloth physics flags — `bPhysicsHairEnabled` / `bPhysicsClothEnabled` per group
8. Camera framing — alignment `CameraFocusPoint` + existing activity camera modes
9. Animation state survives save/load — full state JSON round-trip
10. Debug tools preview/test animations — dev library + debug menu entries

## Content Hook (AnimBP)

Bind in `ABP_SimCharacter`:

- `OnAnimationGroupChanged` → state machine
- `OnAnimationStateChanged` → montage layer, IK targets, facial curves
- `WalkStyleId` / `IdleStyleId` → blend spaces
- `IkTargets` → Full Body IK / Control Rig

Montage naming convention: `montage.<activity_path>` (e.g. `montage.work.computer`, `montage.hygiene.shower`).

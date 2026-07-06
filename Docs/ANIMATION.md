# Animation Pipeline

C++ owns activity state and timing; animation is authored in Content using UE5.6 modern animation tools.

## Required Engine Features

| Feature | Usage |
|---------|-------|
| **Motion Matching** | Locomotion (walk, run) in open apartment spaces |
| **Full Body IK** | Foot placement on floors, hand IK on furniture interactions |
| **Control Rig** | Procedural spine/head look-at, eye tracking |
| **State Machine** | High-level states: Idle, Locomote, Activity, Sleep |
| **Blend Spaces** | Speed-based locomotion blends |
| **Cloth Physics** | Clothing via `bEnableClothPhysics` on wardrobe items |
| **Hair Physics** | Groom binding on character archetypes |
| **Facial Animation** | ARKit-style curves or custom anime blend shapes |
| **Eye / Head Tracking** | Control Rig + player/camera target |

## AnimBP Architecture

```
ABP_SimCharacter
├── Locomotion (Motion Matching OR blend space fallback)
├── Slot: ActivityMontages
├── Layer: UpperBody (holding items, gestures)
├── Control Rig: IK_Feet, IK_Hands, LookAt
└── Facial: LS facial curves driven by conversation/emotion
```

## Activity → Animation Binding

1. `UApartmentLifeActivityData::MontageId` maps to a montage or Motion Matching database tag
2. `UApartmentLifeActivityComponent::OnActivityStarted` → AnimBP event
3. AnimBP plays montage or transitions MM pose search
4. `OnActivityCompleted` → return to idle/locomotion

### Example Activity Montage IDs

| MontageId | Activity |
|-----------|----------|
| `montage.cooking.stir` | Cooking |
| `montage.cleaning.wipe` | Cleaning |
| `montage.hygiene.shower` | Showering |
| `montage.hygiene.brush_teeth` | Brushing teeth |
| `montage.fitness.yoga_pose` | Yoga |
| `montage.intimate.solo` | Masturbation (gated) |
| `montage.bodily.toilet` | Voiding (gated) |
| `montage.sleep.bed` | Sleeping |

## Furniture Interaction

Furniture Blueprints expose interaction sockets:

- `Socket_Interact` — primary use point
- `Socket_Sit` — seated activities
- `Socket_Lie` — sleep

`UApartmentLifeActivityData::RequiredFurnitureTag` must match furniture `GameplayTag`.

## Conversation / Social

Dynamic conversations are Blueprint-driven using:

- Personality from `FApartmentLifePersonalityProfile`
- Relationship affinity/trust
- Memory retrieval from `UApartmentLifeLifeSimulationComponent::Memories`

Facial animation layers receive emotion weights from dialogue system (future plugin).

## Performance Notes

- Motion Matching databases per environment type (indoor apartment)
- LOD: swap to simplified AnimBP at distance
- Cloth/hair sim disabled on distant NPCs via significance manager
- Facial LOD: full curves < 5m, jaw blink only beyond

## Phase 2 Roadmap

- Pose Search database per activity category
- ML/decompressor for high-fidelity anime locomotion
- Lip sync from voice profile IDs on appearance component

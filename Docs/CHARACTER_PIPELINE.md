# Character Pipeline

Premium anime-inspired character customization, body morphing, layered wardrobe, animation hooks, yoga mini-game, grooming routines, and NPC style AI.

## Plugin Map

| Plugin | Responsibility |
|--------|----------------|
| `ApartmentLifeCharacter` | Creator state, face/body morphs, body fit profile, collision |
| `ApartmentLifeWardrobe` | Layered clothing, favorites, outfit contexts, clothing fit |
| `ApartmentLifeCharacterPipeline` | Animation groups, yoga, grooming, NPC style scoring |
| `ApartmentLifeCamera` | Character face/outfit/full-body focus, pose/animation preview |

## Character Creator

All options are **data-driven** via `UApartmentLifeMorphCatalogData`, hairstyle/voice/personality catalog assets, and `FApartmentLifeCharacterCreatorState`.

### Supported Customization

| Category | Struct / Field |
|----------|----------------|
| Face regions | `FApartmentLifeFaceCustomization` — shape, eyes, brows, nose, lips, jaw, cheeks, ears |
| Body | `FApartmentLifeBodyCustomization` — height, proportions, muscle, posture, walk/idle style |
| Colors | Skin, hair, eyes, makeup, nails |
| Accessories | Glasses, jewelry, tattoos |
| Presets | Voice, personality, favorite colors |

`UApartmentLifeCharacterCreatorComponent` extends the legacy appearance component and serializes full creator state to JSON for save/load.

## Body System

`UApartmentLifeBodyCustomizationComponent` applies:

- Skeletal mesh scale and morph targets
- Capsule collision resize from height and shoulder width
- `FApartmentLifeBodyFitProfile` for wardrobe sleeve/pant offsets

Body updates broadcast `OnBodyCustomizationUpdated`. `AApartmentLifeSimCharacter` forwards the fit profile to `UApartmentLifeClothingFitComponent`.

### Body → Downstream Systems

| System | Hook |
|--------|------|
| Clothing fit | `UApartmentLifeClothingFitComponent::RefreshClothingFit` |
| Animation | Walk/idle style IDs from body component |
| Collision | Capsule radius/height |
| Camera | Character focus uses actor bounds |
| Interaction | IK alignment via AnimBP + furniture sockets |

## Wardrobe System

See `ApartmentLifeWardrobeTypes.h` for categories (tops, bottoms, dresses, jackets, sleepwear, athletic, formal, shoes, socks, hats, glasses, jewelry, bags).

Features:

- Outfit presets and favorite outfits (`SaveCurrentAsFavorite`, `ApplyFavoriteOutfit`)
- Context selection: work, lounge, athletic, formal, sleep, weather-aware
- Laundry state (`MarkLayerDirty`, `StartLaundryCycle`)
- NPC style profile (`FApartmentLifeWardrobeStyleProfile`)
- Per-item fit ranges on `UApartmentLifeClothingItemData`

## Animation System

C++ owns **state and IDs**; authored animation lives in Content (Motion Matching, Full Body IK, Control Rig, AnimBP).

`UApartmentLifeAnimationComponent` tracks `FApartmentLifeAnimationState` and fitness progress. Activity changes on `AApartmentLifeSimCharacter` map to animation groups via `UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity`.

### Animation Groups

Idle, walking, running, sitting variants, lying/sleeping, cooking, eating, cleaning, laundry, reading, gaming, computer use, TV, yoga, stretching, exercising, dancing, dressing, grooming, shower routine (non-explicit hygiene), mirror routine, conversation gestures.

Data assets: `UApartmentLifeAnimationSetData`, yoga poses, grooming routines.

## Yoga Mini-Game

`UApartmentLifeYogaMinigameComponent`:

- Pose matching score
- Balance meter
- Breathing rhythm
- Flexibility and fitness XP
- Mat placement requirement (`bOnYogaMat`)
- Outfit compatibility via clothing data flags

Pair with `AApartmentLifeCameraPawn::FocusCharacter` using `PosePreview` for athletic framing.

## Grooming & Daily Routines

`UApartmentLifeGroomingRoutineComponent` runs ordered steps:

Shower (hygiene routine, non-explicit), brush teeth, wash face, hair care, makeup, skin care, change clothes, mirror check, laundry, closet organization.

Each step improves hygiene, confidence, and schedule readiness (`GetScheduleReadinessBonus`).

## Camera Support

`AApartmentLifeCameraPawn` modes:

| Mode | Use |
|------|-----|
| Orbit / Pan / Zoom | General inspection |
| Photo | DOF and cinematic framing |
| `CharacterFace` | Creator face sliders |
| `CharacterOutfit` | Wardrobe preview |
| `CharacterFullBody` | Body proportions |
| `PosePreview` | Yoga pose matching |
| `AnimationPreview` | Activity montage preview |
| Character rotation | `SetCharacterPreviewRotationEnabled` rotates the focused character instead of the camera |

## NPC Style AI

`UApartmentLifeNPCStyleComponent` scores wardrobe items using income, career, mood, weather, occasion, and personality tags via `UApartmentLifeCharacterPipelineLibrary::ScoreClothingForNPCStyle`.

`AApartmentLifeSimCharacter` refreshes style when:

- Game hour advances (schedule/occasion)
- Creator state changes (favorite colors, personality)
- Simulation mood/career context updates

A high-income professional receives different scoring weights than a student, artist, athlete, or gamer based on price thresholds and style tags.

## Save / Load

Persisted via `IApartmentLifeSaveable`:

| Component | Data |
|-----------|------|
| Creator | Full `FApartmentLifeCharacterCreatorState` JSON |
| Body | `FApartmentLifeBodyCustomization` JSON |
| Wardrobe | Equipped slots, favorites, closet inventory |
| Animation | Fitness progress |
| Grooming | Routine state and preferences |

Register saveables with `UApartmentLifeSaveSubsystem` on character spawn.

## Content Authoring Checklist

1. Create morph catalog and hairstyle/voice/personality data assets
2. Author `ABP_SimCharacter` with motion matching + IK + facial layers
3. Bind `OnAnimationGroupChanged` in AnimBP
4. Create clothing items with fit ranges and yoga compatibility flags
5. Author yoga pose montages and grooming step activities
6. Wire closet UI Blueprint to wardrobe component delegates

## Tests

Automation tests in `ApartmentLifeCharacterPipelineTests.cpp`:

- Activity → animation group mapping
- Yoga pose match scoring
- Grooming step advancement
- Body scale from height

Run in UE Editor: **Session Frontend → Automation → ApartmentLife.CharacterPipeline**

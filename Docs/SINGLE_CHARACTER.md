# Single Character Build (MP08)

Private single-player build centered on **one customizable girl** in one apartment. No city simulation, no NPC population, no multiplayer.

## Scope

| In scope | Deferred |
|----------|----------|
| One apartment | City / open world |
| One girl character | Multiple NPCs |
| Player camera | NPC social networks |
| Wardrobe + furniture | Large career system |
| Daily routine + dialogue | Traffic / public events |
| Yoga, grooming, shower (non-explicit) | Marketplace / cloud |
| Computer-based income | Mod browser |

## The Girl

`AApartmentLifeSimCharacter` with ID `character.main` tracks:

| System | Fields |
|--------|--------|
| Appearance | Creator, body, hair, makeup (components) |
| Wardrobe | `UApartmentLifeWardrobeComponent` |
| Mood | `Mood` — happiness, stress, energy, confidence, comfort |
| Needs | `Needs` — hygiene, hunger, fun, social |
| Player bond | `AffectionTowardPlayer`, `TrustTowardPlayer` |
| Finance | `Finance.Savings`, computer work payouts |
| Memory | `Memories`, conversation history |
| Routine | Schedule + activity system |

## Entry Point

| Setting | Class |
|---------|-------|
| Game mode | `AApartmentLifeSingleCharacterGameMode` |
| Controller | `AApartmentLifeSingleCharacterPlayerController` |
| Pawn | `AApartmentLifeCameraPawn` |

## Core Loop

1. Customize her appearance (character creator camera)
2. Help choose outfits (G / closet interact)
3. Decorate apartment (B build mode)
4. Direct daily activities (E interact — bed, shower, desk, yoga mat, kitchen)
5. Talk with her (Q — `StartConversationWithPlayer`)
6. Earn income at computer (`activity.work.computer`)
7. Save / load (F5 / F9)

## Income

Remote work careers configured via `UApartmentLifeGirlLifeLibrary::ConfigureRemoteWorkCareer`:

- `career.remote_programmer` (default)
- `career.digital_artist`
- `career.online_tutor`
- `career.content_editor`
- `career.virtual_assistant`
- `career.freelancer`

Computer sessions pay on **activity completion** based on hourly wage × duration × productivity. Passive daily salary is disabled for remote careers; daily expenses still apply.

## Activity Effects

`UApartmentLifeGirlLifeLibrary::ApplyActivityCompletion` updates hygiene, energy, mood, affection, and savings when activities finish. Builtin durations apply when no data assets exist (`UApartmentLifeActivityFallbackLibrary`).

## Input

| Key | Action |
|-----|--------|
| E | Girl interacts with furniture under cursor |
| B | Toggle build mode |
| G | Wardrobe / outfit focus |
| Q | Talk with girl |
| 1 | Focus face |
| 2 | Focus outfit |
| F5 / F9 | Quick save / load |

## Save Data

Persists through `IApartmentLifeSaveable`:

- Character appearance, body, wardrobe, outfit
- Furniture placement, apartment state
- Mood, energy, hygiene, affection, trust, savings
- Time of day, memories, conversation history
- Social subsystem (invitations empty in solo build)

City subsystem is **disabled** in `AdultAnimeApartmentLife.uproject`.

## Daily Routine (Target)

Morning: wake → groom → shower → outfit → breakfast → computer work  
Afternoon: work → break → yoga → clean → shop  
Evening: dinner → talk → relax → wardrobe → read/TV → sleep

Schedule templates and data assets can be added without C++ changes.

## Quality Bar

Focused, private, smooth, achievable. One excellent character experience before expanding.

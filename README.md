# Adult Anime Apartment Life

Premium single-player anime-inspired life simulator for adults, built on **Unreal Engine 5.6**.

Centered around realistic cohabitation, apartment customization, careers, relationships, and daily life — where every character lives independently and nothing waits for the player.

## Core Principles

- **Living world** — NPCs manage jobs, income, bills, hobbies, relationships, and schedules autonomously
- **Data-driven** — gameplay content is defined in Primary Data Assets, not hardcoded
- **Modular plugins** — each major system is an isolated UE plugin for expansion and reuse
- **Save compatible** — all persistent state flows through `IApartmentLifeSaveable`
- **Premium visuals** — Lumen, Nanite, HDR, cinematic camera, and anime material pipeline

## Quick Start

### Requirements

- Unreal Engine **5.6**
- Visual Studio 2022 (Windows) with C++ game development workload
- ~50 GB disk for engine + project

### Open the Project

1. Clone this repository
2. Double-click `AdultAnimeApartmentLife.uproject`
3. Allow UE to compile C++ modules on first launch
4. See [Docs/GETTING_STARTED.md](Docs/GETTING_STARTED.md) for editor setup

## Plugin Architecture

| Plugin | Responsibility |
|--------|----------------|
| **ApartmentLifeCore** | Game time, data registry, save interfaces, content settings, shared types |
| **ApartmentLifeCamera** | Orbit camera, zoom, collision, focus lock, free cam, photo mode, DoF |
| **ApartmentLifeCharacter** | Modular appearance: face, body, hair, eyes, makeup, tattoos, voice |
| **ApartmentLifeWardrobe** | Clothing layers, presets, laundry, seasonal/occasion outfit selection |
| **ApartmentLifeApartment** | Apartment builder, furniture, inventory, decoration, room scoring |
| **ApartmentLifeAI** | Schedules, economy, bills, relationships, memories, personalities |
| **ApartmentLifeWorldSim** | Schedules, careers, finance, mood, memory, relationships, shopping, events, save |
| **ApartmentLifeCharacterPipeline** | Animation groups, yoga mini-game, grooming routines, NPC style AI |

See [Docs/ARCHITECTURE.md](Docs/ARCHITECTURE.md) for system diagrams and extension points.

See [Docs/WORLD_SIMULATION.md](Docs/WORLD_SIMULATION.md) for the simulation engine.
See [Docs/APARTMENT_BUILDER.md](Docs/APARTMENT_BUILDER.md) for the apartment builder system.
See [Docs/CHARACTER_PIPELINE.md](Docs/CHARACTER_PIPELINE.md) for character creator, wardrobe, and animation pipeline.

## Key Classes

| Class | Module | Purpose |
|-------|--------|---------|
| `AApartmentLifeCameraPawn` | Camera | Player inspection camera |
| `AApartmentLifeSimCharacter` | Game | Fully composed sim character |
| `AApartmentLifeApartmentUnit` | Apartment | Customizable apartment root |
| `UApartmentLifeNPCSimulationComponent` | WorldSim | Master NPC life engine |
| `UApartmentLifeSaveSubsystem` | Core | Full world save/load |
| `UApartmentLifeWardrobeComponent` | Wardrobe | Clothing and laundry |
| `UApartmentLifeActivityComponent` | Activities | Activity execution |
| `UApartmentLifeGameTimeSubsystem` | Core | World clock — never pauses for player |
| `UApartmentLifeCharacterCreatorComponent` | Character | Full character creator state |
| `UApartmentLifeAnimationComponent` | CharacterPipeline | Animation group and fitness state |
| `UApartmentLifeYogaMinigameComponent` | CharacterPipeline | Yoga pose matching mini-game |

## Content Pipeline

All gameplay content is authored as Primary Data Assets under `/Game/Data/`. See [Docs/CONTENT_PIPELINE.md](Docs/CONTENT_PIPELINE.md).

## Animation

Animation uses Motion Matching, Full Body IK, Control Rig, and facial systems configured in Blueprint/AnimBP. See [Docs/ANIMATION.md](Docs/ANIMATION.md).

## Input Defaults

| Action | Key |
|--------|-----|
| Orbit camera | Right mouse |
| Pan camera | Middle mouse |
| Zoom | Scroll wheel |
| Free camera | F |
| Photo mode | P |
| Focus lock | L |
| Interact | E |

## Platform Targets

- **Windows** — primary target (DX12, Lumen HWRT)
- **Web / Mobile** — future; architecture avoids platform-specific gameplay code

## License

Proprietary. All rights reserved.

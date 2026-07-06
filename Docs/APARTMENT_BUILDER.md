# Apartment Builder System

Complete apartment customization: structure, build mode, furniture, inventory, decoration, cleaning, and room scoring.

## Components on `AApartmentLifeApartmentUnit`

| Component | Purpose |
|-----------|---------|
| `UApartmentLifeInventoryComponent` | Owned/placed/stored/sold items |
| `UApartmentLifeBuildModeComponent` | Player build/decorate mode with undo/redo |
| `UApartmentLifeCleaningComponent` | Dirt, wear, maintenance |
| `UApartmentLifeDecorationComponent` | NPC decoration AI |
| `UApartmentLifeFurniturePlacementComponent` | Spawns `AApartmentLifeFurnitureActor` instances |

## Apartment Layouts

Create `UApartmentLifeApartmentLayoutData` assets:

| Layout | Typical sq ft |
|--------|---------------|
| Studio | 400–500 |
| One Bedroom | 600–750 |
| Two Bedroom | 900–1100 |
| Luxury | 1200+ |
| Shared | 500–700 |

Metadata includes rent, neighborhood quality, noise, natural light, storage, utilities, safety, commute, building rules.

## Build Mode

```cpp
BuildMode->EnterBuildMode();
BuildMode->SetPlacementMode(EApartmentLifeBuildPlacementMode::SnapToGrid);
BuildMode->PlaceItem(FName("furniture.sofa.modern"), Transform, EApartmentLifeRoomType::LivingRoom);
BuildMode->Undo();
```

| Action | API |
|--------|-----|
| Select | `SelectInstance(Guid)` |
| Move | `MoveSelected(Transform)` |
| Rotate | `RotateSelected(YawDelta)` |
| Duplicate | `DuplicateSelected()` |
| Sell | `SellSelected()` |
| Store | `StoreSelected()` |
| Recolor | `RecolorSelected(Color)` |

Camera build modes: `SetBuildTopDownMode(true)`, `FocusRoom(RoomBounds)`.

## Furniture Data Assets

`UApartmentLifeFurnitureItemData` defines all object properties:

- Category (22 types: bed, sofa, desk, appliances, etc.)
- Price, footprint, placement rules, style tags
- Color/material options, comfort, durability, maintenance
- Interaction points and animation hooks

## Style Tags

Minimalist, Cozy, Luxury, Gamer, Academic, Artistic, Fitness, Romantic, Traditional, Modern, Budget, Designer.

NPC decoration scores items by personality + style match via `UApartmentLifeDecorationComponent`.

## Room Scoring

`RecalculateRoomScores()` computes per room:

- Comfort, Cleanliness, Style Match, Functionality
- Clutter, Lighting Quality, Entertainment Value

Scores affect mood (`GetMoodImpact`), sleep (`GetSleepQualityImpact`), productivity (`GetProductivityImpact`).

## Cleaning & Maintenance

Hourly simulation applies dirt/wear via `ApplyWearAndDirt()`. Tidiness-driven `AutoCleanByTidiness()` on NPC apartments.

## NPC Decoration AI

```cpp
FApartmentLifeDecorationProfile Profile;
Profile.PreferredStyles = { EApartmentLifeStyleTag::Minimalist };
Profile.MonthlyBudget = 300.f;
Profile.Tidiness = 0.8f;
DecorationComponent->SetDecorationProfile(Profile);
DecorationComponent->RunDecorationPass(AvailableBudget);
```

High-income minimalist NPCs buy different items than low-income collectors — scored in `ScoreFurnitureForDecorator()`.

## Interaction Hooks

`AApartmentLifeFurnitureActor::TryInteract()` returns activity IDs for beds, sofas, desks, kitchens, bathrooms, closets, workout equipment.

## Save / Load

Apartment unit persists via `IApartmentLifeSaveable`:

- Placed furniture (transform, condition, color)
- Apartment metadata
- Inventory (separate component save)

## Tests

- `ApartmentLife.Builder.GridSnap`
- `ApartmentLife.Builder.RoomScore`
- `ApartmentLife.Builder.InteractionHooks`

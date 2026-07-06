# Content Pipeline

All gameplay content lives under `/Game/` using Unreal Primary Data Assets. **No gameplay values in C++.**

## Folder Structure

```
Content/
├── Characters/          # Meshes, AnimBPs, character BPs
├── Clothing/            # Clothing meshes, cloth assets
├── Data/
│   ├── Activities/
│   ├── Characters/
│   ├── Clothing/
│   ├── Furniture/
│   └── Rooms/
├── Maps/
├── Materials/           # Anime master materials, instances
├── UI/
└── VFX/
```

## Data Asset Naming

| Prefix | Class | Example |
|--------|-------|---------|
| `DA_Activity_` | `UApartmentLifeActivityData` | `DA_Activity_Cooking_Pasta` |
| `DA_Clothing_` | `UApartmentLifeClothingItemData` | `DA_Clothing_Casual_Hoodie` |
| `DA_Outfit_` | `UApartmentLifeOutfitPresetData` | `DA_Outfit_Work_Default` |
| `DA_Furniture_` | `UApartmentLifeFurnitureItemData` | `DA_Furniture_Sofa_Modern` |
| `DA_Room_` | `UApartmentLifeRoomDefinitionData` | `DA_Room_Bedroom_Standard` |
| `DA_Character_` | `UApartmentLifeCharacterCustomizationData` | `DA_Character_Archetype_Female01` |

## Asset ID Convention

`AssetId` must be **stable across updates** for save compatibility:

```
clothing.casual.hoodie_blue
activity.cooking.pasta
furniture.sofa.modern_grey
character.archetype.female_01
```

Use lowercase dot-separated paths. Never rename `AssetId` after shipping — add new IDs instead.

## Activity Content Matrix

| Category | Example Asset IDs | Notes |
|----------|-------------------|-------|
| Cooking | `activity.cooking.*` | Requires kitchen furniture tag |
| Hygiene | `activity.hygiene.shower` | `bRequiresPrivacy` |
| Grooming | `activity.grooming.*` | Bathroom or bedroom |
| Bodily Function | `activity.bodily.*` | `bRequiresExplicitContent` |
| Intimate | `activity.intimate.*` | Gated by content settings |
| Sleep | `activity.sleep.night` | Bedroom furniture tag |
| Work | `activity.work.remote` | Office room |
| Fitness | `activity.fitness.yoga` | Living room or balcony |

## Clothing Authoring

Per `UApartmentLifeClothingItemData`:

1. Assign `ClothingMesh` (skinned to character skeleton)
2. Set `Layer` and `Category`
3. Add `SeasonalTags` and `SuitableOccasions`
4. Set `WarmthRating` (0 = light, 1 = heavy)
5. Enable `bEnableClothPhysics` for coats, skirts, etc.
6. Mark `bRequiresMatureContent` for nude category items

## Furniture Authoring

Per `UApartmentLifeFurnitureItemData`:

1. Assign Nanite-enabled `Mesh`
2. Set `AllowedRooms` and `SupportedActions`
3. Set `bAllowScale` for rugs, plants — disable for appliances
4. Tag interaction points in Blueprint child for activity system

## Material Guidelines (Anime Premium)

- Master material: toon ramp + rim light + subtle subsurface on skin
- Parameterize skin tone, hair color, clothing tint via `FApartmentLifeColorCustomization`
- Use Lumen-friendly roughness ranges (0.3–0.7 skin, 0.1–0.4 hair highlights)
- 2K–4K textures for hero characters; 1K for background props

## Registry Bootstrap

On game start, load a Primary Asset bundle and register:

```cpp
// Typically in GameInstance Blueprint or subsystem
Registry->RegisterAsset(LoadedActivityData);
```

Or scan `/Game/Data` with Asset Manager `LoadPrimaryAssetsWithType`.

## Localization

Use `FText DisplayName` and `Description` on all data assets. Never localize `AssetId`.

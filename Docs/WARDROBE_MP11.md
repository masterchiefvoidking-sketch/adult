# Wardrobe, Clothing & Style System (MP11)

Full wardrobe loop for the private single-character apartment simulator: browse, preview, equip, purchase, filter, outfit presets, laundry, mood effects, and save/load.

## Done Criteria

The player can:

1. Open wardrobe (G key or closet interactable)
2. Rotate and zoom the character (wardrobe camera from MP10)
3. Preview clothing before equipping or buying
4. Buy clothing with savings (shopping mode)
5. Equip owned clothing
6. Save, rename, and delete outfit presets
7. Filter and sort by category, slot, search, price, style, owned, favorites
8. Use activity-based outfit recommendations
9. See outfit effects on mood, confidence, and comfort
10. Save, reload, and keep all wardrobe state

## Architecture

| Component | Role |
|-----------|------|
| `UApartmentLifeWardrobeComponent` | Owned inventory, equipped slots, presets, laundry, purchase, preview |
| `UApartmentLifeWardrobeCatalogLibrary` | ~35 builtin clothing items (no `.uasset` required) |
| `UApartmentLifeWardrobeLibrary` | Filter/sort, activity recommendations, mood effects |
| `UApartmentLifeWardrobeUiController` | UI state + on-screen overlay (category tabs, search, sort) |
| `UApartmentLifeWardrobeShoppingComponent` | Browse/purchase with income-tier gating |
| `UApartmentLifeClothingFitComponent` | Body-driven sleeve/pant offsets and fit validation |
| `UApartmentLifeWardrobeBootstrapLibrary` | Seeds starter closet + named presets |

## Clothing Categories

Casual, Work-from-home, Lounge, Sleepwear, Athletic/Yoga, Formal, Seasonal, Accessories.

## Clothing Slots

Hair accessory, Glasses, Earrings, Necklace, Top, Jacket, Bottom, Dress, Socks, Shoes, Bracelet/Watch, Bag.

## Outfit Presets

Bootstrap seeds six named presets plus custom favorites:

- Morning (`preset.morning`)
- Work (`preset.work`)
- Lounge (`preset.lounge`)
- Sleep (`preset.sleep`)
- Yoga (`preset.yoga`)
- Going Out (`preset.going_out`)

## Shopping

Local-only shopping via `UApartmentLifeWardrobeShoppingComponent`:

- Browse full catalog with filters
- Preview before purchase
- Purchase deducts savings via `UApartmentLifeWardrobeComponent::PurchaseClothing`
- Income tier gates luxury items (`GetIncomeTierForSavings`)
- Overspending adds stress; affordable purchases boost confidence

Enable shopping mode in the UI controller to show unowned items within the player's income tier.

## Laundry

Simple state machine on equipped slots: Clean → Worn → Dirty → In Wash → Drying → Clean.

- `MarkEquippedWorn()` runs after activities complete
- `StartLaundryCycle()` / `AdvanceLaundryCycle()` restore clean state
- Dirty/worn clothing reduces comfort and confidence via mood effects

## Mood Effects

`UApartmentLifeWardrobeLibrary::ComputeOutfitMoodEffect` considers:

- Average comfort rating of equipped items
- Favorite item bonuses (+confidence, +mood)
- Dirty/worn penalties (-comfort, -confidence)
- Activity mismatch (e.g. non-yoga shoes during yoga)

Applied automatically when activities start on `AApartmentLifeSimCharacter`.

## Activity Recommendations

`RecommendOutfitForActivity` maps activities to outfit contexts:

| Activity | Recommended context |
|----------|---------------------|
| Yoga / fitness | Athletic |
| Computer work | Work |
| Sleep | Sleepwear |
| Lounge / relax | Lounge |

Falls back to highest-scoring owned items per layer when no preset matches.

## Save / Load

`UApartmentLifeWardrobeComponent` persists via `IApartmentLifeSaveable`:

- Equipped slots and preview slots
- Closet inventory and favorite item IDs
- Outfit presets (with `PresetType` and `DisplayName`)
- Shopping history
- Style profile

Wardrobe camera zoom is saved in `UApartmentLifeCameraSettingsSubsystem` (`PreferredWardrobeZoom`).

## Controls

| Key | Action |
|-----|--------|
| G | Open wardrobe + wardrobe camera |
| Mouse drag | Orbit character |
| Mouse wheel | Zoom |
| F5 / F9 | Quick save / load (includes wardrobe state) |

## Tests

Automation tests in `ApartmentLifeWardrobeTests.cpp`:

- `ApartmentLife.Wardrobe.VerticalSliceBootstrap`
- `ApartmentLife.Wardrobe.PurchaseClothing`
- `ApartmentLife.Wardrobe.FilterAndSort`
- `ApartmentLife.Wardrobe.OutfitMoodEffect`
- `ApartmentLife.Wardrobe.SaveRoundTrip`

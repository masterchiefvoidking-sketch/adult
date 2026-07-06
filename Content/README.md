# Content Directory

Unreal Engine content is authored in the editor and is not tracked in this repository scaffold.

See `/Docs/CONTENT_PIPELINE.md` and `/Docs/SOLO_BUILD.md` for data asset conventions and the private solo-build folder layout.

Create the following top-level folders in the UE Content Browser:

```
Characters/
  Body/
  Hair/
  Clothing/
  Animations/
Apartments/
  Rooms/
  Furniture/
  Materials/
Gameplay/
  Camera/
  Interaction/
  AI/
  Schedules/
  Relationships/
  Activities/
UI/
  CharacterCreator/
  Wardrobe/
  BuildMode/
  Dialogue/
  Inventory/
Data/
  Characters/
  Clothing/
  Furniture/
  Jobs/
  Activities/
  Dialogues/
Maps/
Materials/
VFX/
SaveSystem/
DeveloperTools/
Tests/
```

## Vertical Slice Minimum Content

To replace C++ placeholders with real assets, author first:

1. One apartment layout data asset (`DA_Apartment_OneBedroom`)
2. Six furniture data assets matching starter IDs (`furniture.bed.default`, etc.)
3. One character archetype per sim character
4. A small casual outfit set (top, bottom, footwear)
5. `Maps/DevSandbox` — empty floor + lighting for PIE

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

## Single Character Minimum Content

Author first for `character.main`:

1. One apartment layout data asset
2. Eight furniture data assets (bed, closet, desk, sofa, kitchen, shower, mirror, yoga mat)
3. One girl character archetype + AnimBP
4. Small casual outfit set
5. `Maps/DevSandbox` — empty floor + lighting for PIE

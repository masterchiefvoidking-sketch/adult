# City & Open World Simulation

Persistent city layer for districts, buildings, POIs, careers, skills, economy, transportation, weather, and world events.

## Plugin Map

| Plugin | Responsibility |
|--------|----------------|
| `ApartmentLifeCity` | Districts, buildings, POIs, economy, transit, events, LOD, background sim |
| `ApartmentLifeWorldSim` | NPC careers, skills, shopping, schedules, world events |
| `ApartmentLifeCore` | Time, weather, save system |

## City Districts

`EApartmentLifeCityDistrict` supports downtown, residential, luxury, university, shopping, business, waterfront, entertainment, parks, and suburbs.

Each `UApartmentLifeDistrictData` asset defines:

| Stat | Purpose |
|------|---------|
| Population | Density baseline |
| Average income | NPC economic profile |
| Crime level | Safety modifier |
| Average rent | Housing cost |
| Property value index | Rent multiplier |
| Traffic density | Commute and mood |
| Noise level | Apartment comfort |
| Public transit | Transport availability |
| Local events | District-scoped event IDs |

## Buildings

`EApartmentLifeBuildingType` covers apartment buildings, grocery/clothing/furniture/electronics stores, cafés, restaurants, bakeries, libraries, bookstores, gyms, yoga studios, parks, offices, universities, hospitals, salons, and convenience stores.

`UApartmentLifeCityBuildingData` includes:

- Opening hours (`FApartmentLifeBuildingHours`)
- Employee and customer capacity
- Store inventory (`FApartmentLifeStoreInventory`)
- Linked apartment unit IDs

## Careers & Skills

### Careers (`UApartmentLifeCareerData`)

Extended with workplace POI, work district, stress per work day, vacation days, dress code, and required skill levels.

Supported career archetypes (via data assets): software engineer, office worker, teacher, nurse, artist, designer, retail, chef, barista, librarian, student, freelancer, fitness instructor.

### Skills (`FApartmentLifeSkillSet`)

Cooking, fitness, creativity, programming, art, music, organization, communication, leadership, home maintenance.

Skills improve through activities via `UApartmentLifeWorldSimCityBridge::ApplyActivitySkillGain`.

## Shopping

NPC shopping uses WorldSim `EvaluateShoppingNeeds` with income, budget, personality, and season.

City layer adds:

- Per-building inventories and stock levels
- Open/closed state by hour and day phase
- Sale discounts during city events

## Transportation

`EApartmentLifeTransportMode`: walking, bicycle, public transit, ride share, personal car.

`UApartmentLifeTransitRouteData` defines travel minutes and cost between districts.

`UApartmentLifeCityLibrary::ComputeCommuteMinutes` feeds schedule planning.

## Weather & Day/Night

Global weather from `UApartmentLifeGameTimeSubsystem` affects:

- District traffic (`ApplyWeatherToTraffic`)
- NPC mood and wardrobe
- Outdoor activity availability

`EApartmentLifeDayPhase`: morning rush, business hours, afternoon, evening, late night.

Businesses open/close via `IsBusinessOpen`.

## Economy

`FApartmentLifeCityEconomyState` tracks:

- Employment rate
- Housing demand
- Average salary
- Rent fluctuation
- Seasonal demand multiplier
- Active sale events

Daily ticks via `TickCityEconomy`.

## World Events

### City events (`UApartmentLifeCityEventData`)

Holiday festivals, farmers markets, concerts, community fairs, apartment meetings, seasonal celebrations, sporting events, neighborhood cleanups, sale events.

`UApartmentLifeCitySubsystem` rolls events daily and broadcasts `OnCityEventStarted`.

### Random events (`UApartmentLifeRandomEventData`)

WorldSim events now apply `MoodImpact`, `FinancialImpact`, and `TriggeredActivityId` to NPCs.

## Optimization

`EApartmentLifeSimulationLOD`:

| LOD | Distance | Behavior |
|-----|----------|----------|
| Full | Player district / < 2km | Full animation and interaction |
| Reduced | < 5km | Simplified updates |
| Background | < 12km | Schedule-only simulation |
| Dormant | > 12km | Minimal tick |

`UApartmentLifeCitySubsystem::SimulateBackgroundDistricts` runs hourly for non-player districts.

Object pooling and World Partition streaming are configured in Content/engine settings.

## Save System

Save version bumped to **2**.

Persisted:

- City economy, active events, NPC locations, player district
- NPC skills, relationships, memories, mood (WorldSim)
- World time and weather (Core)

`UApartmentLifeCitySubsystem` implements `IApartmentLifeSaveable`.

## Content Authoring Checklist

1. Create `DA_District_*` assets for each neighborhood
2. Create `DA_Building_*` and `DA_POI_*` for shops, offices, homes
3. Create `DA_Career_*` with workplace POI and district links
4. Create `DA_TransitRoute_*` between districts
5. Create `DA_CityEvent_*` for festivals and sales
6. Link apartment units to building IDs
7. Configure World Partition on city map for streaming

## Tests

Automation tests in `ApartmentLifeCityTests.cpp`:

- Day phase calculation
- Commute minutes
- Simulation LOD tiers
- Building hours

Run in UE Editor: **Session Frontend → Automation → ApartmentLife.City**

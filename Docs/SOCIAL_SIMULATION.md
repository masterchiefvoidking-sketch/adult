# Social Simulation

Dialogue, relationships, memory, emotional AI, romance progression, reputation, and NPC social autonomy for believable everyday life.

## Plugin Map

| Plugin | Responsibility |
|--------|----------------|
| `ApartmentLifeWorldSim` | Personality traits, mood state, relationship records, memory records, NPC life engine |
| `ApartmentLifeSocial` | Conversation engine, topics, invitations, romance gates, reputation, NPC friendship autonomy |

## Personality System

`FApartmentLifePersonalityTraits` drives dialogue tone, schedule priorities, spending, apartment style, friendships, stress response, hobbies, and decisions.

### Traits (0–1)

Confidence, kindness, introversion, extroversion, humor, patience, empathy, ambition, tidiness, creativity, curiosity, responsibility, generosity, optimism, emotional openness.

Interest floats (cooking, gaming, fitness, reading, music) influence topic affinity and shared-interest scoring.

## Mood System

`FApartmentLifeMoodState` tracks:

| Field | Description |
|-------|-------------|
| Happiness / OverallMood | General emotional state |
| Stress, Energy, Fatigue | Daily capacity |
| Confidence, Motivation | Self-drive |
| Loneliness, Comfort | Social/emotional wellbeing |
| SocialBattery | Capacity for interaction |

`FApartmentLifeMoodInfluences` feeds recalculation from sleep, work, apartment cleanliness, weather, finances, meals, exercise, social fulfillment, and personal achievements.

## Memory System

`FApartmentLifeMemoryRecord` stores categorized memories with strength, emotional weight, timestamps, and optional summary text.

### Categories

Person met, conversation, gift, invitation, holiday, birthday, shared hobby, apartment visit, personal milestone, promise kept/broken, shared activity, life event, relationship milestone.

Recent memories weigh more via `GetRecencyWeight()` unless reinforced.

## Relationship Model

`FApartmentLifeRelationshipRecord` tracks separate axes:

| Axis | Purpose |
|------|---------|
| Familiarity | How well they know each other |
| Friendship | Platonic closeness |
| Trust | Reliability and honesty |
| Respect | Admiration of character |
| Comfort | Ease around each other |
| Admiration | Positive regard |
| Compatibility | Natural fit |
| SharedInterests | Hobby overlap |
| RomanceAttraction | Romantic potential |

Stages progress from stranger → acquaintance → friend → close friend → romantic interest → dating → partner via `UApartmentLifeSocialLibrary::GetStageFromRelationship`.

## Conversation Engine

`UApartmentLifeConversationComponent` manages session lifecycle:

1. `StartConversation` — pair with partner, build context
2. `AdvanceConversation` — select topic, generate line, update phase
3. `EndConversation` — apply relationship, memory, mood, and social fulfillment results

`UApartmentLifeSocialLibrary::BuildDialogueContext` assembles personality, mood, relationship, time, weather, location, career, and recent memories.

Dialogue lines are selected from data-driven `UApartmentLifeDialogueTopicData` assets with anti-repetition via conversation history.

## Topic System

`EApartmentLifeConversationTopic` includes work, school, movies, music, games, books, cooking, fitness, travel, fashion, technology, pets, food, holidays, home decorating, future plans, and more.

Topics unlock based on `MinFriendshipToDiscuss` and `MinTrustToDiscuss` on topic data assets. Deeper relationships unlock personal and future-plan topics.

## Social Activities & Invitations

`UApartmentLifeSocialSubsystem` orchestrates NPC autonomy:

- Registers sim characters on spawn
- Hourly friendship maintenance when social need is high
- Issues invitations (coffee, dinner, shopping, park walks, museums, concerts, festivals, movie nights, cooking, decorating, gaming, yoga, hiking, exercise)
- Accept/decline based on relationship score, mood, and personality
- Completing activities records shared-activity memories and relationship gains

## Romance Progression

Romance develops through compatibility, trust, quality conversations, and shared activities — not simple point grinding.

`UApartmentLifeRomanceMilestoneData` defines milestone requirements (stage, trust, compatibility, conversation count, shared activities).

`FApartmentLifeRomanceProgress` tracks attraction and achieved milestones.

## Reputation

`FApartmentLifeReputationState` per character tracks reliability, consideration, generosity, and overall reputation.

Actions like keeping promises, arriving on time, generosity, cancelling plans, and rudeness adjust scores via `ApplyReputationAction`.

## NPC Autonomy

NPCs independently:

- Maintain friendships when social need rises
- Issue and accept invitations
- Record memories from conversations and activities
- Update relationship stages
- Continue simulation without player presence

## Events

`UApartmentLifeSocialEventData` supports birthdays, holidays, promotions, apartment parties, neighborhood events, seasonal festivals, new neighbors, furniture sales, and hobby clubs.

## Save / Load

Persisted via `IApartmentLifeSaveable`:

| Component | Data |
|-----------|------|
| NPC Simulation | Personality, full mood, relationships, memories |
| Conversation | Conversation history |
| Social Subsystem | Pending invitations, reputation by character |

## Content Authoring Checklist

1. Create `UApartmentLifeDialogueTopicData` assets with opening/response/closing lines
2. Create `UApartmentLifeSocialActivityInviteData` for each invite type
3. Define `UApartmentLifeRomanceMilestoneData` for relationship arcs
4. Author `UApartmentLifeSocialEventData` for seasonal/holiday events
5. Wire UMG dialogue UI to `OnConversationLineSpoken` and `OnConversationEnded`
6. Bind facial animation emotion tags from dialogue lines in AnimBP

## Tests

Automation tests in `ApartmentLifeSocialTests.cpp`:

- Relationship stage derivation
- Reputation action scoring
- Topic affinity for personality interests
- Memory recency weighting

Run in UE Editor: **Session Frontend → Automation → ApartmentLife.Social**

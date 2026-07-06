// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDialogueCatalogLibrary.h"

namespace
{
	const TArray<FText>& PickPool(const TArray<FText>& Pool)
	{
		return Pool;
	}

	FText PickRandom(const TArray<FText>& Pool)
	{
		if (Pool.Num() == 0)
		{
			return FText::GetEmpty();
		}
		return Pool[FMath::RandRange(0, Pool.Num() - 1)];
	}
}

int32 UApartmentLifeDialogueCatalogLibrary::GetBuiltinDialogueLineCount()
{
	return 56;
}

FApartmentLifeDialogueLine UApartmentLifeDialogueCatalogLibrary::GenerateContextualPlayerLine(const FApartmentLifeDialogueContext& Context)
{
	FApartmentLifeDialogueLine Line;
	Line.LineId = FName(TEXT("dialogue.contextual.player"));

	const int32 Hour = Context.TimeOfDay.Hour;
	const float Mood = Context.SpeakerMood.Happiness;
	const float Energy = Context.SpeakerMood.Energy;
	const float Stress = Context.SpeakerMood.Stress;
	const float Savings = Context.SavingsSnapshot;
	const float Comfort = Context.ApartmentComfortScore;
	const bool bRain = Context.Weather.Weather == EApartmentLifeWeather::Rain || Context.Weather.Weather == EApartmentLifeWeather::Storm;

	TArray<FText> Pool;

	// Priority-ordered contextual selection
	if (Hour >= 5 && Hour < 10)
	{
		Pool = {
			FText::FromString(TEXT("Good morning... I slept pretty well.")),
			FText::FromString(TEXT("Morning! The light in here is really nice today.")),
			FText::FromString(TEXT("Hi... I'm still waking up, but it's a calm morning."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Personal;
	}
	else if (Stress > 65.f)
	{
		Pool = {
			FText::FromString(TEXT("Work has been a lot lately... my shoulders are so tense.")),
			FText::FromString(TEXT("I could use a quiet moment. Today felt overwhelming.")),
			FText::FromString(TEXT("Sorry if I seem distracted — I'm still thinking about work."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Work;
	}
	else if (Energy < 30.f)
	{
		Pool = {
			FText::FromString(TEXT("I'm so tired... maybe I should nap soon.")),
			FText::FromString(TEXT("My eyes keep closing. It's been a long day.")),
			FText::FromString(TEXT("I need to rest a little. Talking is nice, though."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Personal;
	}
	else if (Mood > 75.f)
	{
		Pool = {
			FText::FromString(TEXT("I feel really good today! Everything feels lighter.")),
			FText::FromString(TEXT("Something about today just makes me smile.")),
			FText::FromString(TEXT("I'm in such a happy mood. Thanks for checking on me."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Personal;
	}
	else if (bRain)
	{
		Pool = {
			FText::FromString(TEXT("I love the rain on the windows... it feels cozy in here.")),
			FText::FromString(TEXT("Rainy days are perfect for staying inside together.")),
			FText::FromString(TEXT("The apartment sounds so peaceful with the rain."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Weather;
	}
	else if (Savings < 150.f)
	{
		Pool = {
			FText::FromString(TEXT("I've been watching my budget closely lately...")),
			FText::FromString(TEXT("Maybe I should pick up an extra freelance task soon.")),
			FText::FromString(TEXT("I want to save up, but I still want the apartment to feel nice."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Work;
	}
	else if (Comfort > 70.f)
	{
		Pool = {
			FText::FromString(TEXT("The apartment feels so comfortable lately. I love how it's coming together.")),
			FText::FromString(TEXT("That new furniture really changed the mood in here.")),
			FText::FromString(TEXT("Home feels peaceful right now. I'm glad we upgraded a few things."))
		};
		Line.Topic = EApartmentLifeConversationTopic::HomeDecorating;
	}
	else if (Context.SpeakerPersonality.FitnessInterest > 0.5f && Hour >= 16 && Hour < 20)
	{
		Pool = {
			FText::FromString(TEXT("I did some yoga earlier — my balance is getting better.")),
			FText::FromString(TEXT("Stretching really helped after sitting at the desk.")),
			FText::FromString(TEXT("I want to try a harder pose next session."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Fitness;
	}
	else if (Context.SpeakerPersonality.CookingInterest > 0.4f)
	{
		Pool = {
			FText::FromString(TEXT("I was thinking about what to cook tonight... any ideas?")),
			FText::FromString(TEXT("The kitchen smells nice when something's simmering.")),
			FText::FromString(TEXT("Cooking here feels relaxing when the apartment is quiet."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Cooking;
	}
	else if (Hour >= 21 || Hour < 5)
	{
		Pool = {
			FText::FromString(TEXT("It's getting late... I should start winding down soon.")),
			FText::FromString(TEXT("Night feels calm. I like how quiet the apartment gets.")),
			FText::FromString(TEXT("Maybe I'll read a little before bed."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Personal;
	}
	else
	{
		Pool = {
			FText::FromString(TEXT("It's nice having you here with me.")),
			FText::FromString(TEXT("I was just relaxing. Want to talk for a bit?")),
			FText::FromString(TEXT("This apartment feels more like home every day.")),
			FText::FromString(TEXT("I like how peaceful it is here.")),
			FText::FromString(TEXT("Thanks for spending time with me today."))
		};
		Line.Topic = EApartmentLifeConversationTopic::Personal;
	}

	Line.SpeakerText = PickRandom(Pool);

	if (Stress > 60.f) Line.EmotionTag = FName(TEXT("emotion.stressed"));
	else if (Mood > 70.f) Line.EmotionTag = FName(TEXT("emotion.happy"));
	else if (Energy < 35.f) Line.EmotionTag = FName(TEXT("emotion.tired"));
	else Line.EmotionTag = FName(TEXT("emotion.relaxed"));

	Line.RelationshipImpact = 0.15f + Context.SpeakerPersonality.Empathy * 0.1f;
	return Line;
}

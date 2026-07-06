// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityFallbackLibrary.h"

int32 UApartmentLifeActivityFallbackLibrary::GetBuiltinDurationMinutes(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("sleep"))) return 480;
	if (Id.Contains(TEXT("nap"))) return 60;
	if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("hygiene"))) return 20;
	if (Id.Contains(TEXT("groom")) || Id.Contains(TEXT("mirror"))) return 15;
	if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("stretch"))) return 30;
	if (Id.Contains(TEXT("work")) || Id.Contains(TEXT("computer")) || Id.Contains(TEXT("study"))) return 120;
	if (Id.Contains(TEXT("cook")) || Id.Contains(TEXT("eat"))) return 45;
	if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("wardrobe"))) return 10;
	if (Id.Contains(TEXT("social")) || Id.Contains(TEXT("talk"))) return 20;
	if (Id.Contains(TEXT("relax")) || Id.Contains(TEXT("read")) || Id.Contains(TEXT("tv"))) return 30;
	if (Id.Contains(TEXT("breathe"))) return 10;
	if (Id.Contains(TEXT("laundry"))) return 30;
	return 15;
}

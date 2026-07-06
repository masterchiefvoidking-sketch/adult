// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeUiLibrary.h"
#include "ApartmentLifeUiOverlayGate.h"
#include "ApartmentLifeUiSubsystem.h"
#include "Engine/GameInstance.h"

bool UApartmentLifeUiLibrary::ShouldSuppressDebugOverlay(const UObject* WorldContextObject)
{
	if (FApartmentLifeUiOverlayGate::bSuppressDebugOverlays)
	{
		return true;
	}
	if (!WorldContextObject)
	{
		return false;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return false;
	}

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	const UApartmentLifeUiSubsystem* UiSubsystem = GameInstance->GetSubsystem<UApartmentLifeUiSubsystem>();
	return UiSubsystem && UiSubsystem->IsSlateUiActive();
}

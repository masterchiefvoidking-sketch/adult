// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifePlayerController.h"
#include "ApartmentLifeCameraPawn.h"

AApartmentLifePlayerController::AApartmentLifePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

AApartmentLifeCameraPawn* AApartmentLifePlayerController::GetCameraPawn() const
{
	return Cast<AApartmentLifeCameraPawn>(GetPawn());
}

void AApartmentLifePlayerController::SetFocusTarget(AActor* NewTarget)
{
	if (AApartmentLifeCameraPawn* CameraPawn = GetCameraPawn())
	{
		CameraPawn->SetFocusTarget(NewTarget);
	}
}

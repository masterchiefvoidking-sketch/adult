// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "AdultAnimeApartmentLifeGameModeBase.h"
#include "ApartmentLifeCameraPawn.h"
#include "ApartmentLifePlayerController.h" // ApartmentLifeCamera plugin
#include "ApartmentLifeGameState.h"

AAdultAnimeApartmentLifeGameModeBase::AAdultAnimeApartmentLifeGameModeBase()
{
	DefaultPawnClass = AApartmentLifeCameraPawn::StaticClass();
	PlayerControllerClass = AApartmentLifePlayerController::StaticClass();
	GameStateClass = AApartmentLifeGameState::StaticClass();
}

// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Global gate so feature UI controllers can skip on-screen debug overlays when Slate UI is active. */
struct APARTMENTLIFECORE_API FApartmentLifeUiOverlayGate
{
	static bool bSuppressDebugOverlays;
};

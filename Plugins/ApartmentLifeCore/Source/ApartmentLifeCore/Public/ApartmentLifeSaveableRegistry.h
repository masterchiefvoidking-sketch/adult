// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UObject;

/**
 * Runtime registry for UObject saveables that are not world actors (e.g. subsystems).
 * Subsystems register on Initialize and unregister on Deinitialize.
 */
class APARTMENTLIFECORE_API FApartmentLifeSaveableRegistry
{
public:
	static void Register(UObject* Saveable);
	static void Unregister(UObject* Saveable);
	static TArray<UObject*> GetRegistered();
};

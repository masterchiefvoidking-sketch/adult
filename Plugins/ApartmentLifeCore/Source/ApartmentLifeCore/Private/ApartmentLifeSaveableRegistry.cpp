// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSaveableRegistry.h"
#include "ApartmentLifeSaveable.h"

namespace
{
	TArray<TWeakObjectPtr<UObject>> GRegisteredSaveables;
}

void FApartmentLifeSaveableRegistry::Register(UObject* Saveable)
{
	if (!Saveable || !Saveable->Implements<UApartmentLifeSaveable>())
	{
		return;
	}

	GRegisteredSaveables.AddUnique(Saveable);
}

void FApartmentLifeSaveableRegistry::Unregister(UObject* Saveable)
{
	GRegisteredSaveables.RemoveAll([Saveable](const TWeakObjectPtr<UObject>& Entry)
	{
		return Entry.Get() == Saveable;
	});
}

TArray<UObject*> FApartmentLifeSaveableRegistry::GetRegistered()
{
	TArray<UObject*> Result;
	for (const TWeakObjectPtr<UObject>& Entry : GRegisteredSaveables)
	{
		if (UObject* Obj = Entry.Get())
		{
			Result.Add(Obj);
		}
	}
	return Result;
}

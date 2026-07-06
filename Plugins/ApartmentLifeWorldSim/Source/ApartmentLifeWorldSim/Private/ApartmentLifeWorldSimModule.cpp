// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "Modules/ModuleManager.h"

class FApartmentLifeWorldSimModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
};

void FApartmentLifeWorldSimModule::StartupModule()
{
}

IMPLEMENT_MODULE(FApartmentLifeWorldSimModule, ApartmentLifeWorldSim)

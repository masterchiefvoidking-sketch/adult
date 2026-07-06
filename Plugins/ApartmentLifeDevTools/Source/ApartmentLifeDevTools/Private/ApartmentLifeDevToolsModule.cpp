// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "Modules/ModuleManager.h"

class FApartmentLifeDevToolsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FApartmentLifeDevToolsModule, ApartmentLifeDevTools)

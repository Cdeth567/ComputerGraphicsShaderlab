#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "DebandPluginExtension.h"

class FDebandPluginModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedPtr<class FDebandPluginSceneViewExtension, ESPMode::ThreadSafe> ViewExtension;  // Renamed
};
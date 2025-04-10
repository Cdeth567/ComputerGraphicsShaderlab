#include "DebandPluginModule.h"
#include "DebandPluginLog.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDebandPluginModule"

void FDebandPluginModule::StartupModule()
{
    UE_LOG(LogDebandPlugin, Log, TEXT("Deband plugin startup")); 

    // Map shader directory
    FString PluginShaderDir = FPaths::Combine(
        IPluginManager::Get().FindPlugin(TEXT("DebandPlugin"))->GetBaseDir(), 
        TEXT("Shaders")
    );
    AddShaderSourceDirectoryMapping(TEXT("/Plugin/DebandPlugin"), PluginShaderDir);

    // Initialize extension
    ViewExtension = FSceneViewExtensions::NewExtension<FDebandPluginSceneViewExtension>();
}

void FDebandPluginModule::ShutdownModule()
{
    UE_LOG(LogDebandPlugin, Log, TEXT("Deband plugin shutdown")); 
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebandPluginModule, DebandPlugin);
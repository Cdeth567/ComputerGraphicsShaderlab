#include "DebandPluginModule.h"
#include "DebandPluginLog.h"

#include "Interfaces/IPluginManager.h"
#include "SceneViewExtension.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FDebandPluginModule"

void FDebandPluginModule::StartupModule()
{
    UE_LOG(LogDebandPlugin, Log, TEXT("Deband plugin startup")); 

    FString PluginShaderDir = FPaths::Combine(
        IPluginManager::Get().FindPlugin(TEXT("DebandPlugin"))->GetBaseDir(), 
        TEXT("Shaders")
    );
    AddShaderSourceDirectoryMapping(TEXT("/DebandPlugin"), PluginShaderDir);

    FCoreDelegates::OnPostEngineInit.AddLambda([this]() {
<<<<<<< HEAD
        ViewExtension = FSceneViewExtensions::NewExtension<FDebandPluginSceneViewExtension>();
=======
        ViewExtension = FSceneViewExtensions::NewExtension<FDebandPluginExtension>();
>>>>>>> de56a78071da40d76458a4a6976d279089684d25
    });
}

void FDebandPluginModule::ShutdownModule()
{
    UE_LOG(LogDebandPlugin, Log, TEXT("Deband plugin shutdown")); 
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebandPluginModule, DebandPlugin);

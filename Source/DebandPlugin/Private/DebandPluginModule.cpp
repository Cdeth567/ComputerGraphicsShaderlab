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
        ViewExtension = FSceneViewExtensions::NewExtension<FDebandPluginSceneViewExtension>();
    });
}

void FDebandPluginModule::ShutdownModule()
{
    UE_LOG(LogDebandPlugin, Log, TEXT("Deband plugin shutdown")); 
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDebandPluginModule, DebandPlugin);

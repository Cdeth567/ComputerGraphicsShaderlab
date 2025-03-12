#include "FullScreenPassModule.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"
#include "FullScreenPassLog.h"

#define LOCTEXT_NAMESPACE "FFullScreenPassModule"

void FFullScreenPassModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("FullScreenPass"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/FullScreenPass"), PluginShaderDir);
	UE_LOG(FullScreenPass, Log, TEXT("Module startup"));
}

void FFullScreenPassModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(FullScreenPass, Log, TEXT("Module shutdown"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFullScreenPassModule, FullScreenPass);

#include "DebandPluginExtension.h"
#include "DebandShaders.h"
#include "PostProcess/PostProcessInputs.h"

static TAutoConsoleVariable<int32> CVarDebandEnabled(
    TEXT("r.Deband.Enabled"),
    1,
    TEXT("Toggle debanding effect\n0: Disabled\n1: Enabled (default)"));

static TAutoConsoleVariable<float> CVarDebandSearchRadius(
    TEXT("r.Deband.SearchRadius"),
    0.5f,
    TEXT("Debanding search radius (default=0.5)"));

// ... Add other CVars for BitDepth, Mode, etc.

FDebandPluginSceneViewExtension::FDebandPluginSceneViewExtension(const FAutoRegister& AutoRegister) 
    : FSceneViewExtensionBase(AutoRegister) {}

void FDebandPluginSceneViewExtension::PrePostProcessPass_RenderThread(
    FRDGBuilder& GraphBuilder, 
    const FSceneView& View, 
    const FPostProcessingInputs& Inputs)
{
    if (CVarDebandEnabled->GetInt() == 0) return;

    // Get scene texture
    const FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, View.ViewRect);
    if (!SceneColor.IsValid()) return;

    DebandShader::DrawDebandPass(
        GraphBuilder,
        View,
        SceneColor.Texture,
        SceneColor.Texture,
        CVarDebandSearchRadius->GetFloat(),
        8.0f,  // Default bit depth
        2      // Deband mode (2=Deband)
    );
}
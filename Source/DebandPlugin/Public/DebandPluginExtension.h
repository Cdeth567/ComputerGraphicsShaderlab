#pragma once
#include "SceneViewExtension.h"

class DEBANDPLUGIN_API FDebandPluginSceneViewExtension : public FSceneViewExtensionBase
{
public:
    FDebandPluginSceneViewExtension(const FAutoRegister& AutoRegister);

    //~ Begin FSceneViewExtensionBase Interface
    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {};
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {};
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {};
    virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;
    //~ End FSceneViewExtensionBase Interface

private:
    // Add deband-specific parameters
    float SearchRadius = 0.5f;
    int32 BitDepth = 8;
    // ... other settings
};
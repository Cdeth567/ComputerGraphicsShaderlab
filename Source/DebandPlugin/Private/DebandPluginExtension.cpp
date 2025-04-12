#include "DebandPluginExtension.h"
#include "DebandShaders.h"
#include "FXRenderingUtils.h"
#include "PostProcess/PostProcessInputs.h"

static TAutoConsoleVariable<int32> CVarDebandEnabled(
    TEXT("r.Deband"),
    1,
    TEXT("Enable Debanding post-process\n")
    TEXT("0: Disabled\n")
    TEXT("1: Enabled (default)"));

static TAutoConsoleVariable<float> CVarDebandStrength(
    TEXT("r.Deband.Strength"),
    0.0025f,
    TEXT("Debanding effect intensity (0.001-0.01)"));

static TAutoConsoleVariable<int32> CVarDebandIterations(
    TEXT("r.Deband.Iterations"),
    3,
    TEXT("Number of deband iterations (1-5)"));

FDebandPluginSceneViewExtension::FDebandPluginSceneViewExtension(const FAutoRegister& AutoRegister) :
FSceneViewExtensionBase(AutoRegister)
{

}

void FDebandPluginSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
        if (CVarDebandEnabled->GetInt() == 0) return;

        Inputs.Validate();
        const FIntRect ViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);
        
        FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, ViewRect);
        if (!SceneColor.IsValid()) return;

        FRDGTextureDesc OutputDesc = SceneColor.Texture->Desc;
        OutputDesc.Format = PF_FloatRGBA;
        FRDGTexture* DebandedTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("DebandedTexture"));
        
        FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
        TShaderMapRef<FDebandVS> VertexShader(ShaderMap);
        TShaderMapRef<FDebandPS> PixelShader(ShaderMap);

        FDebandPS::FParameters* Parameters = GraphBuilder.AllocParameters<FDebandPS::FParameters>();
        Parameters->InputTexture = SceneColor.Texture;
        Parameters->InputSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
        Parameters->Strength = CVarDebandStrength->GetFloat();
        Parameters->Iterations = CVarDebandIterations->GetInt();
        Parameters->RenderTargets[0] = FRenderTargetBinding(DebandedTexture, ERenderTargetLoadAction::EClear);

        AddDrawScreenPass(
            GraphBuilder,
            RDG_EVENT_NAME("DebandShader"),
            View,
            FScreenPassTextureViewport(SceneColor),
            FScreenPassTextureViewport(DebandedTexture),
            VertexShader,
            PixelShader,
            Parameters
        );

        AddCopyTexturePass(GraphBuilder, DebandedTexture, SceneColor.Texture);
};
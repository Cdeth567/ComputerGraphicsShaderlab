#include "DebandPluginExtension.h"
#include "DebandShaders.h"
#include "FXRenderingUtils.h"
#include "PostProcess/PostProcessInputs.h"

static TAutoConsoleVariable<float> SearchRadius(
    TEXT("r.Deband.SearchRadius"),
    0.5f,
    TEXT("Debanding Search Radius"));

static TAutoConsoleVariable<int> BitDepth(
    TEXT("r.Deband.BitDepth"),
    8,
    TEXT("Bit depth of data to be debanded"));

static TAutoConsoleVariable<bool> AutomateBitDepth(
    TEXT("r.Deband.AutomateBitDepth"),
    true,
    TEXT("Automatic bit depth detection"));

static TAutoConsoleVariable<int> DebandMode(
    TEXT("r.Deband.DebandMode"),
    2,
    TEXT("Dither mode"));

static TAutoConsoleVariable<bool> SkyOnly(
    TEXT("r.Deband.SkyOnly"),
    false,
    TEXT("Apply to sky only"));

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

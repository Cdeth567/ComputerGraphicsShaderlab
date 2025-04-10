#include "DebandPluginExtension.h"
#include "DebandShaders.h"
#include "FXRenderingUtils.h"
#include "PostProcess/PostProcessInputs.h"

// Console variables для управления дебандингом
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


class FDebandSceneViewExtension : public FSceneViewExtensionBase
{
public:
    FDebandSceneViewExtension(const FAutoRegister& AutoRegister)
        : FSceneViewExtensionBase(AutoRegister) {};

    virtual void PrePostProcessPass_RenderThread(
        FRDGBuilder& GraphBuilder,
        const FSceneView& View,
        const FPostProcessingInputs& Inputs) override
    {
        if (CVarDebandEnabled->GetInt() == 0) return;

        Inputs.Validate();
        const FIntRect ViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);
        
        // Получаем входную текстуру
        FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, ViewRect);
        if (!SceneColor.IsValid()) return;

        // Создаем выходной Render Target
        FRDGTextureDesc OutputDesc = SceneColor.Texture->Desc;
        OutputDesc.Format = PF_FloatRGBA; // Для работы с HDR
        FRDGTexture* DebandedTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("DebandedTexture"));
        
        // Настройка параметров шейдера
        FGlobalShaderMap* ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
        TShaderMapRef<FDebandVS> VertexShader(ShaderMap);
        TShaderMapRef<FDebandPS> PixelShader(ShaderMap);

        FDebandPS::FParameters* Parameters = GraphBuilder.AllocParameters<FDebandPS::FParameters>();
        Parameters->InputTexture = SceneColor.Texture;
        Parameters->InputSampler = TStaticSamplerState<SF_Bilinear>::GetRHI();
        Parameters->Strength = CVarDebandStrength->GetFloat();
        Parameters->Iterations = CVarDebandIterations->GetInt();
        Parameters->RenderTargets[0] = FRenderTargetBinding(DebandedTexture, ERenderTargetLoadAction::EClear);

        // Рендеринг эффекта
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

        // Копируем результат обратно в основную текстуру
        AddCopyTexturePass(GraphBuilder, DebandedTexture, SceneColor.Texture);
    }
};

// Файл DebandShaders.usf
// #include "/Plugins/DebandPlugin/Shaders/Private/Deband.usf"

// Texture2D InputTexture;
// SamplerState InputSampler;

// float Strength;
// int Iterations;

// float4 FDebandPS(FDebandVSOutput In) : SV_Target0
// {
//     float2 UV = In.TextureUV;
//     float4 color = InputTexture.SampleLevel(InputSampler, UV, 0);
    
//     // Простейший алгоритм дебандинга
//     for(int i = 0; i < Iterations; i++)
//     {
//         float2 offset = float2(Strength * (i + 1), 0);
//         color += InputTexture.SampleLevel(InputSampler, UV + offset, 0);
//         color += InputTexture.SampleLevel(InputSampler, UV - offset, 0);
//     }
//     return color / (Iterations * 2 + 1);
// }
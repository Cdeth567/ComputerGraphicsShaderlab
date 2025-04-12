#pragma once
#include "CoreMinimal.h"
#include "ScreenPass.h"
#include "SceneTexturesConfig.h"

// Vertex Shader
class FDebandVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDebandVS);
    
    FDebandVS() = default;
    FDebandVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) 
        : FGlobalShader(Initializer) {}
};

// Pixel Shader
class FDebandPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FDebandPS);
    SHADER_USE_PARAMETER_STRUCT(FDebandPS, FGlobalShader);

public:
    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)
        SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
        SHADER_PARAMETER(float, Strength)
        SHADER_PARAMETER(int32, Iterations)
        RENDER_TARGET_BINDING_SLOTS()
        SHADER_PARAMETER(float, SearchRadius)
        SHADER_PARAMETER(float, BitDepth)
        SHADER_PARAMETER(int, AutomateBitDepth)
        SHADER_PARAMETER(int, DebandMode)
        SHADER_PARAMETER(int, SkyOnly)
    END_SHADER_PARAMETER_STRUCT()
};
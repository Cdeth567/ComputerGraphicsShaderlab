#pragma once
#include "CoreMinimal.h"
#include "ScreenPass.h"
#include "SceneTexturesConfig.h"

// Renamed vertex shader
class FDebandVS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FDebandVS);
    FDebandVS() = default;
    FDebandVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) {}
};

class FDebandPS : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FDebandPS);
    SHADER_USE_PARAMETER_STRUCT(FDebandPS, FGlobalShader);

    BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
        RENDER_TARGET_BINDING_SLOTS()
        SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
        SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneTextureUniformParameters, SceneTexturesStruct)
        
        // Deband-specific parameters
        SHADER_PARAMETER(float, SearchRadius)
        SHADER_PARAMETER(float, BitDepth)
        SHADER_PARAMETER(int32, DebandMode)
        SHADER_PARAMETER(int32, AutomateBitDepth)
        SHADER_PARAMETER(int32, SkyOnly)
    END_SHADER_PARAMETER_STRUCT()

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }
};
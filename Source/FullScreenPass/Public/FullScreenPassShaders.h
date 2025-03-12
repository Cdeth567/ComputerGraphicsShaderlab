#pragma once
#include "CoreMinimal.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "MaterialShader.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "RHI.h"
#include "RHIStaticStates.h"
#include "RenderGraphResources.h"
#include "RenderGraphResources.h"
#include "RenderResource.h"
#include "SceneManagement.h"
#include "ShaderParameterUtils.h"
#include "UObject/ConstructorHelpers.h"

// FScreenPassTextureViewportParameters and FScreenPassTextureInput
#include "ScreenPass.h"
#include "SceneTexturesConfig.h"

#include "FullScreenPassLog.h"

// The vertex shader used by DrawScreenPass to draw a rectangle.
class FFullScreenPassVS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FFullScreenPassVS);

	FFullScreenPassVS() = default;
	FFullScreenPassVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) {}
};

// A simple shader that outputs (0.,0.,0.,0.)
class FFullScreenPassPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FFullScreenPassPS);
	SHADER_USE_PARAMETER_STRUCT(FFullScreenPassPS, FGlobalShader);

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		UE_LOG(FullScreenPass, Log, TEXT("Set env"));
		OutEnvironment.SetDefine(TEXT("CLEAR_PS"), 1);
	}

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()
};
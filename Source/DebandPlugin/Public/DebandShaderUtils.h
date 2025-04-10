#pragma once
#include "DebandShaders.h"

namespace DebandShader
{
    static void DrawDebandPass(
        FRDGBuilder& GraphBuilder,
        const FViewInfo& View,
        FRDGTextureRef InputTexture,
        FRDGTextureRef OutputTexture,
        float SearchRadius = 0.5f,
        float BitDepth = 8.0f,
        int32 DebandMode = 2,
        bool bAutomateBitDepth = true,
        bool bSkyOnly = false)
    {
        FDebandPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FDebandPS::FParameters>();
        PassParameters->View = View.ViewUniformBuffer;
        PassParameters->SceneTexturesStruct = CreateSceneTextureUniformBuffer(GraphBuilder, View.FeatureLevel);
        PassParameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);
        
        // Set deband parameters
        PassParameters->SearchRadius = SearchRadius;
        PassParameters->BitDepth = BitDepth;
        PassParameters->DebandMode = DebandMode;
        PassParameters->AutomateBitDepth = bAutomateBitDepth ? 1 : 0;
        PassParameters->SkyOnly = bSkyOnly ? 1 : 0;

        TShaderMapRef<FDebandVS> VertexShader(View.ShaderMap);
        TShaderMapRef<FDebandPS> PixelShader(View.ShaderMap);

        ClearUnusedGraphResources(PixelShader, PassParameters);

        GraphBuilder.AddPass(
            RDG_EVENT_NAME("DebandPass"),
            PassParameters,
            ERDGPassFlags::Raster,
            [VertexShader, PixelShader, PassParameters, &View](FRHICommandList& RHICmdList)
            {
                FGraphicsPipelineStateInitializer GraphicsPSOInit;
                RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
                GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
                GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
                GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
                GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI;
                GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
                GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
                GraphicsPSOInit.PrimitiveType = PT_TriangleList;
                
                SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0);
                SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), *PassParameters);
                
                DrawRectangle(
                    RHICmdList,
                    0, 0,
                    View.ViewRect.Width(), View.ViewRect.Height(),
                    0, 0,
                    View.ViewRect.Width(), View.ViewRect.Height(),
                    View.ViewRect.Size(),
                    View.ViewRect.Size(),
                    VertexShader,
                    EDRF_UseTriangleOptimization);
            });
    }
}
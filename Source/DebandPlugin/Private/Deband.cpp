#include "DebandShaders.h"
#include "ShaderParameterStruct.h"

IMPLEMENT_GLOBAL_SHADER(FDebandVS, "/DebandPlugin/Private/Deband.usf", "MainVS", SF_Vertex);
IMPLEMENT_GLOBAL_SHADER(FDebandPS, "/DebandPlugin/Private/Deband.usf", "MainPS", SF_Pixel);

#ifndef _TILEMAP
#define _TILEMAP

#include "func.fx"
#include "register.fx"

// ===============
// UITileMap Shader
// ===============
// mesh                 : UiTileMesh
// Rasterizer           : CULL_NONE 
// BlendState           : ALPHABLEND
// DepthStencilState    : NO_WRITE

struct VS_IN
{
    float3 vPos         : POSITION;
    float2 vUV          : TEXCOORD;
};


struct VS_OUT
{
    float4 vPos         : SV_Position;
    float2 vUV          : TEXCOORD;
};

VS_OUT VS_UiTileMap(VS_IN _in)
{
    VS_OUT output   = (VS_OUT)0.f;
    output.vPos     = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV      = _in.vUV;
    return output;
}

float4 PS_UiTileMap(VS_OUT _in) : SV_Target
{
    float4 color1 = float4(0.f, 0.8f, 0.f, 0.5f);
    float4 color2 = float4(0.f, 0.2f, 0.f, 0.5f);
    float4 alphaValue;
    float4 blendColor;

    if (_in.vUV.x == -1
        && _in.vUV.y == -1)
       discard;
    alphaValue = g_tex_0.Sample(g_sam_1, _in.vUV);
    blendColor = (alphaValue * color1) + ((1.0 - alphaValue) * color2);
    blendColor = saturate(blendColor);
   return blendColor;

}

#endif

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
    float4 vColor       : COLOR;
};


struct VS_OUT
{
    float4 vPos         : SV_Position;
    float2 vUV          : TEXCOORD;
    float4 vColor       : COLOR;
};

VS_OUT VS_UiTileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vColor = _in.vColor;
    return output;
}

float4 PS_UiTileMap(VS_OUT _in) : SV_Target
{
    float4 color1;
    float4 color2;
    float4 alphaValue;
    float4 blendColor;

    if (0 == g_int_0)
        discard;

    if (g_TileBuffer[_in.vColor.x].iInfo == 0)
    {
        discard;
    }
    else if (g_TileBuffer[_in.vColor.x].iInfo == 1)
    {
        color1 = float4(0.0f, 0.7f, 0.0f, 0.5f);
        color2 = float4(0.0f, 0.7f, 0.0f, 0.5f);
    }
    else if (g_TileBuffer[_in.vColor.x].iInfo == 4)
    {
        color1 = float4(0.0f, 0.7f, 0.0f, 0.5f);
        color2 = float4(0.0f, 0.7f, 0.0f, 0.5f);
    }
    else if (g_TileBuffer[_in.vColor.x].iInfo == 2)
    {
        color1 = float4(0.0f, 0.5f, 0.0f, 1.f);
        color2 = float4(0.0f, 0.5f, 0.0f, 1.f);
    }
    else if (g_TileBuffer[_in.vColor.x].iInfo == 3)
    {
        color1 = float4(0.5f, 0.0f, 0.0f, 0.5f);
        color2 = float4(0.5f, 0.0f, 0.0f, 0.5f);
    }

    alphaValue = g_tex_0.Sample(g_sam_1, _in.vUV);
    blendColor = (alphaValue * color1) + ((1.0 - alphaValue) * color2);
    blendColor = saturate(blendColor);
   return blendColor;

}

#endif

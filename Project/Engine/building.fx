#ifndef _OBJECTRENDERER
#define _OBJECTRENDERER

#include "register.fx"
#include "func.fx"

// ======================
// Object Render Shader
// RS_TYPE : CULL_NONE
// BS_TYPE : NONALPAHBLEN
// DS_TYPE : LESS
// DOMAIN : 

#define Atlas         g_int_0
#define UVLeftTop     g_vec2_0
#define UVSlice       g_vec2_1

struct VS_IN
{
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
};

struct VS_OUT
{
    float4 vPos      : SV_Position;
    float2 vUV       : TEXCOORD;
    float4 vWorldPos : POSITION;
};


VS_OUT VS_BuildRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);

    return output;
}

float4 PS_BuildRender(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_iAnim2DUse)
    {
        float2 vDiff = (g_vFullSize - g_vSlice) / 2.f;
        float2 vUV = (g_vLeftTop - vDiff - g_vOffset) + (g_vFullSize * _in.vUV);

        if (vUV.x < g_vLeftTop.x
            || g_vLeftTop.x + g_vSlice.x < vUV.x
            || vUV.y < g_vLeftTop.y
            || g_vLeftTop.y + g_vSlice.y < vUV.y)
        {
            discard;
        }

        vOutColor = g_Atals.Sample(g_sam_1, vUV);
        
        if(0 == g_int_0)
            vOutColor.g = vOutColor.g * 3.f;
        else if(1 == g_int_0)
            vOutColor.r = vOutColor.r * 3.f;
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);

        if (0 == g_int_0)
            vOutColor.g = vOutColor.g * 3.f;
        else if (1 == g_int_0)
            vOutColor.r = vOutColor.r * 3.f;
    }

    if (0.f == vOutColor.a)
        discard;
    // ±¤¿ø Ã³¸®
 /*   tLightColor color = (tLightColor)0.f;

    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }*/

    // vOutColor *= color.vDiff;

     return vOutColor;

}
#endif
#ifndef _INTERFACERENDERER
#define _INTERFACERENDERER

#include "register.fx"
#include "func.fx"

#define UVLeftTop       g_vec2_0
#define UVSlice         g_vec2_1

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


VS_OUT VS_Opaque(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    return output;
}

float4 PS_Opaque(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);

    float2 vUV = (UVLeftTop + UVSlice * _in.vUV);

    vOutColor = g_tex_0.Sample(g_sam_0, vUV);


    if (0.f == vOutColor.a)
        discard;

     return vOutColor;

}
#endif
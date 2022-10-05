#ifndef _TEST
#define _TEST


// 상수버퍼 값 받아오기
cbuffer TRANSFORM : register(b0)
{
    float4 g_vPos;
    matrix g_matWorld;
    matrix g_matWV;
    matrix g_matWVP;
};

cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    matrix g_mat_0;
    matrix g_mat_1;
    matrix g_mat_2;
    matrix g_mat_3;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);

SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);
// HLSL 로 VertexShader 작성하기
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};


VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    output.vPos = float4(_in.vPos * 0.5f + g_vPos.xyz, 1.f);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;

    return output;
}



float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = _in.vColor;

    if (g_int_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    }


    return vOutColor;
}



#endif
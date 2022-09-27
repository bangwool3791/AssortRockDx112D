#ifndef _TEST
#define _TEST


// 상수버퍼 값 받아오기
cbuffer POSITION : register(b0)
{
    float4 g_vPos;
};



// HLSL 로 VertexShader 작성하기
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
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

    output.vPos = float4(_in.vPos + g_vPos.xyz, 1.f);
    output.vColor = _in.vColor;

    return output;
}



float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = _in.vColor;

    _in.vUV;


    return vOutColor;
}



#endif
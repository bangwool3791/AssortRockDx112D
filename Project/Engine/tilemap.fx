#ifndef _TILEMAP
#define _TILEMAP

#include "func.fx"
#include "register.fx"

// ===============
// TileMap Shader
#define AtlasTex g_tex_0
#define LeftTop g_vec2_0
#define Slice  g_vec2_1
#define TileCount g_vec2_2;
#define CameraPos g_vec4_0;
// ===============


struct VS_IN
{
    float3 vPos       : POSITION;
    float2 vUV        : TEXCOORD;
    uint   iInstance  : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float2 vUV          : TEXCOORD;
    uint   iInstance    : SV_InstanceID;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;
    output.vPosition     = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vPosition     += float4(TileBuffer[_in.iInstance].vPos, 0.f);
    output.vPosition.xyz -= g_vec4_0.xyz;
    output.vPosition     = mul(output.vPosition, g_matView * g_matProj);
    output.iInstance     = _in.iInstance;
    output.vUV = _in.vUV;
    /*
        UV 좌표를 확대한다.
        0,0              TileCount,0



        0, TileCount     TileCount, TileCount
    */

    //output.vUV = _in.vUV * TileCount;
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    /*
    UV 좌표를 확대한다.
    0,0 1,0 2,0  ...       TileCount-1,0  TileCount,0
    0,1                                   .
    0,2                                   .
    .                                     .
    .                                     .
    0, TileCount-1
    0, TileCount                          TileCount, TileCount
    */

    /*
    * TileCount.x * y + x
    */
    //int bufferIdx = g_vec2_2.x * floor(_in.vUV.y) + _in.vUV.x;

    /*
    * Texture 시작 위치부터, Slice 크기만큼, UV 좌표(0~1)까지 값에 비례하도록 UV좌표 설정
    */
    //float2 vUV = TileBuffer[bufferIdx].vLeftTop + (TileBuffer[bufferIdx].vSlice * frac(_in.vUV));

    tLightColor color = (tLightColor)0.f;

    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vPosition, i);
    }

    if (TileBuffer[_in.iInstance].ibyOption == 0)
    {
        vOutColor = AtlasTex.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 1)
    {
        vOutColor = g_tex_1.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 2)
    {
        vOutColor = g_tex_2.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 3)
    {
        vOutColor = g_tex_3.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 4)
    {
        vOutColor = g_tex_4.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 5)
    {
        vOutColor = g_tex_5.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 6)
    {
        vOutColor = g_tex_6.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 7)
    {
        vOutColor = g_tex_7.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 8)
    {
        vOutColor = g_tex_8.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 9)
    {
        vOutColor = g_tex_9.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 10)
    {
        vOutColor = g_tex_10.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 11)
    {
        vOutColor = g_tex_11.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 12)
    {
        vOutColor = g_tex_12.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 13)
    {
        vOutColor = g_tex_13.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 14)
    {
        vOutColor = g_tex_14.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 15)
    {
        vOutColor = g_tex_15.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 16)
    {
        vOutColor = g_tex_16.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 17)
    {
        vOutColor = g_tex_17.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 18)
    {
        vOutColor = g_tex_18.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 19)
    {
        vOutColor = g_tex_19.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 20)
    {
        vOutColor = g_tex_20.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 21)
    {
        vOutColor = g_tex_21.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 22)
    {
        vOutColor = g_tex_22.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 23)
    {
        vOutColor = g_tex_23.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 24)
    {
        vOutColor = g_tex_24.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 25)
    {
        vOutColor = g_tex_25.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 26)
    {
        vOutColor = g_tex_26.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 27)
    {
        vOutColor = g_tex_27.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 28)
    {
        vOutColor = g_tex_28.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 29)
    {
        vOutColor = g_tex_29.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 30)
    {
        vOutColor = g_tex_30.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 31)
    {
        vOutColor = g_tex_31.Sample(g_sam_1, _in.vUV);
    }
    else if (TileBuffer[_in.iInstance].ibyOption == 32)
    {
        vOutColor = g_tex_32.Sample(g_sam_1, _in.vUV);
    }

    vOutColor *= color.vDiff;

    return vOutColor;
}


#endif

#pragma once
#ifndef _TILEMAP
#define _TILEMAP

#include "register.fx"

// ===============
// TileMap Shader
#define AtlasTex g_tex_0
#define LeftTop g_vec2_0
#define Slice  g_vec2_1
#define TileCount g_vec2_2;
// ===============


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    /*
        UV 좌표를 확대한다.
        0,0              TileCount,0



        0, TileCount     TileCount, TileCount
    */

    output.vUV = _in.vUV * TileCount;

    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
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
    int bufferIdx = g_vec2_2.x * floor(_in.vUV.y) + _in.vUV.x;

    /*
    * Texture 시작 위치부터, Slice 크기만큼, UV 좌표(0~1)까지 값에 비례하도록 UV좌표 설정
    */
    float2 vUV = TileBuffer[bufferIdx].vLeftTop + (TileBuffer[bufferIdx].vSlice * frac(_in.vUV));

    return AtlasTex.Sample(g_sam_1, vUV);
}


#endif

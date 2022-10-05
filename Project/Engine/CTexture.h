#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // System Mem 에 로딩된 이미지 픽셀데이터 관리
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU Mem 에 옮긴 데이터 관리하는 객체
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // Tex2D 를 전달할 때 사용
    D3D11_TEXTURE2D_DESC                m_Desc;     // Tex2D Description 구조체

public:
    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipelineStage);


private:
    // 파일로 부터 로딩
    virtual int Load(const wstring& _strFilePath);


public:
    CTexture();
    ~CTexture();
};


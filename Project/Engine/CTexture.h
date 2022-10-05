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
    ScratchImage                        m_Image;    // System Mem �� �ε��� �̹��� �ȼ������� ����
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU Mem �� �ű� ������ �����ϴ� ��ü
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // Tex2D �� ������ �� ���
    D3D11_TEXTURE2D_DESC                m_Desc;     // Tex2D Description ����ü

public:
    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipelineStage);


private:
    // ���Ϸ� ���� �ε�
    virtual int Load(const wstring& _strFilePath);


public:
    CTexture();
    ~CTexture();
};


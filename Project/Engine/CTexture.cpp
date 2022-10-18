#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture()
    : CRes(RES_TYPE::TEXTURE)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
    int a;
}

int CTexture::Load(const wstring& _strFilePath)
{
    HRESULT hr = E_FAIL;

    wchar_t szExt[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

    wstring strExt = szExt;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (strExt == L".tga" || strExt == L".TGA")
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else // WIC (png, jpg, jpeg, bmp )
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
        return hr;


    // sysmem -> GPU
    hr = CreateShaderResourceView(DEVICE
        , m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_SRV.GetAddressOf());

    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

    return hr;
}

void CTexture::UpdateData(UINT _iRegisterNum, UINT _iPipelineStage)
{
    if ((UINT)PIPELINE_STAGE::VS & _iPipelineStage)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::HS & _iPipelineStage)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::DS & _iPipelineStage)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::GS & _iPipelineStage)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::PS & _iPipelineStage)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}


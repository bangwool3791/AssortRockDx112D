#include "pch.h"

#include "CMaterial.h"
#include "CDevice.h"
#include "CGraphicsShader.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(RES_TYPE::MATERIAL)
	, m_tConst{}
{
	for (size_t i{ 0 }; i < TEX_END; ++i)
	{
		m_arrTex[i] = nullptr;
	}
}

CMaterial::~CMaterial()
{

}

void CMaterial::SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData)
{
	switch (_eScalarType)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		m_tConst.iArr[(UINT)_eScalarType] = *(int*)_pData;
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_tConst.fArr[(UINT)_eScalarType - FLOAT_0] = *(float*)_pData;
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_tConst.v2Arr[(UINT)_eScalarType - VEC2_0] = *(Vec2*)_pData;
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_tConst.v4Arr[(UINT)_eScalarType - VEC4_0] = *(Vec4*)_pData;
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_tConst.matArr[(UINT)_eScalarType - MAT_0] = *(Matrix*)_pData;
		break;
	}
}

void CMaterial::UpdateData()
{
	for (size_t i{ 0 }; i < TEX_END; ++i)
	{
		if(nullptr != m_arrTex[i])
			m_arrTex[i]->UpdateData(i, ALL_STAGE);
	}

	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_tConst);
	pCB->UpdateData(PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	m_pShader->UpdateDate();
}

void CMaterial::SetTexParam(TEX_PARAM _eTex, Ptr<CTexture> _pTex)
{
	m_arrTex[_eTex] = _pTex;
}

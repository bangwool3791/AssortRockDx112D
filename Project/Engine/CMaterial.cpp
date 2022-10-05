#include "pch.h"

#include "CMaterial.h"
#include "CDevice.h"
#include "CGraphicsShader.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(RES_TYPE::MATERIAL)
	, m_tConst{}
{
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
		m_tConst.fArr[(UINT)_eScalarType] = *(float*)_pData;
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_tConst.v2Arr[(UINT)_eScalarType] = *(Vec2*)_pData;
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_tConst.v4Arr[(UINT)_eScalarType] = *(Vec4*)_pData;
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_tConst.matArr[(UINT)_eScalarType] = *(Matrix*)_pData;
		break;
	}
}

void CMaterial::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_tConst);
	pCB->UpdateData(PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	m_pShader->UpdateDate();
}


#include "pch.h"
#include "MaterialUI.h"

#include <Engine\struct.h>
#include <Engine\CShader.h>
#include <Engine\CMaterial.h>
#include <Engine\CResMgr.h>

#include "ParamUI.h"

MaterialUI::MaterialUI()
	: ResUI("Material##UI", RES_TYPE::MATERIAL)
	, m_eSelectTexParam(TEX_PARAM::TEX_END)
{
}

MaterialUI::~MaterialUI()
{

}

void MaterialUI::update()
{
	ResUI::update();
}

void MaterialUI::render_update()
{
	ResUI::render_update();
	CMaterial* pMtrl = (CMaterial*)GetTarget().Get();

	string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	string strRelativePath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

	ImGui::Text("Key           ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	string strShaderKey;
	
	if (nullptr != pMtrl->GetShader())
	{
		strShaderKey = string(pMtrl->GetShader()->GetKey().begin(), pMtrl->GetShader()->GetKey().end());
	}

	ImGui::Text("GraphicsShader");
	ImGui::SameLine();
	ImGui::InputText("##ShaderName", (char*)strShaderKey.data(), strShaderKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::Text("Shader Parameter");

	const vector<tScalarParam> vecScalar = pMtrl->GetShader()->GetScalarParam();

	for (size_t i{}; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int iData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &iData);
			ParamUI::Param_Int(vecScalar[i].strName, &iData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &iData);
		}
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float fData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
			ParamUI::Param_Float(vecScalar[i].strName, &fData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
		}
			break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec2(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
			break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec4(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
			break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{

		}
			break;
		}
	}


	const vector<tTextureParam> vecTex = pMtrl->GetShader()->GetTextureParam();
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);
		if (ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MaterialUI::SetTexture))
		{
			//몇번째 Texture를 변경 할 것인지
			m_eSelectTexParam = vecTex[i].eParam;
		}
		else
		{
			//텍스쳐가 세팅 되어있으면 Texture를 Set
			pMtrl->SetTexParam(vecTex[i].eParam, pTex);
		}
	}
}

//List에서 선택 된 Texture를 Load
//클릭 된 머터리얼의 주소에, 선택 된 Texture 인덱스의 Texture를 변경한다.
void MaterialUI::SetTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(nullptr != pTex);

	CMaterial* pMtrl = ((CMaterial*)GetTarget().Get());
	pMtrl->SetTexParam(m_eSelectTexParam, pTex);
}

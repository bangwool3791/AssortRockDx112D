#include "pch.h"
#include "MaterialUI.h"
#include "ParamUI.h"
#include "ListUI.h"

#include <Engine\struct.h>
#include <Engine\CShader.h>
#include <Engine\CMaterial.h>
#include <Engine\CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine\CEventMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

#include "CImGuiMgr.h"

MaterialUI::MaterialUI()
	: ResUI("Material##UI", RES_TYPE::MATERIAL)
	, m_eSelectTexParam(TEX_PARAM::TEX_END)
	, m_strRes{}
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

	if (m_strRes != strKey)
		m_strRes = strKey;

	string strRelativePath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

	ImGui::Text("Key           ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	if (ImGui::InputText("##MtrlKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (!CResMgr::GetInst()->DeleteRes((RES_TYPE)pMtrl->GetResType(), pMtrl->GetKey()))
		{
			MessageBox(nullptr, L"리소스 삭제 실패", L"에러", MB_OK);
		}
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += pMtrl->GetRelativePath();

		string strPath(strFilePath.begin(), strFilePath.end());
		
		if (remove(strPath.c_str()))
			cout << strPath << " " << "파일 삭제" << endl;

		for (size_t i{}; i < MAX_LAYER; ++i)
		{
			const vector<CGameObject*>& vecObjects = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetObjects();

			for (size_t i{}; i < vecObjects.size(); ++i)
			{
				wstring wstrPath = vecObjects[i]->MeshRender()->GetSharedMaterial()->GetRelativePath();
				if (strFilePath == wstrPath)
				{
					vecObjects[i]->MeshRender()->GetSharedMaterial()->SetRelativePath(wstring(strKey.begin(), strKey.end()));
					break;
				}
			}
		}

		pMtrl->SwapFile(L"\\material\\" + wstring(strKey.begin(), strKey.end()) + L".mtrl");
		pMtrl->SetName(wstring(strKey.begin(), strKey.end()));
		// 삭제 가능
		tEvent evn = {};
		evn.eType = EVENT_TYPE::EDIT_RES;
		evn.wParam = (DWORD_PTR)pMtrl->GetResType();
		evn.lParam = (DWORD_PTR)(pMtrl);
		CEventMgr::GetInst()->AddEvent(evn);

		MessageBox(nullptr, L"원본 리소스 삭제됨", L"리소스 변경 확인", MB_OK);
	}

	ImGui::Text("Path          ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlPath", (char*)strRelativePath.data(), strRelativePath.length(), ImGuiInputTextFlags_ReadOnly);

	string strShaderKey;
	
	if (nullptr != pMtrl->GetShader())
	{
		strShaderKey = string(pMtrl->GetShader()->GetKey().begin(), pMtrl->GetShader()->GetKey().end());
	}

	ImGui::Text("GraphicsShader");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputText("##ShaderName", (char*)strShaderKey.data(), strShaderKey.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	//쉐이더 변경 및 선택
	if (ImGui::Button("##ShaderBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::GRAPHICS_SHADER);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MaterialUI::SetShader);

		pListUI->Open();
	}
	ImGui::NewLine();
	ImGui::Text("Shader Parameter");

	if (nullptr == pMtrl->GetShader())
		return;

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

void MaterialUI::SetShader(DWORD_PTR _strShaderKey)
{
	string strKey = (char*)_strShaderKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(wstrKey);
	assert(nullptr != pShader);

	((CMaterial*)GetTarget().Get())->SetShader(pShader);
}
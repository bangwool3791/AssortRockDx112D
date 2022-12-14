#include "CLevelSaveLoad.h"

#include <Engine\CLevel.h>

#include <Engine\CComponent.h>
#include <Engine\CGameObject.h>

CSaveLoadMgr::CSaveLoadMgr()
{

}

CSaveLoadMgr::~CSaveLoadMgr()
{

}

void CSaveLoadMgr::SaveLevel(CLevel* _Level, wstring _strRelativePath)
{
	//입력 레벨이 nullptr이면 assert
	assert(_Level);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	//레벨 이름 저장
	_Level->SaveToFile(pFile);

	for (UINT i{}; i < MAX_LAYER; ++i)
	{
		//레이어 이름 저장
		CLayer* pLayer = _Level->GetLayer(i);
		pLayer->SaveToFile(pFile);

		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObjects();
		for (size_t j{}; j < vecParentObj.size(); ++j)
		{
			SaveGameObject(vecParentObj[j], pFile);
		}
	}
	fclose(pFile);
}

void CSaveLoadMgr::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	//오브젝트 이름 저장
	_Object->SaveToFile(_File);

	for (UINT i{}; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr != pComponent)
		{
			pComponent->SaveToFile(_File);
		}
	}
}


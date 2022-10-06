#pragma once

#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*>					m_arrRes[(UINT)RES_TYPE::END];
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayoutInfo;
	UINT								m_iLayoutOffset;

public:
	template<typename T>
	inline void AddRes(const wstring& _strKey, Ptr<T> _pRes);
	template<typename T>
	Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);
	template<typename T>
	Ptr<T> FindRes(const wstring& _strKey);

public :
	void init();

	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutInfo() { return m_vecLayoutInfo; }

private:
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultGraphicsShader();
	void CreateDefaultMaterial();
	void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName);
};

template<typename T>
RES_TYPE GetType()
{
	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		return RES_TYPE::MESH;
	if (typeid(T).hash_code() == typeid(CGraphicsShader).hash_code())
		return RES_TYPE::GRAPHICS_SHADER;
	if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		return RES_TYPE::MATERIAL;
	if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		return RES_TYPE::TEXTURE;
	else
		return RES_TYPE::END;	
}

template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T> _pRes)
{
	RES_TYPE eResType = ::GetType<T>();
	
	CRes* pRes = FindRes<T>(_strKey).Get();

	assert(!pRes);
	
	m_arrRes[(UINT)eResType].insert(make_pair(_strKey, _pRes.Get()));
}

template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
	auto iter = m_arrRes[(UINT)::GetType<T>()].find(_strKey);

	if (iter == m_arrRes[(UINT)::GetType<T>()].end())
		return nullptr;

	return (T*)iter->second;
}

template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
	//1. 타입을 읽어온다.
	//2. 경로를 완성한다.
	//3. 리소스 객체를 완성한다.
	RES_TYPE eResType = GetType<T>();

	CRes* pResource = FindRes<T>(_strKey).Get();

	if (nullptr != pResource)
		return (T*)pResource;
	
	pResource = new T;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath +=_strRelativePath;

	if (FAILED(pResource->Load(strFilePath)))
	{
		MessageBox(nullptr, strFilePath.c_str(), L"리소스 로딩 실패", MB_OK);
		return nullptr;
	}

	pResource->SetKey(_strKey);
	pResource->SetRelativePath(_strRelativePath);
	m_arrRes[(UINT)eResType].insert(make_pair(_strKey, pResource));

	return (T*)pResource;
}
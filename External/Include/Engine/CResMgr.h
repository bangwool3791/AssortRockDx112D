#pragma once

#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"
#include "CComputeShader.h"
#include "CPrefab.h"

class CResMgr
	:public CSingleton<CResMgr>
{
private:
	map<wstring, Ptr<CRes>>				m_arrRes[(UINT)RES_TYPE::END];
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayoutInfo;
	UINT								m_iLayoutOffset;
	bool								m_bChanged;
public:
	template<typename T>
	inline void AddRes(const wstring& _strKey, T* _pRes);
	template<typename T>
	Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);
	template<typename T>
	Ptr<T> FindRes(const wstring& _strKey);

public :
	void init();
	void tick() { m_bChanged = false; }
	bool IsChanged() { return m_bChanged; }

	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutInfo() { return m_vecLayoutInfo; }

	Ptr<CTexture> CreateTexture(const wstring& _strKey
		, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag);
	Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

	const map<wstring, Ptr<CRes>>& GetResource(RES_TYPE _eResType)
	{
		return m_arrRes[(UINT)_eResType];
	}

private:
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultGraphicsShader();
	void CreateDefaultMaterial();
	void CreateDefaultPrefab();
	void CreateDefaultComputeShader();
	void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName);

public :
	CResMgr();
	virtual  ~CResMgr();


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

	if (typeid(T).hash_code() == typeid(CComputeShader).hash_code())
		return RES_TYPE::COMPUTE_SHADER;

	if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		return RES_TYPE::TEXTURE;

	if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		return RES_TYPE::PREFAB;
	else
		return RES_TYPE::END;	
}

template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, T* _pRes)
{
	RES_TYPE eResType = ::GetType<T>();
	
	CRes* pRes = FindRes<T>(_strKey).Get();

	assert(!pRes);

	_pRes->SetKey(_strKey);
	m_arrRes[(UINT)eResType].insert(make_pair(_strKey, _pRes));
}

template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
	auto iter = m_arrRes[(UINT)::GetType<T>()].find(_strKey);

	if (iter == m_arrRes[(UINT)::GetType<T>()].end())
		return nullptr;

	return (T*)iter->second.Get();
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

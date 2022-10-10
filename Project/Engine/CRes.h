#pragma once
#include "Ptr.h"
#include "CEntity.h"

class CRes :
    public CEntity
{
private:
    wstring         m_strKey;
    wstring         m_strRelativePath;
    const RES_TYPE  m_eResType;
    int             m_iRefCount;

public:
    void AddRef()
    {
        ++m_iRefCount;
    }
    
    void Release()
    {
        --m_iRefCount;

        //assert(!(m_iRefCount < 0));

        if (0 == m_iRefCount)
        {
            delete this;
        }
    }
public:
    // 파일로 부터 로딩
    virtual int Load(const wstring& _strFilePath) = 0;


public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }
    RES_TYPE GetResType() { return m_eResType; }

public:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }

    template<typename T>
    friend class Ptr;

public:
    CRes(RES_TYPE _eResType);
    ~CRes();
};


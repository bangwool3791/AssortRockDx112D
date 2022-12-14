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

    bool            m_bEngineRes;

public:
    void AddRef()
    {
        ++m_iRefCount;
    }
    
    void Release()
    {
        --m_iRefCount;

        assert(!(m_iRefCount < 0));

        if (0 == m_iRefCount)
        {
            delete this;
        }
    }
public:
    //���Ϸ� ����
    virtual void Save(const wstring _strRelativePath) = 0;
    // ���Ϸ� ���� �ε�
    virtual int Load(const wstring& _strFilePath) = 0;

protected: 
    void SaveKeyPath(FILE* _pFile);
    void LoadKeyPath(FILE* _pFile);

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }
    RES_TYPE GetResType() { return m_eResType; }

public:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }

    template<typename T>
    friend class Ptr;
    friend class CResMgr;
    /*
    * �߻� �Լ��� �ڽĸ� ��� ������ ��.
    */
public:
    virtual CRes* Clone() = 0;
public:
    CRes(RES_TYPE _eResType, bool _bEngineRes = false);
    CRes(const CRes& _other);
    virtual ~CRes();
};


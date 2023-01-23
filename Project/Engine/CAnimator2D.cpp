#include "pch.h"

#include "CTexture.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

std::map<wstring, CAnimation2D*> CAnimator2D::m_mapRef{};

CAnimator2D::CAnimator2D()
    :CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_bRepeat(false)
    , m_mapAnim{}
    , m_strKey{}
{
    SetName(L"CAnimator2D");
}

CAnimator2D::CAnimator2D(const CAnimator2D& _rhs)
    :CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_bRepeat(_rhs.m_bRepeat)
    , m_mapAnim{}
    , m_strKey{ _rhs.m_strKey }
{
    SetName(L"CAnimator2D");

    CAnimation2D* pAnimation2D{};
    auto iter = _rhs.m_mapAnim.begin();
    for (; iter != _rhs.m_mapAnim.end(); ++iter)
    {
        pAnimation2D = nullptr;
        m_mapAnim.insert(make_pair(iter->first, pAnimation2D = iter->second->Clone()));
        pAnimation2D->SetOwner(this);
    }
    assert(m_mapAnim.size());
    --iter;
    Play(iter->first, true);
}

CAnimator2D::~CAnimator2D()
{
    Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
    assert(nullptr != _AtlasTex);

    CAnimation2D* pAnimation2D;

    pAnimation2D = FindAnimation(_strKey);

    assert(nullptr == pAnimation2D);

    pAnimation2D = new CAnimation2D;
    pAnimation2D->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);
    pAnimation2D->SetName(_strKey);
    pAnimation2D->m_pOwner = this;
    m_mapAnim[_strKey] = pAnimation2D;
}


bool SortAtlas(const tDxatlas& lhs, const tDxatlas& rhs)
{
    if (lhs.iFrame < rhs.iFrame)
        return true;
    else
        return false;
}

void CAnimator2D::CreateAnimation()
{
    TiXmlDocument ReadDoc;
    wstring wstrContent = CPathMgr::GetInst()->GetContentPath();
    wstring wstrPath = wstrContent;
    wstrPath.append(L"texture\\sprite");
    string strPath = WStringToString(wstrPath);
    FILE* pFile = nullptr;

    unordered_map<string, unordered_map<string, vector<tDxatlas>>> map;

    for (const auto& entry : fs::directory_iterator(strPath))
    {
        std::cout << entry.path() << std::endl;

        string path = entry.path().string();

        string extension = path.substr(path.size() - 8, 8);
        string atlas{};

        if (extension != ".dxatlas")
            continue;

        path = path.replace(path.size() - 8, path.size(), ".dds");
        atlas = path.substr(wstrContent.size(), path.size() - wstrContent.size());
        ReadDoc.LoadFile(entry.path().string().c_str());// xml 파일 로드
        //"DB"라는 노드를 찾는다
        TiXmlElement* ReadRoot = ReadDoc.FirstChildElement("Complex");
        //ReadRoot("DB")노드 하위의 "class1",의 하위 "Teacher"라는 노드를 찾는다.
        TiXmlElement* sub = ReadRoot->FirstChildElement("Properties")->FirstChildElement("SingleArray")->FirstChildElement("Items")->FirstChildElement("Complex");

        while (sub)
        {
            TiXmlElement* pElem = sub->FirstChildElement("Properties")->FirstChildElement("Simple");

            tDxatlas tData{};
            string str{};
            string strAnimation{};
            string stTemp{};
            string strFrame{};

            while (pElem)
            {
                TiXmlAttribute* pAttrib = pElem->FirstAttribute();
                while (pAttrib)
                {

                    if (!strcmp("n", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();

                        str = pAttrib->Value();
                        //Get a Image file name
                        //delete a number.png
                        strAnimation = str.substr(0, str.size() - 8);

                        if ('_' == strAnimation[strAnimation.size() - 1])
                        {
                            strAnimation = strAnimation.erase(strAnimation.size() - 1, 1);
                        }

                        std::cout << strAnimation << std::endl;

                        stTemp = str.substr(str.size() - 8, 8);
                        strFrame = stTemp.substr(1, 3);

                        tData.iFrame = atoi(strFrame.data());
                    }
                    else if (!strcmp("x", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.x = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("y", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.y = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("w", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.w = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("h", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.h = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("pX", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.pX = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("pY", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.pY = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("oX", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.oX = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("oY", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.oY = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("oW", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.oW = std::stof(pAttrib->Value());
                    }
                    else if (!strcmp("oH", pAttrib->Value()))
                    {
                        pAttrib = pAttrib->Next();
                        tData.oH = std::stof(pAttrib->Value());
                        map[atlas][strAnimation].push_back(tData);

                        sort(map[atlas][strAnimation].begin(), map[atlas][strAnimation].end(), SortAtlas);
                    }

                    pAttrib = pAttrib->Next();
                }
                pElem = pElem->NextSiblingElement();
            }

            sub = sub->NextSiblingElement();
        }
    }

    //adjust Full Size
    //Vec2 FullSize{};

    //for (auto iter{ map.begin() }; iter != map.end(); ++iter)
    //{

    //    Ptr<CTexture> _AtlasTex = CResMgr::GetInst()->FindRes<CTexture>(StringToWString(iter->first));

    //    assert(nullptr != _AtlasTex);

    //    for (auto iter2{ iter->second.begin() }; iter2 != iter->second.end(); ++iter2)
    //    {
    //        for (auto iter3{ iter2->second.begin() }; iter3 != iter2->second.end(); ++iter3)
    //        {
    //            Vec2 vFullSize = Vec2{ iter3->w,iter3->h };

    //            if (FullSize < vFullSize)
    //            {
    //                FullSize = vFullSize;
    //            }
    //        }

    //        for (auto iter3{ iter2->second.begin() }; iter3 != iter2->second.end(); ++iter3)
    //        {
    //            iter3->w = FullSize.x;
    //            iter3->h = FullSize.y;
    //        }
    //    }
    //}

    for (auto iter{ map.begin() }; iter != map.end(); ++iter)
    {

        Ptr<CTexture> _AtlasTex = CResMgr::GetInst()->FindRes<CTexture>(StringToWString(iter->first));

        assert(nullptr != _AtlasTex);

        for (auto iter2{ iter->second.begin() }; iter2 != iter->second.end(); ++iter2)
        {
            CAnimation2D* pAnimation2D;

            wstring _strKey = StringToWString(iter2->first);

            pAnimation2D = new CAnimation2D;
            pAnimation2D->Create(_strKey, _AtlasTex, iter2->second, 10);
            pAnimation2D->SetName(_strKey);
            pAnimation2D->m_pOwner = this;

            if (auto iter = m_mapRef[_strKey])
            {
                delete m_mapRef[_strKey];
            }

            m_mapRef[_strKey] = pAnimation2D;
        }
    }
}

void CAnimator2D::CloneAnimation(const wstring& _strKey, CAnimator2D& _pAnimation)
{
    for (auto iter{ m_mapRef.begin() }; iter != m_mapRef.end(); ++iter)
    {
        if (iter->first.find(_strKey) != std::string::npos)
        {
            _pAnimation.Add_Animation(iter->second);
        }
    }
}

void CAnimator2D::finaltick()
{
    if (!IsValid(m_pCurAnim))
        return;

    if (ANIMATION_STATE::FINISH == m_pCurAnim->m_eState && m_bRepeat)
    {
        m_pCurAnim->Reset();
    }

    m_pCurAnim->finaltick();
}


CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
    CAnimation2D* iter = m_mapAnim[_strKey];

    if (iter == nullptr)
        return nullptr;

    return iter;
}

void CAnimator2D::Play()
{
    auto iter = m_mapAnim.begin();

    m_strKey = iter->first;
    CAnimation2D* pAnimation = iter->second;

    assert(nullptr != pAnimation);

    if (IsValid(pAnimation))
    {
        m_pCurAnim = pAnimation;
    }
    m_bRepeat = true;
    pAnimation->Reset();
}

void CAnimator2D::Play(const wstring& _strKey)
{
    m_strKey = _strKey;
    CAnimation2D* pAnimation = FindAnimation(_strKey);

    assert(nullptr != pAnimation);

    if (IsValid(pAnimation))
    {
        m_pCurAnim = pAnimation;
    }
    pAnimation->Reset();
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
    m_strKey = _strKey;
    CAnimation2D* pAnimation = FindAnimation(_strKey);

    assert(nullptr != pAnimation);

    if (IsValid(pAnimation))
    {
        m_pCurAnim = pAnimation;
    }
    m_bRepeat = _bRepeat;
    pAnimation->Reset();
}

void CAnimator2D::Update()
{
    if (!IsValid(m_pCurAnim))
        return;

    if (m_bActive)
        m_pCurAnim->Update();
}

void CAnimator2D::UpdateData()
{
    if (!IsValid(m_pCurAnim))
        return;

    if (m_bActive)
        m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    if (!IsValid(m_pCurAnim))
        return;

    m_pCurAnim->Clear();
}


int  CAnimator2D::Add_Animation2D(Vec2 _vLeftTop, Vec2 _vSlice, float _fDuration, Vec2 _vFullSize)
{
    return m_pCurAnim->Add_Animation2D(_vLeftTop, _vSlice, _fDuration, _vFullSize);
}

int CAnimator2D::Delete_Animation2D()
{
    return m_pCurAnim->Delete_Animation2D();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
    COMPONENT_TYPE type = GetType();
    fwrite(&type, sizeof(UINT), 1, _File);

    // Animation 저장
    size_t iAnimCount = m_mapAnim.size();
    fwrite(&iAnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        pair.second->SaveToFile(_File);
    }
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
    // Animation
    size_t iAnimCount = 0;
    fread(&iAnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < iAnimCount; ++i)
    {
        CAnimation2D* pAnim = new CAnimation2D;
        pAnim->LoadFromFile(_File);

        pAnim->m_pOwner = this;
        m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
    }
}

void CAnimator2D::SetTexture(Ptr<CTexture> _AtlasTex)
{

    for (auto iter{ m_mapAnim.begin() }; iter != m_mapAnim.end(); ++iter)
    {
        iter->second->SetTexture(_AtlasTex);
    }
}

tAnim2DInfo CAnimator2D::GetAniInfo()
{
    if (m_pCurAnim)
        return m_pCurAnim->GetAniInfo();
    else
    {
        return tAnim2DInfo();
    }
}

const tAnim2DFrm& CAnimator2D::GetAniFrame()
{
    return m_pCurAnim->GetAniFrame();
}

void CAnimator2D::SetLeftTopX(float _fValue, int _index)
{
    m_pCurAnim->SetLeftTopX(_fValue, _index);
}

void CAnimator2D::SetLeftTopY(float _fValue, int _index)
{
    m_pCurAnim->SetLeftTopY(_fValue, _index);
}

void CAnimator2D::SetLeftTop(Vec2 _vLeftTop, int _index)
{
    m_pCurAnim->SetLeftTop(_vLeftTop, _index);
}

void CAnimator2D::SetSliceX(float _fValue, int _index)
{
    m_pCurAnim->SetSliceX(_fValue, _index);
}

void CAnimator2D::SetSliceY(float _fValue, int _index)
{
    m_pCurAnim->SetSliceY(_fValue, _index);
}

void CAnimator2D::SetSlice(Vec2 _vSlice, int _index)
{
    m_pCurAnim->SetSlice(_vSlice, _index);
}

void CAnimator2D::SetOffsetX(float _fValue, int _index)
{
    m_pCurAnim->SetOffsetX(_fValue, _index);
}

void CAnimator2D::SetOffsetY(float _fValue, int _index)
{
    m_pCurAnim->SetOffsetY(_fValue, _index);
}

void CAnimator2D::SetOffset(Vec2 _vOffset, int _index)
{
    m_pCurAnim->SetOffset(_vOffset, _index);
}

void CAnimator2D::SetFullSizeX(float _fValue, int _index)
{
    m_pCurAnim->SetFullSizeX(_fValue, _index);
}

void CAnimator2D::SetFullSizeY(float _fValue, int _index)
{
    m_pCurAnim->SetFullSizeY(_fValue, _index);
}

void CAnimator2D::SetFullSize(Vec2 _vFullSize, int _index)
{
    m_pCurAnim->SetFullSize(_vFullSize, _index);
}

void CAnimator2D::SetDuration(float _fDuration, int _index)
{
    m_pCurAnim->SetDuration(_fDuration, _index);
}

const wstring& CAnimator2D::GetKey()
{
    return m_pCurAnim->GetName();
}

const vector<wstring>& CAnimator2D::GetKeys()
{
    static vector<wstring> vec{};
    vec.clear();
    for (auto iter{ m_mapAnim.begin() }; iter != m_mapAnim.end(); ++iter)
    {
        vec.push_back(iter->first);
    }

    return vec;
}

void CAnimator2D::Add_Animation(CAnimation2D* _pAnimatnion)
{
    map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_pAnimatnion->GetName());

    if (iter != m_mapAnim.end())
        return;

    m_mapAnim.insert(make_pair(_pAnimatnion->GetName(), _pAnimatnion->Clone()));
}

CAnimation2D* CAnimator2D::Add_Animation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
    map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

    if (iter != m_mapAnim.end())
        return nullptr;

    CAnimation2D* pAnimation2D = new CAnimation2D;
    pAnimation2D->SetName(_strKey);
    pAnimation2D->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);
    m_mapAnim.insert(make_pair(_strKey, pAnimation2D));
    return pAnimation2D;
}

CAnimation2D* CAnimator2D::Delete_Animation(const wstring& _strName)
{
    if (m_mapAnim.size() <= 1)
        return nullptr;

    auto iter = m_mapAnim.find(_strName);

    if (iter == m_mapAnim.end())
        return nullptr;

    delete iter->second;

    iter = m_mapAnim.erase(iter);

    if (m_mapAnim.size() > 0)
    {
        --iter;
        return iter->second;
    }
    else
        return nullptr;
}

bool CAnimator2D::IsEnd()
{
    if (!IsValid(m_pCurAnim))
        return false;

    return m_pCurAnim->IsEnd();  
}

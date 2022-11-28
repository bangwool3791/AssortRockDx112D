#pragma once
#include "pch.h"
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_AtlasTex;     // Ÿ�ϸ��� ����ϴ� ��Ʋ�� �̹���
    Vec2                m_vTileCount;   // Ÿ�ϸ� ���� ���� ����

    vector<tTile>       m_vecTile;
    vector<list<tTile>>	m_vecAdjacency;
    CStructuredBuffer* m_TileBuffer;   // �� Ÿ���� ��Ʋ�� �������� ����ü

private:
    void Ready_Adjacency();
public:
    void SetTileAtlas(Ptr<CTexture> _AtlasTex) { m_AtlasTex = _AtlasTex; }
    void SetTileCount(UINT _iWidth, UINT _iHeight);

public:
    virtual void finaltick() override;
    virtual void render() override;


    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};


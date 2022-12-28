#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    CPrefab*         m_pTargetPrefab;
    TreeUI*          m_Tree;
    bool             m_bDragEvent;

public:
    void ResetContent();


private:
    virtual void update() override;
    virtual void render_update() override;


private:
    void SetResourceToInspector(DWORD_PTR _res);
    void SetDragObject(DWORD_PTR _res);
public:
    ContentUI();
    ~ContentUI();
};


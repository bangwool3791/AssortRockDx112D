#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;

class InspectorUI :
    public UI
{
private:
    CGameObject*    m_TargetObj;
    ComponentUI*    m_arrComUI[(UINT)COMPONENT_TYPE::END];
    ComponentUI*    m_arrObjUI[(UINT)OBJECT_TYPE::END];
public:
    void SetTarget(CGameObject* _Target);
    void SetLight(CGameObject* _Light);


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};


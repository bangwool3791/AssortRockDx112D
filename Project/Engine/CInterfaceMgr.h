#pragma once

class CGameObject;

class CInterfaceMgr :
	public CSingleton<CInterfaceMgr>
{
private:
	CGameObject*				   m_pTarget;
	vector<CGameObject*>		   m_vecTapButton;
public:
	CGameObject* GetTarget() { return m_pTarget; }
	void SetTarget(CGameObject* _pGameObject) { m_pTarget = _pGameObject; }
	void AddTapButton(CGameObject* _pGameObject);
private:
	CInterfaceMgr();
	virtual ~CInterfaceMgr();
	friend class CSingleton<CInterfaceMgr>;
};
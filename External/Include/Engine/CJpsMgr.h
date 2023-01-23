#pragma once

#include "Base.h"

class CJpsMgr : public CSingleton<CJpsMgr>
{
private:
	CGameObject*				  m_pTileObejct;
	vector<Vec3>				  m_vecResult;
	JPSPath						  m_JpsPath;
	std::shared_ptr<JPSCollision> m_spCollision;
	Int32 GridWidth = TILEX;
	Int32 GridHeight = TILEZ;
	std::list<JPSCoord> ResultNodes;	// Result for JPS
public :
	void Initialize(Ptr<CMesh> _Mesh);
	const vector<Vec3>& Update(Int32 x1, Int32 z1, Int32 x2, Int32 z2);
	void SetCollision(Int32 x, Int32 z);
	void ClearCollision(Int32 x, Int32 z);
	CGameObject* GetTileObj() { return m_pTileObejct; }
private:
	CJpsMgr();
	virtual ~CJpsMgr();
	friend class CSingleton<CJpsMgr>;
};
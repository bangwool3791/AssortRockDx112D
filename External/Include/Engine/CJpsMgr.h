#pragma once

#include "Base.h"

class CJpsMgr :
	public CSingleton<CJpsMgr>
{
private:
	JPSPath	m_JpsPath;
	std::shared_ptr<JPSCollision> m_spCollision;
	Int32 GridWidth = TERRAINX;
	Int32 GridHeight = TERRAINZ;
	std::list<JPSCoord> ResultNodes;	// Result for JPS
public :
	void Initialize(Ptr<CMesh> _Mesh);
	const vector<Vec3>& Update(Int32 x, Int32 z);
	void SetCollision(Int32 x, Int32 z);
	void ClearCollision(Int32 x, Int32 z)
private:
	CJpsMgr();
	virtual ~CJpsMgr();
	friend class CSingleton<CJpsMgr>;
};
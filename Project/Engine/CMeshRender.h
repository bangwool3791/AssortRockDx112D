#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender
	:public CComponent
{
private :
	Ptr<CMesh>				m_pMesh;
	Ptr<CMaterial>			m_pMtrl;

public :
	void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(Ptr<CMaterial> _pMtrl) { m_pMtrl = _pMtrl; }
	Ptr<CMaterial> GetMaterial() { return m_pMtrl; }
public:
	virtual void tick() override;
	virtual void finaltick() override;
	void render();

public :
	CMeshRender();
	virtual ~CMeshRender();
};

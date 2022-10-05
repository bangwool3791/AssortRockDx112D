#pragma once
#include "CComponent.h"

class CMesh;
class CMaterial;

class CMeshRender
	:public CComponent
{
private :
	CMesh*				m_pMesh;
	CMaterial*			m_pMtrl;

public :
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }
	CMaterial* GetMaterial() { return m_pMtrl; }
public:
	virtual void tick() override;
	virtual void finaltick() override;
	void render();

public :
	CMeshRender();
	virtual ~CMeshRender();
};

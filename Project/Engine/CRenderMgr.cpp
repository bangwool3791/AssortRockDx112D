#include "pch.h"

#include "CRenderMgr.h"
#include "CCamera.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{

}

void CRenderMgr::init()
{
}
	
void CRenderMgr::tick()
{
	m_vecCam.clear();
}

void CRenderMgr::render()
{
	for (auto elem{ m_vecCam.begin() }; elem != m_vecCam.end(); ++elem)
	{
		(*elem)->render();
	}
}

#include "pch.h"

#include "CLevel.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CRenderMgr::CRenderMgr()
	:m_pLight2DBuffer{}
{
	m_pLight2DBuffer = new CStructuredBuffer;
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);

}

CRenderMgr::~CRenderMgr()
{
	Safe_Delete(m_pLight2DBuffer);
}

void CRenderMgr::init()
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();

	// 후처리 용도, 렌더타겟 복사용
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}
	
void CRenderMgr::tick()
{
	m_vecCam.clear();
	//광원 정보 매 프레임 마다 삭제
	m_vecLight2D.clear();
}

void CRenderMgr::render()
{

	// 렌더타겟 지정
	static Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	static Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	UpdateNoiseTexture();

	UpdateLight2D();

	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (LEVEL_STATE::PLAY == pLevel->GetState())
	{
		render_game();
	}
	else
	{
		redner_editor();
	}

}

void CRenderMgr::render_game()
{
	for (auto elem{ m_vecCam.begin() }; elem != m_vecCam.end(); ++elem)
	{
		(*elem)->render();
	}
}

void CRenderMgr::redner_editor()
{
	assert(m_EditorCam);
	m_EditorCam->render();
}

void CRenderMgr::UpdateNoiseTexture()
{
	Ptr<CTexture> NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"Noise_02");

	NoiseTex->UpdateData(58, PIPELINE_STAGE::ALL_STAGE);
	NoiseTex->UpdateData_CS(58, true);

	g_global.vNoiseResolution = Vec2{ NoiseTex->GetWidth(), NoiseTex->GetHeight() };
}

/*
* Light2D Component final Tick에서 Update된 m_vecLight2D 데이터를 구조체 버퍼 저장
*/
void CRenderMgr::UpdateLight2D()
{
	if (m_pLight2DBuffer->GetElementsCount() < m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementsSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}
		
	/*
	* 구조체 버퍼안에, 조명 정보를 입력한다.
	*/
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), (UINT)m_vecLight2D.size());

	m_pLight2DBuffer->UpdateData(55, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}

void CRenderMgr::CopyRenderTarget()
{
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	
	ID3D11ShaderResourceView* SRV = nullptr;

	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

CCamera* CRenderMgr::GetMainCam()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetLevelState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}

#pragma once
#include "CEntity.h"

class CStructuredBuffer :
	public CEntity
{
private:
	ComPtr<ID3D11Buffer>				m_SB;
	ComPtr<ID3D11ShaderResourceView>	m_SRV;

	D3D11_BUFFER_DESC					m_tDesc;
	SB_TYPE								m_eType;

	UINT								m_iElementSize;
	UINT								m_iElementCount;

public:
	int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, void* _pInitial);
	UINT GetElementsSize() { return m_iElementSize; }
	UINT GetElementsCount() { return m_iElementCount; }

	void SetData(void* _pSrc, UINT _iElementCount);

	void UpdateData(UINT _RegisterNum, UINT _PipelineStage);

public :
	CLONE(CStructuredBuffer);

public :
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& _origin);
	~CStructuredBuffer();
};
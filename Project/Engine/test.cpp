#include "pch.h"
#include "test.h"

#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CDevice.h"
#include "CTimeMgr.h"


// ID3D11Buffer 의 종류
// Vertex Buffer, Index Buffer, Constant Buffer, StructuredBuffer

ID3D11Buffer* g_CB{ nullptr };

ID3D11Buffer* g_VB{ nullptr };
ID3D11InputLayout* g_Layout{ nullptr };

ID3DBlob* g_ErrBlob{ nullptr };

ID3DBlob* g_VSBlob{ nullptr };
ID3D11VertexShader* g_VS{ nullptr };

ID3DBlob* g_PSBlob{ nullptr };
ID3D11PixelShader* g_PS{ nullptr };

Vec3 g_vPos;

void init()
{

	D3D11_BUFFER_DESC bufferdesc = {};

	bufferdesc.ByteWidth = sizeof(Vec4); // 16 의 배수로
	bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = DEVICE->CreateBuffer(&bufferdesc, nullptr, &g_CB);

	Vtx arrVtx[3];

	arrVtx[0].vPos = Vec3{ 0.f, 1.f, 1.f };
	arrVtx[0].vColor = Vec4{ 1.f, 0.f, 0.f, 1.f };

	arrVtx[1].vPos = Vec3{ 1.f, 0.f, 1.f };
	arrVtx[1].vColor = Vec4{ 0.f, 1.f, 0.f, 1.f };

	arrVtx[2].vPos = Vec3{ -1.f, 0.f, 1.f };
	arrVtx[2].vColor = Vec4{ 0.f, 0.f, 1.f, 1.f };

	bufferdesc = {};
	bufferdesc.ByteWidth = sizeof(Vtx) * 3;
	bufferdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bufferdesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	bufferdesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA tSubData{};

	tSubData.pSysMem = arrVtx;

	hr = DEVICE->CreateBuffer(&bufferdesc, &tSubData, &g_VB);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();

	hr = D3DCompileFromFile((strFilePath + L"shader\\test.fx").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Test", "vs_5_0", 0, 0, &g_VSBlob, &g_ErrBlob);

	hr = DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), nullptr, &g_VS);

	hr = D3DCompileFromFile((strFilePath + L"shader\\test.fx").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Test", "ps_5_0", 0, 0, &g_PSBlob, &g_ErrBlob);

	hr = DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), nullptr, &g_PS);

	// InputLayout
	// IA 단계에 전달됨
	// 정점 구조 정보
	D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {};

	arrLayout[0].AlignedByteOffset = 0;
	arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	arrLayout[0].InputSlot = 0;
	arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrLayout[0].SemanticName = "POSITION";
	arrLayout[0].SemanticIndex = 0;

	arrLayout[1].AlignedByteOffset = 12;
	arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	arrLayout[1].InputSlot = 0;
	arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrLayout[1].SemanticName = "COLOR";
	arrLayout[1].SemanticIndex = 0;

	hr = DEVICE->CreateInputLayout(arrLayout, 2, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), &g_Layout);
}

void tick()
{
	// 키 입력으로 삼각형 움직이기
	if (KEY_PRESSED(KEY::LEFT))
	{
		g_vPos.x -= DT * 1.f;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		g_vPos.x += DT * 1.f;
	}

	if (KEY_PRESSED(KEY::UP))
	{
		g_vPos.y += DT * 1.f;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		g_vPos.y -= DT * 1.f;
	}

	Vec4 vPos = Vec4(g_vPos, 0.f);

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(g_CB, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &vPos, sizeof(Vec4));
	CONTEXT->Unmap(g_CB, 0);
}

void render()
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &g_VB, &iStride, &iOffset);
	CONTEXT->IASetInputLayout(g_Layout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CONTEXT->VSSetShader(g_VS, 0, 0);
	CONTEXT->PSSetShader(g_PS, 0, 0);

	CONTEXT->VSSetConstantBuffers(0, 1, &g_CB);
	CONTEXT->Draw(3, 0);
}

void release()
{
	g_VB->Release();
	g_Layout->Release();

	g_VSBlob->Release();
	g_VS->Release();
	g_PSBlob->Release();
	g_PS->Release();

	if (nullptr != g_ErrBlob)
	{
		g_ErrBlob->Release();
	}
}
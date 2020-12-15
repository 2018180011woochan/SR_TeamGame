#include "..\Header\Image.h"

USING(Engine)

const DWORD VERTEXRECT::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

Image::Image(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_nScreenHeight(0)
	, m_nScreenWidth(0)
	, m_nIndexCount(0)
	, m_nVertexCount(0)
{
}

void Image::Free()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pTexture);
}

Image * Image::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	Image* pInstance = new Image(_pGameObject, _pDevice);
	if (nullptr == pInstance)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to Create 'Image' Component"));
		return nullptr;
	}
	if (FAILED(pInstance->Initialize()))
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to initialize component. (CMeshRenderer)"));
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

HRESULT Image::Initialize()
{
	return S_OK;
}

HRESULT Image::SetTexture(CTexture * const _pTexture)
{
	//이전 텍스처 릴리즈
	SafeRelease(m_pTexture);
	//텍스처 변경
	m_pTexture = _pTexture;
	//변경된 텍스처 레퍼런스 카운트 증가.
	SafeAddRef(_pTexture);
	return S_OK;
}

HRESULT Image::CreateBuffer()
{
	m_nVertexCount = 6;
	m_nIndexCount = 2;
	//버텍스 버퍼 생성.
	m_pDevice->CreateVertexBuffer(
		sizeof(VERTEXRECT) * m_nVertexCount,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		VERTEXRECT::FVF,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer,
		0);
	
	LPVERTEXRECT pVertices = nullptr;

	/*
		1--------2
		|		 |
		|		 |
		|		 |
		0--------3
	*/

	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0].Position	= D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVertices[0].UV			= D3DXVECTOR2(0.f, 1.f);
	pVertices[1].Position	= D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertices[1].UV			= D3DXVECTOR2(0.f, 0.f);
	pVertices[2].Position	= D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertices[2].UV			= D3DXVECTOR2(1.f, 0.f);
	pVertices[3].Position	= D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertices[3].UV			= D3DXVECTOR2(1.f, 1.f);
	m_pVertexBuffer->Unlock();

	m_pDevice->CreateIndexBuffer(
		sizeof(INDEX16) * m_nIndexCount,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndexBuffer,
		0
	);

	LPINDEX16 pIndices = nullptr;
	m_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	pIndices[0] = { 0,1,2 };
	pIndices[1] = { 0,2,3 };
	m_pIndexBuffer->Unlock();
	return E_NOTIMPL;
}


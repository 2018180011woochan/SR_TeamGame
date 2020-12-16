#include "..\Header\Image.h"
#include "Texture.h"

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
	, m_eImageType(ImageType::Simple)
	, m_eFillMethod(FillMethod::Horizontal)
	, m_eFillOrigin(FillOrigin::Left)
	, m_fFillAmount(1.0f)
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
	if (FAILED(CreateBuffer()))
		return E_FAIL;
	return S_OK;
}

HRESULT Image::Render()
{
	if (FAILED(m_pDevice->SetTexture(0, (m_pTexture == nullptr ? 0 : m_pTexture->GetTexture()))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEXRECT))))
		return E_FAIL;
	if (FAILED(m_pDevice->SetFVF(VERTEXRECT::FVF)))
		return E_FAIL;
	if (FAILED(m_pDevice->SetIndices(m_pIndexBuffer)))
		return E_FAIL;
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 0, m_nIndexCount);

}

HRESULT Image::SetTexture(CTexture * const _pTexture)
{
	//이전 텍스처 릴리즈
	SafeRelease(m_pTexture);
	//텍스처 변경
	m_pTexture = _pTexture;
	//변경된 텍스처 레퍼런스 카운트 증가.
	SafeAddRef(_pTexture);

	SetNativeSize();

	return S_OK;
}

HRESULT Image::SetNativeSize()
{
	if (nullptr == m_pTexture || nullptr == m_pTexture->GetTexture())
		return S_OK;
	LPVERTEXRECT pVertices = nullptr;

	D3DXIMAGE_INFO tImageDesc = m_pTexture->GetDesc();

	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	
	pVertices[0].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f), -float(tImageDesc.Height * 0.5f), 0.f);
	pVertices[1].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f),  float(tImageDesc.Height * 0.5f), 0.f);
	pVertices[2].Position = D3DXVECTOR3( float(tImageDesc.Width * 0.5f),  float(tImageDesc.Height * 0.5f), 0.f);
	pVertices[3].Position = D3DXVECTOR3( float(tImageDesc.Width * 0.5f), -float(tImageDesc.Height * 0.5f), 0.f);
	
	m_pVertexBuffer->Unlock();


	return S_OK;
}

HRESULT Image::SetImageType(const ImageType _eImageType)
{
	m_eImageType = _eImageType;
	return S_OK;
}

HRESULT Image::SetFillAmount(const float _fValue)
{
	if (ImageType::Fill != m_eImageType)
		return S_OK;
	if (_fValue == m_fFillAmount)
		return S_OK;

	m_fFillAmount = _fValue;

	if (0 > m_fFillAmount)
		m_fFillAmount = 0;
	else if (1 < m_fFillAmount)
		m_fFillAmount = 1;

	switch (m_eFillMethod)
	{
	case Engine::Image::Horizontal:
		FillHorizontal();
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT Image::CreateBuffer()
{
	m_nVertexCount = 4;
	m_nIndexCount = 2;
	//버텍스 버퍼 생성.
	m_pDevice->CreateVertexBuffer(
		sizeof(VERTEXRECT) * m_nVertexCount,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS |D3DUSAGE_WRITEONLY,
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
	return S_OK;
}

HRESULT Image::FillHorizontal()
{
	D3DXIMAGE_INFO tImageDesc = m_pTexture->GetDesc();

	LPVERTEXRECT pVertices = nullptr;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, D3DLOCK_DONOTWAIT);

	switch (m_eFillOrigin)
	{
	case Engine::Image::Left:
		pVertices[0].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f), -float(tImageDesc.Height * 0.5f), 0.f);
		pVertices[1].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f),  float(tImageDesc.Height * 0.5f), 0.f);
		pVertices[2].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f) + float(m_fFillAmount * tImageDesc.Width),  float(tImageDesc.Height * 0.5f), 0.f);
		pVertices[2].UV = D3DXVECTOR2(m_fFillAmount, 0.f);
		pVertices[3].Position = D3DXVECTOR3(-float(tImageDesc.Width * 0.5f) + float(m_fFillAmount * tImageDesc.Width), -float(tImageDesc.Height * 0.5f), 0.f);
		pVertices[3].UV = D3DXVECTOR2(m_fFillAmount, 1.f);
		break;
	case Engine::Image::Right:
		break;
	default:
		break;
	}

	m_pVertexBuffer->Unlock();
	return S_OK;
}


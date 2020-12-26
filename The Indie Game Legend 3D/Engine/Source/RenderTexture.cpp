#include "..\Header\RenderTexture.h"
#include "GameObject.h"
USING(Engine)

CRenderTexture::CRenderTexture()
	: m_pSurface(nullptr)
	, m_pZBuffer(nullptr)
	, m_pOrigSurface(nullptr)
	, m_pOrigZBuffer(nullptr)
	, m_pSprite(nullptr)
{
	SafeAddRef(m_pDevice);
	ZeroMemory(&m_tSurfaceDesc, sizeof(D3DSURFACE_DESC));
}

void CRenderTexture::Free()
{
	CTexture::Free();
	SafeRelease(m_pOrigSurface);
	SafeRelease(m_pOrigZBuffer);
	SafeRelease(m_pZBuffer);
	SafeRelease(m_pSurface);
	SafeRelease(m_pSprite);
}

CRenderTexture * CRenderTexture::Create()
{
	CRenderTexture* pInstance = new CRenderTexture();
	return pInstance;
}

HRESULT CRenderTexture::Initialize(const UINT _nCX, const UINT _nCY)
{
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		return E_FAIL;

	if (nullptr == m_pOrigSurface)
	{
		m_pDevice->GetRenderTarget(0, &m_pOrigSurface);
		m_pDevice->GetDepthStencilSurface(&m_pOrigZBuffer);
	}
	
	//기존에 만들어진 텍스처 릴리즈
	SafeRelease(m_pZBuffer);
	SafeRelease(m_pSurface);
	SafeRelease(m_pTexture);

	//RenderTarget 용도의 텍스처 생성
	if (FAILED(m_pDevice->CreateTexture(_nCX, _nCY, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, 0)))
		return E_FAIL;
	//생성된 텍스처의 Surface보관.
	if (FAILED(m_pTexture->GetSurfaceLevel(0, &m_pSurface)))
		return E_FAIL;
	//생성된 텍스처 Surface 정보 보관.
	if (FAILED(m_pSurface->GetDesc(&m_tSurfaceDesc)))
		return E_FAIL;

	m_tImageInfo.Format = m_tSurfaceDesc.Format;
	m_tImageInfo.Height = m_tSurfaceDesc.Height;
	m_tImageInfo.Width = m_tSurfaceDesc.Width;

	//BackBuffer의 포맷 정보 받아옴.
	D3DSURFACE_DESC tOrigZBufferDesc;
	ZeroMemory(&tOrigZBufferDesc, sizeof(D3DSURFACE_DESC));
	if (FAILED(m_pOrigZBuffer->GetDesc(&tOrigZBufferDesc)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilSurface(_nCX, _nCY, tOrigZBufferDesc.Format, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZBuffer, NULL)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderTexture::DrawOnRenderTexture(LPDIRECT3DTEXTURE9 _pTexture, const RECT* _pRect, const D3DXVECTOR3& _vCenter, const D3DXMATRIX& _matWorld, const bool _bClear)
{
	if (nullptr == _pTexture)
		return E_FAIL;

	m_pDevice->SetDepthStencilSurface(m_pZBuffer);
	m_pDevice->SetRenderTarget(0, m_pSurface);

	if (true == _bClear)
		m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 0.f, 0);

	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	m_pSprite->SetTransform(&_matWorld);
	m_pSprite->Draw(_pTexture, _pRect, &_vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->SetRenderTarget(0, m_pOrigSurface);
	m_pDevice->SetDepthStencilSurface(m_pOrigZBuffer);
	return S_OK;
}



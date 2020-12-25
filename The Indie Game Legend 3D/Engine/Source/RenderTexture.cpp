#include "..\Header\RenderTexture.h"

USING(Engine)

CRenderTexture::CRenderTexture(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pTexture(nullptr)
	, m_pSurface(nullptr)
	, m_pZBuffer(nullptr)
	, m_pOrigSurface(nullptr)
	, m_pOrigZBuffer(nullptr)
{
	ZeroMemory(&m_tSurfaceDesc, sizeof(D3DSURFACE_DESC));
}

void CRenderTexture::Free()
{
	CComponent::Free();
	SafeRelease(m_pOrigSurface);
	SafeRelease(m_pOrigZBuffer);
	SafeRelease(m_pZBuffer);
	SafeRelease(m_pSurface);
	SafeRelease(m_pTexture);
}

CRenderTexture * CRenderTexture::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CRenderTexture* pInstance = new CRenderTexture(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

HRESULT CRenderTexture::Initialize()
{
	return S_OK;
}

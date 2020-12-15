#include "..\Header\MeshRenderer.h"
#include "MeshManager.h"
USING(Engine)

CMeshRenderer::CMeshRenderer(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pMeshManager(CMeshManager::GetInstance())
	, m_pMesh(nullptr)
{
	SafeAddRef(m_pMeshManager);
}

void CMeshRenderer::Free()
{
	SafeRelease(m_pMesh);
	SafeRelease(m_pMeshManager);
	
}

HRESULT CMeshRenderer::Initialize()
{
	return S_OK;
}

HRESULT CMeshRenderer::Render()
{
	if (nullptr == m_pMesh)
		return E_FAIL;

	m_pMesh->Render();

	return S_OK;
}

CMeshRenderer * CMeshRenderer::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CMeshRenderer* pInstance = new CMeshRenderer(_pGameObject, _pDevice);

	if (nullptr == pInstance)
	{
		PrintLog(TEXT("Warning"), TEXT("Failed to create component. (CMeshRenderer)"));
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

HRESULT CMeshRenderer::SetMesh(const TSTRING & _sMeshID)
{
	//기존 메쉬 릴리즈
	SafeRelease(m_pMesh);

	m_pMesh = m_pMeshManager->Clone(_sMeshID);

	if (nullptr == m_pMesh)
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshRenderer::SetTexture(const UINT _nGroupIndex, CTexture* const _pTexture)
{
	if (nullptr == m_pMesh)
		return E_FAIL;
	return m_pMesh->SetTexture(_nGroupIndex, _pTexture);
}



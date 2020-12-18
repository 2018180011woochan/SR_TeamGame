#include "..\Header\Collider.h"
#include "MeshManager.h"
USING(Engine)


CCollider::CCollider(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pMeshManager(CMeshManager::GetInstance())
	, m_pCollisionMesh(nullptr)
{
	SafeAddRef(m_pMeshManager);
	ZeroMemory(&m_tBound, sizeof(BOUND));
}

void CCollider::Free()
{
	CComponent::Free();
	SafeRelease(m_pCollisionMesh);
	SafeRelease(m_pMeshManager);
}

CCollider * CCollider::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CCollider* pInstance = new CCollider(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

HRESULT CCollider::SetMesh(const TSTRING & _sMesh)
{	//기존 메쉬 릴리즈
	SafeRelease(m_pCollisionMesh);

	m_pCollisionMesh = m_pMeshManager->Clone(_sMesh);

	if (nullptr == m_pCollisionMesh)
		return E_FAIL;
	//바운드 구조체 설정.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)(m_pCollisionMesh->GetVertices()), 
		(DWORD)(m_pCollisionMesh->GetVertexCount()), 
		sizeof(VERTEX), 
		&(m_tBound.vCenter), 
		&(m_tBound.fRadius));
	return S_OK;
}

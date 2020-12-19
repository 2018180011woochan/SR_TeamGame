#include "..\Header\Collider.h"
#include "MeshManager.h"
#include "GameObject.h"
USING(Engine)


CCollider::CCollider(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pMeshManager(CMeshManager::GetInstance())
	, m_pCollisionMesh(nullptr)
{
	SafeAddRef(m_pMeshManager);
	ZeroMemory(&m_tBoundingBox, sizeof(BOUNDINGBOX));
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

	//SafeAddRef(m_pCollisionMesh);

	SetBound();
	return S_OK;
}

HRESULT CCollider::Draw()
{
	if (nullptr == m_pCollisionMesh)
		return E_FAIL;
	return m_pCollisionMesh->Draw();
}

void CCollider::SetBound()
{
	if (nullptr == m_pCollisionMesh)
		return;

	LPVERTEX	pVertices = new VERTEX[m_pCollisionMesh->GetVertexCount()];
	UINT		nVertexCount = m_pCollisionMesh->GetVertexCount();

	memcpy(pVertices, m_pCollisionMesh->GetVertices(), sizeof(VERTEX) * nVertexCount);

	D3DXMATRIX matScale;
	D3DXVECTOR3 vScale = ((CTransform*)m_pGameObject->GetComponent<CTransform>())->Get_TransformDesc().vScale;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);


	for (UINT i = 0; i < nVertexCount; ++i)
	{
		D3DXVec3TransformCoord(&(pVertices[i].Position), &(pVertices[i].Position), &matScale);
	}

	//바운딩 박스 구조체
	D3DXComputeBoundingBox((D3DXVECTOR3*)(pVertices), nVertexCount, sizeof(VERTEX), &(m_tBoundingBox.vMin), &(m_tBoundingBox.vMax));

	SafeDeleteArray(pVertices);
}

BOUNDINGBOX CCollider::GetBound()
{
	CTransform* pTransform = nullptr;

	pTransform = (CTransform*)(m_pGameObject->GetComponent<CTransform>());

	BOUNDINGBOX tBoundingBox = m_tBoundingBox;
	tBoundingBox.vMin += pTransform->Get_Position();
	tBoundingBox.vMax += pTransform->Get_Position();

	return tBoundingBox;
}

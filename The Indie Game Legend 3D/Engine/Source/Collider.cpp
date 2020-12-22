#include "..\Header\Collider.h"
#include "MeshManager.h"
#include "GameObject.h"

//test
#include <iostream>
USING(std);
USING(Engine)


CCollider::CCollider(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_pMeshManager(CMeshManager::GetInstance())
	, m_pCollisionMesh(nullptr)
	, m_bIsRigid(false)
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

	UINT      nVertexCount = m_pCollisionMesh->GetVertexCount();
	D3DXVECTOR3*   pVertices = new D3DXVECTOR3[nVertexCount];

	memcpy(pVertices, m_pCollisionMesh->GetVertices(), sizeof(D3DXVECTOR3) * nVertexCount);

	D3DXMATRIX matScale;
	D3DXVECTOR3 vScale = ((CTransform*)m_pGameObject->GetComponent<CTransform>())->Get_TransformDesc().vScale;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);


	for (UINT i = 0; i < nVertexCount; ++i)
	{
		D3DXVec3TransformCoord(&(pVertices[i]), &(pVertices[i]), &matScale);
	}

	//바운딩 박스 구조체
	D3DXComputeBoundingBox((D3DXVECTOR3*)(pVertices), nVertexCount, sizeof(D3DXVECTOR3), &(m_tBoundingBox.vMin), &(m_tBoundingBox.vMax));

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

const D3DXVECTOR3* CCollider::GetVertices()
{
	if (nullptr == m_pCollisionMesh)
		return nullptr;
	return m_pCollisionMesh->GetVertices();
}

const UINT CCollider::GetVertexCount()
{
	if (nullptr == m_pCollisionMesh)
		return 0;
	return m_pCollisionMesh->GetVertexCount();
}

bool CCollider::IsRayPicking(OUT D3DXVECTOR3& _pOut, OUT float& _Dis, const D3DXVECTOR3 _vRayPv, const D3DXVECTOR3 _vRayDir)
{
	const D3DXVECTOR3* pVertices =  GetVertices();
	CTransform* pTrans = (CTransform*)m_pGameObject->GetComponent<CTransform>();
	_matrix matWorld = pTrans->Get_TransformDesc().matWorld;
	_matrix matInvWorld = matWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &matInvWorld);

	//월드 > 로컬
	_vector vRayPv = _vRayPv;
	_vector vRayDir = _vRayDir;
	D3DXVec3TransformCoord(&vRayPv, &vRayPv, &matInvWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matInvWorld);
	D3DXVec3Normalize(&vRayDir, &vRayDir);
	
	_uint nMaxCount = m_pCollisionMesh->GetVertexCount();

	float fU = 0.f, fV = 0.f;
	_vector V1, V2, V3;

	for (_uint i = 0; i < nMaxCount; i += 3)
	{
		V1 = pVertices[i];
		V2 = pVertices[i+1];
		V3 = pVertices[i+2];
		if (D3DXIntersectTri(&V1,&V2,&V3,
			&vRayPv,&vRayDir,&fU,&fV,&_Dis))
		{
			if(_Dis < 1.f)
				continue;

			// 교차점 = V1 + U(V2 - V1) + V(V3 - V1)
			_pOut = V1 + fU * (V2 - V1) + fV * (V3 - V1);
			D3DXVec3TransformCoord(&_pOut, &_pOut, &matWorld);//로컬 > 월드
			return true;
		}

	}
	return false;
}

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
	, m_pDrawMesh(nullptr)
	, m_bIsRigid(false)
{
	SafeAddRef(m_pMeshManager);
	ZeroMemory(&m_tBound, sizeof(BOUND));
}

void CCollider::Free()
{
	CComponent::Free();
	SafeRelease(m_pDrawMesh);
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

HRESULT CCollider::SetMesh(const TSTRING & _sMesh, const BOUND::BOUNDTYPE _eBoundType)
{
	SafeRelease(m_pCollisionMesh);

	m_pCollisionMesh = m_pMeshManager->Clone(_sMesh);

	if (nullptr == m_pCollisionMesh)
		return E_FAIL;

	m_tBound.eType = _eBoundType;
	SetBound();
	return S_OK;
}

HRESULT CCollider::Draw()
{
	if (nullptr == m_pDrawMesh)
		return E_FAIL;
	CTransform* pTransform = ((CTransform*)GetGameObject()->GetComponent<CTransform>());
	D3DXVECTOR3 vPosition = pTransform->Get_Position();
	D3DXMATRIX matScale;
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	if (BOUND::BOUNDTYPE::BOX == m_tBound.eType)
		D3DXMatrixScaling(&matScale, m_tBound.fLength, m_tBound.fHeight, m_tBound.fDepth);
	else
		D3DXMatrixScaling(&matScale, m_tBound.fRadius, m_tBound.fRadius, m_tBound.fRadius);

	D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, vPosition.z);

	matWorld = matScale * matTrans;

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	return m_pDrawMesh->Draw();
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
	if (BOUND::BOUNDTYPE::BOX == m_tBound.eType)
	{
		D3DXComputeBoundingBox((D3DXVECTOR3*)(pVertices), nVertexCount, sizeof(D3DXVECTOR3), &(m_tBound.vMin), &(m_tBound.vMax));
		m_tBound.fLength = m_tBound.vMax.x - m_tBound.vMin.x;
		m_tBound.fHeight = m_tBound.vMax.y - m_tBound.vMin.y;
		m_tBound.fDepth = m_tBound.vMax.z - m_tBound.vMin.z;
		m_pDrawMesh = m_pMeshManager->Clone(TEXT("Cube"));
	}
	else
	{
		D3DXComputeBoundingSphere((D3DXVECTOR3*)(pVertices), nVertexCount, sizeof(D3DXVECTOR3), &(m_tBound.vCenter), &(m_tBound.fRadius));
		m_pDrawMesh = m_pMeshManager->Clone(TEXT("Sphere"));
	}
	SafeDeleteArray(pVertices);
}

BOUND CCollider::GetBound()
{
	CTransform* pTransform = nullptr;
	BOUND tBound;
	pTransform = (CTransform*)(m_pGameObject->GetComponent<CTransform>());

	//바운딩 박스 구조체
	if (BOUND::BOUNDTYPE::BOX == m_tBound.eType)
	{
		tBound = m_tBound;
		tBound.vCenter = pTransform->Get_Position();
		tBound.vMin += pTransform->Get_Position();
		tBound.vMax += pTransform->Get_Position();
	}
	else
	{
		tBound = m_tBound;
		tBound.vCenter = pTransform->Get_Position();
	}

	return tBound;
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

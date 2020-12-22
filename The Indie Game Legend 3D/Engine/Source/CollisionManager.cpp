#include "..\Header\CollisionManager.h"
#include "GameObject.h"
#include "Collider.h"
#include <iostream>
#include <time.h>
USING(Engine)
IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

void CCollisionManager::Free()
{
}

void CCollisionManager::CollisionCheck(list<CGameObject*> _GameObjects)
{
	vector<CCollider*>	Collider;

	CCollider* pCollider = nullptr;

	for (CGameObject* pGameObject : _GameObjects)
	{
		if (false == pGameObject->IsEnable())
			continue;

		pCollider = (CCollider*)pGameObject->GetComponent<CCollider>();

		if (nullptr == pCollider)
			continue;

		Collider.push_back(pCollider);
	}

	UINT nSize = Collider.size();
	bool bCollision = false;

	for (UINT i = 0; i < nSize; ++i)
	{
		for (UINT j = i + 1; j < nSize; ++j)
		{
			if (false == Collider[i]->m_bIsRigid && false == Collider[j]->m_bIsRigid)
				continue;

			if (true == AABB(Collider[i], Collider[j]))
			{
				D3DXVECTOR3 vPenetration = D3DXVECTOR3(0.f, 0.f, 0.f);
				if (true == PolygonCollision(Collider[i], Collider[j], vPenetration))
				{
					Collider[i]->GetGameObject()->OnCollision(Collider[j]->GetGameObject());
					Collider[j]->GetGameObject()->OnCollision(Collider[i]->GetGameObject());
				}
			}

		}
	}
	Collider.clear();
}

bool CCollisionManager::AABB(CCollider * _pOrigin, CCollider * _pTarget)
{
	//Bounding Volume(Box) 를 AABB 검사에 이용해서 우선 검사.
	BOUNDINGBOX tOriginBox = _pOrigin->GetBound();
	BOUNDINGBOX	tTargetBox = _pTarget->GetBound();

	if (tOriginBox.vMax.x < tTargetBox.vMin.x || tOriginBox.vMin.x > tTargetBox.vMax.x)
		return false;
	if (tOriginBox.vMax.y < tTargetBox.vMin.y || tOriginBox.vMin.y > tTargetBox.vMax.y)
		return false;
	if (tOriginBox.vMax.z < tTargetBox.vMin.z || tOriginBox.vMin.z > tTargetBox.vMax.z)
		return false;
	return true;
	//위 의 검사에서 충돌되었다고 판단된 콜리더들만 메쉬를 사용해서 면충돌 검사.
}

bool CCollisionManager::LineCrossPlane(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane)
{
	float fSign1 = 0.f;
	float fSign2 = 0.f;

	fSign1 = _tPlane.a * _vLineStart.x + _tPlane.b * _vLineStart.y + _tPlane.c * _vLineStart.z + _tPlane.d;
	fSign2 = _tPlane.a * _vLineEnd.x + _tPlane.b * _vLineEnd.y + _tPlane.c * _vLineEnd.z + _tPlane.d;

	return ((fSign1 * fSign2 >= 0.f) ? false : true);
}

bool CCollisionManager::IntersectingPoint(OUT D3DXVECTOR3 & _vPoint, const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane)
{
	D3DXVECTOR3 vLineDir = _vLineEnd - _vLineStart;
	
	D3DXVec3Normalize(&vLineDir, &vLineDir);

	//직선위의 임의이 점과 직선의 시작점으로 부터의 거리.
	float fDistance = 0.f;	
	float fDenominator = 0.f;
	float fNumerator = 0.f;

	fDenominator = _tPlane.a * vLineDir.x + _tPlane.b * vLineDir.y + _tPlane.c * vLineDir.z;	
	if (fabs(0 - fDenominator) < FLT_EPSILON)
		return false;	
	fNumerator = -(_tPlane.a * _vLineStart.x + _tPlane.b * _vLineStart.y + _tPlane.c * _vLineStart.z + _tPlane.d);
	
	fDistance = fNumerator / fDenominator;

	if (fDistance < 0)
		return false;

	_vPoint = _vLineStart + (vLineDir * fDistance);
	return true;
}

bool CCollisionManager::PointInPolygon(const D3DXVECTOR3 _vPoint, const D3DXVECTOR3 _vPolyPointA, const D3DXVECTOR3 _vPolyPointB, const D3DXVECTOR3 _vPolyPointC)
{
	float fRadian = 0.f;
	float fDotResult = 0.f;
	D3DXVECTOR3 vPolyPoint[3] = { _vPolyPointA, _vPolyPointB,_vPolyPointC };

	D3DXVECTOR3 vOperandA = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vOperandB = D3DXVECTOR3(0.f, 0.f, 0.f);

	for (UINT i = 0; i < 3; ++i)
	{
		vOperandA = vPolyPoint[i] - _vPoint;
		vOperandB = vPolyPoint[((i + 1) % 3)] - _vPoint;

		D3DXVec3Normalize(&vOperandA, &vOperandA);
		D3DXVec3Normalize(&vOperandB, &vOperandB);

		fDotResult = D3DXVec3Dot(&vOperandA, &vOperandB);

		fRadian += acosf(fDotResult);
	}

	if (fRadian >= (D3DXToRadian(36.0f)))
		return true;

	return false;
}

bool CCollisionManager::LineIntersectPolygon(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXVECTOR3 * _pVertices, const UINT _nVertexCount, D3DXVECTOR3 & _vPenetration)
{
	D3DXPLANE tPlane;
	ZeroMemory(&tPlane, sizeof(D3DXPLANE));
	D3DXVECTOR3 vPoint = D3DXVECTOR3(0.f, 0.f, 0.f);

	for (UINT i = 0; i < _nVertexCount; i += 3)
	{
		D3DXPlaneFromPoints(&tPlane, &_pVertices[i], &_pVertices[i + 1], &_pVertices[i + 2]);

		if (false == LineCrossPlane(_vLineStart, _vLineEnd, tPlane))
			continue;

		if (false == IntersectingPoint(vPoint, _vLineStart, _vLineEnd, tPlane))
			continue;

		if (true == PointInPolygon(vPoint, _pVertices[i], _pVertices[i + 1], _pVertices[i + 2]))
		{
			D3DXVECTOR3 vToEnd = _vLineEnd - vPoint;
			D3DXVECTOR3 vToStart = _vLineStart - vPoint;
			D3DXVECTOR3 vPlaneNorm = D3DXVECTOR3(tPlane.a, tPlane.b, tPlane.c);

			if (D3DXVec3Dot(&vToStart, &vPlaneNorm) >= 0.f)
				_vPenetration = vToEnd;
			else
				_vPenetration = vToStart;
			return true;
		}

	}

	return false;
}

bool CCollisionManager::PolygonCollision(CCollider * _pOrigin, CCollider * _pTarget, D3DXVECTOR3& _vPenetration)
{
	D3DXVECTOR3* pOriginVertices = nullptr;
	D3DXVECTOR3* pTargetVertices = nullptr;
	UINT		 nOriginVertexCount = 0;
	UINT		 nTargetVertexCount = 0;

	const D3DXVECTOR3* pOriginPrototype = _pOrigin->GetVertices();
	const D3DXVECTOR3* pTargetPrototype = _pTarget->GetVertices();

	nOriginVertexCount = _pOrigin->GetVertexCount();
	pOriginVertices = new D3DXVECTOR3[nOriginVertexCount];
	nTargetVertexCount = _pTarget->GetVertexCount();
	pTargetVertices = new D3DXVECTOR3[nTargetVertexCount];

	D3DXMATRIX matI;
	D3DXMatrixIdentity(&matI);

	D3DXMATRIX matOriginWorld = ((CTransform*)_pOrigin->GetGameObject()->GetComponent<CTransform>())->Get_WorldMatrix();
	D3DXMATRIX matTargetWorld = ((CTransform*)_pTarget->GetGameObject()->GetComponent<CTransform>())->Get_WorldMatrix();

	if (matI == matOriginWorld)
	{
		memcpy(pOriginVertices, pOriginPrototype, nOriginVertexCount);
	}
	else
	{
		for (UINT i = 0; i < nOriginVertexCount; ++i)
		{
			D3DXVec3TransformCoord(&pOriginVertices[i], &pOriginPrototype[i], &matOriginWorld);
		}
	}

	if (matI == matTargetWorld)
	{
		memcpy(pTargetVertices, pTargetPrototype, nTargetVertexCount);
	}
	else
	{
		for (UINT i = 0; i < nTargetVertexCount; ++i)
		{
			D3DXVec3TransformCoord(&pTargetVertices[i], &pTargetPrototype[i], &matTargetWorld);
		}
	}

	D3DXVECTOR3 vLineStart = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vLineEnd = D3DXVECTOR3(0.f, 0.f, 0.f);
	UINT nStartIndex = 0;
	UINT nEndIndex = 0;

	for (UINT i = 0; i < nOriginVertexCount; ++i)
	{
		nStartIndex = i;
		nEndIndex = ((i / 3) * 3) + ((i + 1) % 3);
		vLineStart = pOriginVertices[nStartIndex];
		vLineEnd = pOriginVertices[nEndIndex];

		if (LineIntersectPolygon(vLineStart, vLineEnd, pTargetVertices, nTargetVertexCount, _vPenetration))
		{
			SafeDeleteArray(pOriginVertices);
			SafeDeleteArray(pTargetVertices);
			return true;
		}
	}

	for (UINT i = 0; i < nTargetVertexCount; ++i)
	{
		nStartIndex = i;
		nEndIndex = ((i / 3) * 3) + ((i + 1) % 3);
		vLineStart = pTargetVertices[nStartIndex];
		vLineEnd = pTargetVertices[nEndIndex];

		if (LineIntersectPolygon(vLineStart, vLineEnd, pOriginVertices, nOriginVertexCount, _vPenetration))
		{
			SafeDeleteArray(pOriginVertices);
			SafeDeleteArray(pTargetVertices);
			return true;
		}
	}

	SafeDeleteArray(pOriginVertices);
	SafeDeleteArray(pTargetVertices);
	return false;
}


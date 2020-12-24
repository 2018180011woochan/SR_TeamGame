#include "..\Header\CollisionManager.h"
#include "GameObject.h"
#include <iostream>

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
	D3DXVECTOR3 vPenetration;

	for (UINT i = 0; i < nSize; ++i)
	{
		for (UINT j = i + 1; j < nSize; ++j)
		{
			if (false == Collider[i]->m_bIsRigid && false == Collider[j]->m_bIsRigid)
				continue;

			BOUND tOrigin = Collider[i]->GetBound();
			BOUND tTarget = Collider[j]->GetBound();

			if (BOUND::BOUNDTYPE::BOX == tOrigin.eType &&BOUND::BOUNDTYPE::BOX == tTarget.eType)
				bCollision = AABBToAABB(tOrigin, tTarget);
			else if (BOUND::BOUNDTYPE::SPHERE == tOrigin.eType &&BOUND::BOUNDTYPE::SPHERE == tTarget.eType)
				bCollision = SphereToSphere(tOrigin, tTarget);
			else
			{
				if (BOUND::BOUNDTYPE::BOX == tOrigin.eType &&BOUND::BOUNDTYPE::SPHERE == tTarget.eType)
				{
					bCollision = AABBToSphere(tOrigin, tTarget, vPenetration);
					if (true == bCollision && true == Collider[j]->m_bIsRigid)
						((CTransform*)(Collider[j]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);
				}
				else
				{
					bCollision = AABBToSphere(tTarget, tOrigin, vPenetration);
					if (true == bCollision && true == Collider[i]->m_bIsRigid)
						((CTransform*)(Collider[i]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);

				}
			}

			if (true == bCollision)
			{
				Collider[i]->GetGameObject()->OnCollision((Collider[j]->GetGameObject()));
				Collider[j]->GetGameObject()->OnCollision((Collider[i]->GetGameObject()));
			}
		}
	}
	Collider.clear();
}

bool CCollisionManager::AABBToAABB(BOUND _tOriginBound, BOUND _tTargetBound)
{
	if (_tOriginBound.vMax.x < _tTargetBound.vMin.x || _tOriginBound.vMin.x > _tTargetBound.vMax.x)
		return false;
	if (_tOriginBound.vMax.y < _tTargetBound.vMin.y || _tOriginBound.vMin.y > _tTargetBound.vMax.y)
		return false;
	if (_tOriginBound.vMax.z < _tTargetBound.vMin.z || _tOriginBound.vMin.z > _tTargetBound.vMax.z)
		return false;
	return true;
}

bool CCollisionManager::AABBToSphere(BOUND _tAABB, BOUND _tSphere, D3DXVECTOR3& _vPenetration)
{
	D3DXVECTOR3 vBoxPoint;

	if (_tSphere.vCenter.x < _tAABB.vCenter.x - _tAABB.fLength * 0.5f)
		vBoxPoint.x = _tAABB.vCenter.x - _tAABB.fLength * 0.5f;
	else if (_tSphere.vCenter.x > _tAABB.vCenter.x + _tAABB.fLength * 0.5f)
		vBoxPoint.x = _tAABB.vCenter.x + _tAABB.fLength * 0.5f;
	else
		vBoxPoint.x = _tSphere.vCenter.x;

	if (_tSphere.vCenter.y < _tAABB.vCenter.y - _tAABB.fHeight * 0.5f)
		vBoxPoint.y = _tAABB.vCenter.y - _tAABB.fHeight * 0.5f;
	else if (_tSphere.vCenter.y > _tAABB.vCenter.y + _tAABB.fHeight * 0.5f)
		vBoxPoint.y = _tAABB.vCenter.y + _tAABB.fHeight * 0.5f;
	else
		vBoxPoint.y = _tSphere.vCenter.y;

	if (_tSphere.vCenter.z < _tAABB.vCenter.z - _tAABB.fDepth * 0.5f)
		vBoxPoint.z = _tAABB.vCenter.z - _tAABB.fDepth * 0.5f;
	else if (_tSphere.vCenter.z > _tAABB.vCenter.z + _tAABB.fDepth * 0.5f)
		vBoxPoint.z = _tAABB.vCenter.z + _tAABB.fDepth * 0.5f;
	else
		vBoxPoint.z = _tSphere.vCenter.z;

	D3DXVECTOR3 vCenterToBox = vBoxPoint - _tSphere.vCenter;
	float fLength = D3DXVec3Length(&vCenterToBox);

	if (fLength <= _tSphere.fRadius)
	{
		D3DXVECTOR3 vDir = -vCenterToBox;
		D3DXVec3Normalize(&vDir, &vDir);
		_vPenetration = vDir * (_tSphere.fRadius - fLength);
		return true;
	}

	return false;
}

bool CCollisionManager::SphereToSphere(BOUND _tOriginBound, BOUND _tTargetBound)
{
	D3DXVECTOR3 vCenterToCenter = _tTargetBound.vCenter - _tOriginBound.vCenter;
	float fLength = D3DXVec3Length(&vCenterToCenter);
	float fDiameter = _tOriginBound.fRadius + _tTargetBound.fRadius;

	if (fDiameter >= fLength)
	{
		cout << "SphereToSphere" << endl;
		return true;
	}
	return false;
}



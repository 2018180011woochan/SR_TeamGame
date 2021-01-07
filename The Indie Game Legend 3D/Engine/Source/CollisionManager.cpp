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
			{
				if (AABBToAABB(tOrigin, tTarget, vPenetration))
				{
					if (Collider[i]->m_bIsStand == false && Collider[j]->m_bIsStand)
					{
						((CTransform*)(Collider[i]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);
					}
					else if (Collider[i]->m_bIsStand  && Collider[j]->m_bIsStand == false)
					{
						AABBToAABB(tTarget, tOrigin, vPenetration);
						((CTransform*)(Collider[j]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);
					}
				}
			}
			else if (BOUND::BOUNDTYPE::SPHERE == tOrigin.eType &&BOUND::BOUNDTYPE::SPHERE == tTarget.eType)
				bCollision = SphereToSphere(tOrigin, tTarget);
			else
			{
				if (BOUND::BOUNDTYPE::BOX == tOrigin.eType &&BOUND::BOUNDTYPE::SPHERE == tTarget.eType)
				{
					bCollision = AABBToSphere(tOrigin, tTarget, vPenetration);
					if (true == bCollision && true == Collider[j]->m_bIsRigid)
					{
						((CTransform*)(Collider[j]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);
						//wcout << Collider[i]->GetGameObject()->GetName().c_str() << "  /  " << Collider[j]->GetGameObject()->GetName().c_str() << "  /  " << endl;
						//cout << vPenetration.x << " / " << vPenetration.y << " / " << vPenetration.z << " / " << endl;
					}
				}
				else
				{
					bCollision = AABBToSphere(tTarget, tOrigin, vPenetration);
					if (true == bCollision && true == Collider[i]->m_bIsRigid && true == Collider[j]->m_bIsRigid)
					{
						((CTransform*)(Collider[i]->GetGameObject()->GetComponent<CTransform>()))->Add_Position(vPenetration);
						//wcout << Collider[i]->GetGameObject()->GetName().c_str() << "  /  " << Collider[j]->GetGameObject()->GetName().c_str() << "  /  " << endl;
						//cout << vPenetration.x << " / " << vPenetration.y << " / " << vPenetration.z << " / " << endl;
					}

				}
			}

			if (true == bCollision)
			{
				Collider[i]->GetGameObject()->OnCollision((Collider[j]->GetGameObject()));
				Collider[j]->GetGameObject()->OnCollision((Collider[i]->GetGameObject()));
			}
		}
	}
}

bool CCollisionManager::AABBToAABB(BOUND _tOriginBound, BOUND _tTargetBound, D3DXVECTOR3& _vPenetration)
{

	if (_tOriginBound.vMax.x < _tTargetBound.vMin.x || _tOriginBound.vMin.x > _tTargetBound.vMax.x)
		return false;
	if (_tOriginBound.vMax.y < _tTargetBound.vMin.y || _tOriginBound.vMin.y > _tTargetBound.vMax.y)
		return false;
	if (_tOriginBound.vMax.z < _tTargetBound.vMin.z || _tOriginBound.vMin.z > _tTargetBound.vMax.z)
		return false;
	_vPenetration = _vector(0, 0, 0);
	float	fleft =		max(_tOriginBound.vMin.x, _tTargetBound.vMin.x);
	float	fright =	min(_tOriginBound.vMax.x, _tTargetBound.vMax.x);
	float	ftop  =		min(_tOriginBound.vMax.z, _tTargetBound.vMax.z);
	float	fbottom =	max(_tOriginBound.vMin.z, _tTargetBound.vMin.z);
	float	fW = fright - fleft;
	float	fH = ftop - fbottom;
	if (fW > fH)
	{
		if ((int)ftop == (int)_tTargetBound.vMax.z)
		{
			_vPenetration.z += fH;
		}
		else if ((int)fbottom == (int)_tTargetBound.vMin.z)
		{
			_vPenetration.z -= fH;
		}
	}
	else
	{
		if ((int)fright == (int)_tTargetBound.vMax.x)
		{
			_vPenetration.x += fW;
		}
		else if ((int)fleft == (int)_tTargetBound.vMin.x)
		{
			_vPenetration.x -= fW;
		}
	}
	return true;
}

bool CCollisionManager::AABBToSphere(BOUND _tAABB, BOUND _tSphere, D3DXVECTOR3& _vPenetration)
{
	D3DXVECTOR3 vBoxPoint;

	if (_tSphere.vCenter.x <= _tAABB.vCenter.x - _tAABB.fLength * 0.5f)
		vBoxPoint.x = _tAABB.vCenter.x - _tAABB.fLength * 0.5f;
	else if (_tSphere.vCenter.x >= _tAABB.vCenter.x + _tAABB.fLength * 0.5f)
		vBoxPoint.x = _tAABB.vCenter.x + _tAABB.fLength * 0.5f;
	else
		vBoxPoint.x = _tSphere.vCenter.x;

	if (_tSphere.vCenter.y <= _tAABB.vCenter.y - _tAABB.fHeight * 0.5f)
		vBoxPoint.y = _tAABB.vCenter.y - _tAABB.fHeight * 0.5f;
	else if (_tSphere.vCenter.y >= _tAABB.vCenter.y + _tAABB.fHeight * 0.5f)
		vBoxPoint.y = _tAABB.vCenter.y + _tAABB.fHeight * 0.5f;
	else
		vBoxPoint.y = _tSphere.vCenter.y;

	if (_tSphere.vCenter.z <= _tAABB.vCenter.z - _tAABB.fDepth * 0.5f)
		vBoxPoint.z = _tAABB.vCenter.z - _tAABB.fDepth * 0.5f;
	else if (_tSphere.vCenter.z >= _tAABB.vCenter.z + _tAABB.fDepth * 0.5f)
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
		//cout << "SphereToSphere" << endl;
		return true;
	}
	return false;
}



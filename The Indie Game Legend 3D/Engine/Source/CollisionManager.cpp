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
	//�Ľ� �ð� ����
	clock_t nStart = clock();

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
	int nCount = 0;
	for (UINT i = 0; i < nSize; ++i)
	{
		for (UINT j = i + 1; j < nSize; ++j)
		{
			if (false == Collider[i]->m_bIsRigid && false == Collider[j]->m_bIsRigid)
				continue;

			bCollision = AABB(Collider[i], Collider[j]);

			nCount++;
			if (false == bCollision)
				continue;
			Collider[i]->GetGameObject()->OnCollision(Collider[j]->GetGameObject());
			Collider[j]->GetGameObject()->OnCollision(Collider[i]->GetGameObject());
			//�ɼǿ� ���� �о�� �� GameObject�� �Լ� ȣ��.
			//����  : Collider[i]->GetGameObject()->OnCollision(Collider[j]->GetGameObject());
		}
	}

	
	clock_t nEnd = clock();
	int a = (1 + Collider.size()) * Collider.size() / 2;
	//cout << "Tile : " << nCount << "    /    Size() : " << Collider.size() <<"  For : "<< a << endl;

	Collider.clear();

}

bool CCollisionManager::AABB(CCollider * _pOrigin, CCollider * _pTarget)
{
	//Bounding Volume(Box) �� AABB �˻翡 �̿��ؼ� �켱 �˻�.
	BOUNDINGBOX tOriginBox = _pOrigin->GetBound();
	BOUNDINGBOX	tTargetBox = _pTarget->GetBound();

	if (tOriginBox.vMax.x < tTargetBox.vMin.x || tOriginBox.vMin.x > tTargetBox.vMax.x)
		return false;
	if (tOriginBox.vMax.y < tTargetBox.vMin.y || tOriginBox.vMin.y > tTargetBox.vMax.y)
		return false;
	if (tOriginBox.vMax.z < tTargetBox.vMin.z || tOriginBox.vMin.z > tTargetBox.vMax.z)
		return false;
	//�� �� �˻翡�� �浹�Ǿ��ٰ� �Ǵܵ� �ݸ����鸸 �޽��� ����ؼ� ���浹 �˻�.

	return true;
}


#include "..\Header\CollisionManager.h"
#include "GameObject.h"
#include "Collider.h"

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
	bool bCollision;
	for (UINT i = 0; i < nSize; ++i)
	{
		for (UINT j = i + 1; j < nSize; ++j)
		{
			bCollision = AABB(Collider[i], Collider[j]);
			if (false == bCollision)
				continue;

			//옵션에 따라 밀어내기 및 GameObject의 함수 호출.
			//예시  : Collider[i]->GetGameObject()->OnCollision(Collider[j]->GetGameObject());
		}
	}

}

bool CCollisionManager::AABB(CCollider * _pOrigin, CCollider * _pTarget)
{

	return true;
}


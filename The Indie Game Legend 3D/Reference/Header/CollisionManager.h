#pragma once
#ifndef __COLLISION_MANAGER_H__
#include "Base.h"
#include "Collider.h"
BEGIN(Engine)
class CGameObject;
class CCollisionManager final : public CBase
{
	DECLARE_SINGLETON(CCollisionManager)
private:
	explicit CCollisionManager();
	virtual ~CCollisionManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	void CollisionCheck(list<CGameObject*> _GameObjects);

private:
	bool AABBToAABB(BOUND _tOriginBound, BOUND _tTargetBound ,D3DXVECTOR3& _vPenetration);
	bool AABBToSphere(BOUND _tAABB, BOUND _tSphere, D3DXVECTOR3& _vPenetration);
	bool SphereToSphere(BOUND _tOriginBound, BOUND _tTargetBound);
};
END
#define __COLLISION_MANAGER_H__
#endif // !__COLLISION_MANAGER_H__

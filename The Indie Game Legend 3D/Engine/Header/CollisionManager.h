#pragma once
#ifndef __COLLISION_MANAGER_H__
#include "Base.h"
BEGIN(Engine)
class CCollider;
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
	bool AABB(CCollider* _pOrigin, CCollider* _pTarget);
};
END
#define __COLLISION_MANAGER_H__
#endif // !__COLLISION_MANAGER_H__

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

	//직선과 평면의 교차 판별.
	bool LineCrossPlane(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane);
	//직선과 교차하는 평면위의 점 도출.
	bool IntersectingPoint(OUT D3DXVECTOR3& _vPoint, const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane);
	////평면위의 점이 폴리곤 영역에 있는지 판별.
	//bool InPolygon(const D3DXVECTOR3 _vPoint, cons)
	//
	bool LineIntersectPolygon(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXVECTOR3* _pVertices, const UINT _nVertexCount, D3DXVECTOR3& _vPenetration);


	bool PolygonCollision(CCollider* _pOrigin, CCollider* _pTarget);
};
END
#define __COLLISION_MANAGER_H__
#endif // !__COLLISION_MANAGER_H__

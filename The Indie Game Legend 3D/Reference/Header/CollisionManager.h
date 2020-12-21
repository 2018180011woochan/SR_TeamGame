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
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;

public:
	void CollisionCheck(list<CGameObject*> _GameObjects);

private:
	bool AABB(CCollider* _pOrigin, CCollider* _pTarget);

	//������ ����� ���� �Ǻ�.
	bool LineCrossPlane(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane);
	//������ �����ϴ� ������� �� ����.
	bool IntersectingPoint(OUT D3DXVECTOR3& _vPoint, const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXPLANE _tPlane);
	////������� ���� ������ ������ �ִ��� �Ǻ�.
	//bool InPolygon(const D3DXVECTOR3 _vPoint, cons)
	//
	bool LineIntersectPolygon(const D3DXVECTOR3 _vLineStart, const D3DXVECTOR3 _vLineEnd, const D3DXVECTOR3* _pVertices, const UINT _nVertexCount, D3DXVECTOR3& _vPenetration);


	bool PolygonCollision(CCollider* _pOrigin, CCollider* _pTarget);
};
END
#define __COLLISION_MANAGER_H__
#endif // !__COLLISION_MANAGER_H__

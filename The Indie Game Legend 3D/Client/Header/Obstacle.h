#pragma once
#ifndef __OBSTACLE_H__
#include "GameObject.h"
USING(Engine)
class CObstacle abstract : public CGameObject
{
protected:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CCollider*	 m_pCollider = nullptr;
public:
	explicit CObstacle();
	explicit CObstacle(const CObstacle& _rOther);
	virtual ~CObstacle() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;
protected:
	virtual void Free() PURE;
};
#define __OBSTACLE_H__
#endif

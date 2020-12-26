#pragma once
#ifndef __FLOOR_H__
#include "GameObject.h"
USING(Engine)
class CFloor : public CGameObject
{
private:
	CCollider* m_pCollider;
private:
	explicit CFloor();
	explicit CFloor(const CFloor& _rOther);
	virtual ~CFloor() = default;
	// CBase을(를) 통해 상속됨
	virtual void	Free() override;
public:
	static CFloor*	Create();
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};
#define __FLOOR_H__
#endif // !__FLOOR_H__

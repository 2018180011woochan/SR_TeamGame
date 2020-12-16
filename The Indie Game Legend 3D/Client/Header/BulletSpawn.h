#pragma once
#ifndef __BULLETSPAWN_H__
#include "Gizmo.h"
USING(Engine)
class CBulletSpawn final : public CGizmo  
{
public:
	explicit CBulletSpawn();
	explicit CBulletSpawn(const CBulletSpawn& _rOther);
	virtual ~CBulletSpawn() = default;

	// CGizmo을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
public:
	static CBulletSpawn* Create();
private:
	virtual void Free() override;
	HRESULT UpdatePos(const float _fDeltaTime);
};
#define __BULLETSPAWN_H__
#endif

#pragma once
#ifndef __PLAYERSPAWN_H__
#include "Gizmo.h"
USING(Engine)
class CPlayerSpawn final : public CGizmo
{
public:
	explicit CPlayerSpawn();
	explicit CPlayerSpawn(const CPlayerSpawn& _rOther);
	virtual ~CPlayerSpawn() = default;

	// CGizmo을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

public:
	static CPlayerSpawn* Create();
private:
	void Free() override;

};

#define __PLAYERSPAWN_H__
#endif
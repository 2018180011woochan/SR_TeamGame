#pragma once
#ifndef __PIRAMIDUNBREAKE_H__
#include "Obstacle.h"
class CPiramidUnBrake final	: public CObstacle
{
private:
	bool		m_bDead = false;

public:
	explicit CPiramidUnBrake();
	explicit CPiramidUnBrake(const CPiramidUnBrake& _rOther);
	virtual ~CPiramidUnBrake() = default;

	// CObstacle을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	virtual void OnCollision(CGameObject* _pGameObject) override;

public:
	static CPiramidUnBrake* Create();
	virtual void Free() override;
};

#define __PIRAMIDUNBREAKE_H__
#endif

#pragma once
#ifndef __GATE_H__
#include "Obstacle.h"
class CGate : public CObstacle
{
public:
	explicit CGate();
	explicit CGate(const CGate& _rOther);
	virtual ~CGate() = default;

	// CObstacle을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	static CGate* Create();
private:
	virtual void Free() override;
};
#define __GATE_H__
#endif

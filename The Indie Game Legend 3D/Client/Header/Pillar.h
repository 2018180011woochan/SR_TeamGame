#pragma once
#ifndef __PILLAR_H__
#include "Obstacle.h"
USING(Engine)
class CPillar final : public CObstacle
{
public:
	explicit CPillar();
	explicit CPillar(const CPillar& _rOther);
	virtual ~CPillar() =default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	static CPillar* Create();
private:
	virtual void Free() override;
};

#define __PILLAR_H__
#endif // !__PILLAR_H__
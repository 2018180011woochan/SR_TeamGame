#pragma once
#ifndef __SLIDEBLOCK_H__

#include "Obstacle.h"
class CSlideBlock final : public CObstacle
{
public:
	explicit CSlideBlock();
	explicit CSlideBlock(const CSlideBlock& _rOther);
	virtual ~CSlideBlock();

	// CObstacle을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	static CSlideBlock* Create();
private:
	virtual void Free() override;
};

#define __SLIDEBLOCK_H__
#endif // !__SLIDEBLOCK_H__

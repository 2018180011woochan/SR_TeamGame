#pragma once
#ifndef __WORM_TAIL_H__
#include "WormPart.h"
USING(Engine)
class CWormTail final : public CWormPart
{
private:
	explicit CWormTail();
	explicit CWormTail(const CWormTail& _rOther);
	virtual ~CWormTail() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWormTail* Create();
	// CGameObject을(를) 통해 상속됨
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
#define __WORM_TAIL_H__
#endif // !__WORM_TAIL_H__

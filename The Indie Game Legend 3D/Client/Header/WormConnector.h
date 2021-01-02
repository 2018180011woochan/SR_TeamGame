#pragma once
#ifndef __WORM_CONNECTOR_H__
#include "WormPart.h"
USING(Engine)
class CWormConnector final : public CWormPart
{
private:
	explicit CWormConnector();
	explicit CWormConnector(const CWormConnector& _rOther);
	virtual ~CWormConnector() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWormConnector* Create();
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
#define __WORM_CONNECTOR_H__
#endif // !__WORM_CONNECTOR_H__

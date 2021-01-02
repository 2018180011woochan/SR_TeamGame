#pragma once
#ifndef __CSWITCH_H__
#include "InteractionObj.h"

class CSwitch final : public CInteractionObj
{
private:
	bool			m_bPress;
private:
	explicit CSwitch();
	explicit CSwitch(const CSwitch& _rOther);
	virtual ~CSwitch() =default;

public:
	// CInteractionObj을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	// CInteractionObj을(를) 통해 상속됨
	virtual void OnCollision(CGameObject * _pGameObject) override;
	virtual void OnNotify() override;
	virtual CGameObject * Clone() override;
	static CSwitch* Create();
private:
	virtual void Free() override;
public:
	void SwitchUp();
	void SwitchDown();
};

#define  __CSWITCH_H__
#endif

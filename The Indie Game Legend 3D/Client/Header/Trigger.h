#pragma once
#ifndef __TRIGGER_H__
#include "GameObject.h"
USING(Engine)
class CTrigger abstract : public CGameObject
{
protected:
	CCollider* m_pCollider = nullptr;

protected:
	explicit CTrigger();
	explicit CTrigger(const CTrigger& _rOther);
	virtual ~CTrigger() =default ;

public:
	//»ó¼Ó
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;

	virtual void OnCollision(CGameObject* _pGameObject) PURE;
	virtual void Free() override;
};
#define __TRIGGER_H__
#endif

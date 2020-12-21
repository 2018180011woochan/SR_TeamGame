#pragma once
#ifndef __ROOMTRIGGER_H__
#include "Trigger.h"
class CRoomTrigger final : public CTrigger
{
public:
	explicit CRoomTrigger();
	explicit CRoomTrigger(const CRoomTrigger& _rOther);
	virtual ~CRoomTrigger() = default;

	// CTrigger을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	virtual void OnCollision(CGameObject * _pGameObject) override;

public:
	//Test
	CMeshRenderer* m_pMeshRenderer = nullptr;

	static CRoomTrigger* Create();
	virtual void Free() override;
};
#define  __ROOMTRIGGER_H__
#endif

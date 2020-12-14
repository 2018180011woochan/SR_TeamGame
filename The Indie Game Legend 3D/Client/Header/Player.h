#pragma once
#ifndef __PLAYER_H__
#include "GameObject.h"
USING(Engine)
class CPlayer : public CGameObject
{
private:
	 CTransform*		m_pTransform;
	 class CKeyManager* m_pKeyMgr;
	 float				m_fMoveSpeed = 0.f;
private:
	HRESULT Key_Input(const float _fDeltaTime);

protected:
	explicit CPlayer();
	explicit CPlayer(const CPlayer& _rOther);
public:
	virtual ~CPlayer() = default;
public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
public:
	static CPlayer* Create();
	virtual void Free() override;
};
#define __PLAYER_H__
#endif

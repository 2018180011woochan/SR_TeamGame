#pragma once
#ifndef __BATTLE_SHIP_H__
#include "GameObject.h"
USING(Engine)
class CKeyManager;
class CTexturePool;
class CBattleShip final : public CGameObject
{
private:
	CKeyManager*		m_pKeyManager;
	
	CMeshRenderer*		m_pMeshRenderer;

	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	D3DXVECTOR3			m_vMoveDir;
	float				m_fMoveSpeed;

	float				m_fFireTime;
	float				m_fFireInterval;

	D3DXVECTOR3			m_vBulletOffset;
private:
	explicit CBattleShip();
	explicit CBattleShip(const CBattleShip& _rOther);
	virtual ~CBattleShip() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CBattleShip* Create();
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

private:
	void	Input(const float _fDeltaTime);
	void	Move(const float _fDeltaTime);
	void	Animate(const float _fDeltaTime);
	void	Fire(const float _fDeltaTime);
};
#define __BATTLE_SHIP_H__
#endif // !__BATTLE_SHIP_H__

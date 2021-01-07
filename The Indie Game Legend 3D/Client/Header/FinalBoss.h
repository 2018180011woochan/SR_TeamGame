#pragma once
#ifndef __FINAL_BOSS_H__
#include "Monster.h"
USING(Engine)
class CTexturePool;
class CBattleShip;
class CFinalBoss final : public CMonster
{
private:
	enum PATTERN { IDLE, NORMAL, PATTERN_END};
private:
	CBattleShip*	m_pPlayer;
	CMeshRenderer*	m_pMeshRenderer;
	
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	PATTERN			m_ePattern;

	float			m_fNormalInterval;
	float			m_fCreateTime;

	int				m_nIndex;
	int				m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;

	bool			m_bLoop;
	bool			m_bReverse;

	D3DXVECTOR3		m_vMoveDirection;
	float			m_fMoveSpeed;

	UINT			m_nViaIndex;
	D3DXVECTOR3		m_vVia[3];
	D3DXVECTOR3		m_vViaStart;

	bool			m_bTest;
private:
	explicit CFinalBoss();
	explicit CFinalBoss(const CFinalBoss& _rOther);
	virtual ~CFinalBoss() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFinalBoss* Create();
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
	void Animate(const float _fDeltaTime);

	void Move(const float _fDeltaTime);

	void SetTextureKey(const TSTRING _sTextureKey);

	void DoPattern(const float _fDeltaTime);
	void SetPattern(PATTERN _ePattern);

	void Idle(const float _fDeltaTime);
	void Normal(const float _fDeltaTime);

	void ExplosionLaser(const float _fDeltaTime);
	void InputTest(const float _fDeltaTime);
};
#define __FINAL_BOSS_H__
#endif // !__FINAL_BOSS_H__

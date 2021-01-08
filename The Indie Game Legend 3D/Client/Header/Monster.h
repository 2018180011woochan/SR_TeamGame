#pragma once
#ifndef __MONSTER_H__
#include "GameObject.h"
#include "SoundMgr.h"
#include "Bullet.h"
USING(Engine)
class CBossHP;
class CMonster : public CGameObject
{
public:
	explicit CMonster();
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT IsBillboarding();
	//  [1/4/2021 wades]
	void	sfxMetalHit();
public:
	//Getter
	bool	GetDead() { return m_bDead; };
	virtual CGameObject * Clone() = 0;
	virtual void OnCollision(CGameObject* _pGameObject) override;
	// �ʱ� ��ġ ����
	virtual void	SetMonsterPos();
	// �ø� ������ ��ġ �ʱ�ȭ
	virtual void	ResetMonsterPos();
	virtual void	OnEnable() override;
	//  [1/7/2021 wades]
	void AddHp(const _int& _nHp);
protected:
	const CTransform*	m_pPlayerTransform;
	class CVIBuffer*	m_pVIBufferCom = nullptr;
	class CTexture*		m_pTextureCom = nullptr;
	CCollider*			m_pCollider;
	_vector				m_vecInitPos;
	//wjm test
	bool				m_bDead = false;
	int					m_iHP;
	int					m_iMaxHP;
	_vector				m_vecStartPos;
	CBossHP*			m_pBossHP;

	bool				m_bHit;
	float				m_fHitDelay;
	float				m_fHitDelayTime;

};

#define __MONSTER_H__
#endif // !__MONSTER_H__


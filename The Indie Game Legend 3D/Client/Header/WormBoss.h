#pragma once
#ifndef __WORMBOSS_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWormBossBody;
class CWormBoss final : public CMonster
{
private:
	explicit CWormBoss();
	explicit CWormBoss(const CWormBoss& other);
	virtual ~CWormBoss() = default;

	enum State {LEFT, RIGHT, UP, DOWN, IDLE};

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
private:
	HRESULT Movement(float fDeltaTime);
	void Waving(float fDeltaTime);
	void ChaseAttack(float fDeltaTime);

public:
	virtual CGameObject * Clone() override;
	static CWormBoss* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CWormBossBody* pWormBody;




	float		   m_fFrameSpeed;
	float		   m_fFrameDeltaTime;
	
	float			m_fMoveSpeed;

	_vector vRightDir;
	_vector vLeftDir;
	State				m_eCurDirState;
	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;
	bool				m_bDashAttack;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	int					m_iChaseTime;

	UINT nIndex;
};

#define __WORMBOSS_H__
#endif // !__WORMBOSS_H__


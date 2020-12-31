#pragma once
#ifndef __WORMBOSSTAIL_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWormBoss;
class CWormBossTail final : public CMonster
{
private:
	explicit CWormBossTail();
	explicit CWormBossTail(const CWormBossTail& other);
	virtual ~CWormBossTail() = default;

	enum State { LEFT, RIGHT, UP, DOWN, IDLE };
	enum Pos {UPPOS, DOWNPOS, SAME};

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

public:
	void Set_BodyPos(const _vector _vPos, UINT _nBodyIndex);

public:
	virtual CGameObject * Clone() override;
	static CWormBossTail* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CTransform*	   m_pWormBossTransform;
	CWormBoss*	   m_pWormBoss;

	float		   m_fFrameSpeed;
	float		   m_fFrameDeltaTime;
	
	float			m_fMoveSpeed;

	float			m_fHeadJumpPower;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	State				m_eCurDirState;
	Pos					m_eCurPos;
	_vector vRightDir;
	_vector vLeftDir;

	UINT nIndex;
	UINT nBodyIndex;
};

#define __WORMBOSSTAIL_H__
#endif // !__WORMBOSSTAIL_H__


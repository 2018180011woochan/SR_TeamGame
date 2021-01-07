#pragma once
#ifndef __GREENBOYDOWNHAND_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CGreenBoyDownHand final : public CMonster
{
private:
	explicit CGreenBoyDownHand();
	explicit CGreenBoyDownHand(const CGreenBoyDownHand& other);
	virtual ~CGreenBoyDownHand() = default;

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
	void Jumping(float fDeltaTime);
	bool isCloseToPlayer();
	void BulletFire();
	void Crush(float fDeltaTime);

public:
	void SetPos(const _vector _Pos);
	void SetIsAttack(const bool _bAttack);
	void SetIsCrush(const bool _bAttack);
	void SetBossDead(const bool _isBossDead);

public:
	virtual CGameObject * Clone() override;
	static CGreenBoyDownHand* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CTransform* pBodyTransform;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fFireSpeed;
	float				m_fFireDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;
	bool				m_bisAttack;
	bool				m_bCrush;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	bool				m_bIsBossDead;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __GREENBOYDOWNHAND_H__
#endif // !__GREENBOYDOWNHAND_H__

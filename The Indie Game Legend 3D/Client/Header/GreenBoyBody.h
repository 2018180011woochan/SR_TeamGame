#pragma once
#ifndef __GREENBOYBODY_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CGreenBoyRightHand;
class CGreenBoyLeftHand;
class CGreenBoyUpHand;
class CGreenBoyDownHand;
class CGreenBoyHead;
class CGreenBoyFace;
class CFlame;
class CGreenBoyBody final : public CMonster
{
private:
	explicit CGreenBoyBody();
	explicit CGreenBoyBody(const CGreenBoyBody& other);
	virtual ~CGreenBoyBody() = default;

	enum Attack {LEFTBEEM, RIGHTBEEM, DOUBLEBEEM, BEEMEND};

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
	
public:
	void SetPos(const _vector _Pos);
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual CGameObject * Clone() override;
	static CGreenBoyBody* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CGreenBoyRightHand* pRightHand;
	CGreenBoyLeftHand* pLeftHand;
	CGreenBoyUpHand*  pUpHand;
	CGreenBoyDownHand*  pDownHand;
	CGreenBoyHead* pHead;
	CGreenBoyFace* pFace;
	CFlame* pFlame;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	float				m_fAttackSpeed;
	float				m_fAttackDeltaTime;

	bool				m_bIsAttack;
	Attack				m_eAttackBeem;
	UINT nIndex;

	bool m_bIsTPS;
};

#define __GREENBOYBODY_H__
#endif // !__GREENBOYBODY_H__

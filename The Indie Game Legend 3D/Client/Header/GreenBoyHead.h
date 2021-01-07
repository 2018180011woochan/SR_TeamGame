#pragma once
#ifndef __GREENBOYHEAD_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CGreenBoyHead final : public CMonster
{
private:
	explicit CGreenBoyHead();
	explicit CGreenBoyHead(const CGreenBoyHead& other);
	virtual ~CGreenBoyHead() = default;

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
	void SetEggPos(const _vector _EggPos);
	void SetBossDead(const bool _isBossDead);

public:
	virtual CGameObject * Clone() override;
	static CGreenBoyHead* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	bool				m_bIsBossDead;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __GREENBOYHEAD_H__
#endif // !__GREENBOYHEAD_H__

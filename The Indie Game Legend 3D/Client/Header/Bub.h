#pragma once
#ifndef __BUB_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBub final : public CMonster
{
private:
	explicit CBub();
	explicit CBub(const CBub& other);
	virtual ~CBub() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	//Test
	virtual void OnCollision(CGameObject* _pGameObject) override;


private:
	HRESULT Movement(float fDeltaTime);
	void Jumping(float fDeltaTime);

public:
	void SetEggPos(const _vector _EggPos);

public:
	virtual CGameObject * Clone() override;
	static CBub* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*	m_pCollider;

	float				m_fJumpingCnt = 0;
	float				m_fJumpSpeed = 5;
	float				m_fMaxJump = 5.f;
	float				m_fMoveSpeed = 5.f;
	bool				m_isMaxJump = false;
	bool				m_isJumping = false;

	UINT nIndex = 0;

	bool m_bIsTPS;
};

#define __BUB_H__
#endif // !__BUB_H__

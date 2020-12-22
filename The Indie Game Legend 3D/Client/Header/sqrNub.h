#pragma once
#ifndef __SQRNUB_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CsqrNub final : public CMonster
{
private:
	explicit CsqrNub();
	explicit CsqrNub(const CsqrNub& other);
	virtual ~CsqrNub() = default;

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

public:
	void SetEggPos(const _vector _EggPos);

public:
	virtual CGameObject * Clone() override;
	static CsqrNub* Create();
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

	UINT nIndex;

	bool m_bIsTPS;
};

#define __SQRNUB_H__
#endif // !__SQRNUB_H__

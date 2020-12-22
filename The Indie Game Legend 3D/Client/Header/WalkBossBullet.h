#pragma once
#ifndef __WALKBOSSBULLET_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWalkBossBullet final : public CGameObject
{
private:
	explicit CWalkBossBullet();
	explicit CWalkBossBullet(const CWalkBossBullet& other);
	virtual ~CWalkBossBullet() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Movement(float fDeltaTime);
	void Jumping(float fDeltaTime);

public:
	void SetWalkBossPos(const _vector _TurretPos);

public:
	HRESULT IsBillboarding();

public:
	virtual CGameObject * Clone() override;
	static CWalkBossBullet* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;
	CCollider*			m_pCollider;
	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;
	D3DXVECTOR3				m_vecDir;
	float					m_fBulletSpeed;
	UINT nIndex;

	bool m_bIsTPS;
};

#define __WALKBOSSBULLET_H__
#endif // !__WALKBOSSBULLET_H__

#pragma once
#ifndef __WAKERBOSS_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWalkerBoss final : public CMonster
{
private:
	explicit CWalkerBoss();
	explicit CWalkerBoss(const CWalkerBoss& other);
	virtual ~CWalkerBoss() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void BulletFire();

private:
	HRESULT Movement(float fDeltaTime);
	void Dash(float _fDeltaTime);
	bool isCloseToPlayer();

public:
	virtual CGameObject * Clone() override;
	static CWalkerBoss* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer*	m_pMeshRenderer = nullptr;
	CTexturePool*	m_pTexturePool;
	float			m_fFireSpeed;
	float			m_fFireDeltaTime;
	float			m_fWalkSpeed;
	float			m_fWalkDeltaTime;

	float			m_fLightningSpeed;
	float			m_FLightningDeltaTime;

	bool			m_bisStop;
	bool			m_bisDash;
	int				iCnt = 0;
	int				iDashCnt = 0;


	float			m_fMoveSpeed;
	UINT			nIndex = 0;
};

#define __WAKERBOSS_H__
#endif // !__WAKERBOSS_H__


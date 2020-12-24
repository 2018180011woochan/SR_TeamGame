#pragma once
#ifndef __PHUBANS_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CPhubans final : public CMonster
{
private:
	explicit CPhubans();
	explicit CPhubans(const CPhubans& other);
	virtual ~CPhubans() = default;

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
	void Dash(float _fDeltaTime);
	bool isCloseToPlayer();

public:
	virtual CGameObject * Clone() override;
	static CPhubans* Create();
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

#define __PHUBANS_H__
#endif // !__PHUBANS_H__


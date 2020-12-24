#pragma once
#ifndef __WALKERBULLET_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CWalkerBullet : public CGameObject
{
private:
	explicit CWalkerBullet();
	explicit CWalkerBullet(const CWalkerBullet& other);
	virtual ~CWalkerBullet() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	virtual CGameObject * Clone() override;
	static CWalkerBullet* Create();

private:
	HRESULT Movement(float fDeltaTime);

public:
	void SetTurretPos(const _vector _TurretPos);

public:
	HRESULT IsBillboarding();

private:
	virtual void Free() override;

private:
	CMeshRenderer*			m_pMeshRenderer = nullptr;
	CTexturePool*			m_pTexturePool;
	D3DXVECTOR3				m_vecDir;
	CCollider*				m_pCollider;

	UINT					nIndex = 0;
	float					m_fBulletSpeed;
};

#define __WALKERBULLET_H__
#endif // !__WALKERBULLET_H__


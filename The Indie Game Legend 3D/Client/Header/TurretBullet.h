#pragma once
#ifndef __TURRETBULLET_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CTurretBullet : public CGameObject
{
private:
	explicit CTurretBullet();
	explicit CTurretBullet(const CTurretBullet& other);
	virtual ~CTurretBullet() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual void OnCollision(CGameObject* _pGameObject) override;
public:
	virtual CGameObject * Clone() override;
	static CTurretBullet* Create();

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
	bool					m_bDead;
};

#define __TURRETBULLET_H__
#endif // !__TURRETBULLET_H__


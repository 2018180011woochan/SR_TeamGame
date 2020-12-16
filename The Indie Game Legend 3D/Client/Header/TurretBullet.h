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

public:
	virtual CGameObject * Clone() override;
	static CTurretBullet* Create();

private:
	HRESULT Movement(float fDeltaTime);

public:
	void SetTurretPos(const _vector _TurretPos) { m_vecTurretPos = _TurretPos; }

private:
	virtual void Free() override;

private:
	CMeshRenderer*			m_pMeshRenderer = nullptr;
	CTexturePool*			m_pTexturePool;
		
	_vector					m_vecTurretPos;
	CTransform*				m_vecPlayerTransform;
	UINT					nIndex = 0;
	float					m_fBulletSpeed = 5.f;
};

#define __TURRETBULLET_H__
#endif // !__TURRETBULLET_H__


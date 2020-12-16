#pragma once
#ifndef __TURRET_H__
#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CTurret final : public CMonster
{
private:
	explicit CTurret();
	explicit CTurret(const CTurret& other);
	virtual ~CTurret() = default;

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

public:
	virtual CGameObject * Clone() override;
	static CTurret* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	DWORD		   m_dwFireTime;
	float		   m_fFireSpeed;
	float		   m_fFireDeltaTime;
};

#define __TURRET_H__
#endif // !__TURRET_H__


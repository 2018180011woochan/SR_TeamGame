#pragma once
#ifndef __PLYAERBULLET_H__
#include "Bullet.h"
USING(Engine)
class CPlayerBullet final : public CBullet
{
private:
	EWeaponType		m_eBulletType;
private:
	explicit CPlayerBullet();
	explicit CPlayerBullet(const CPlayerBullet& _rOther);
	virtual ~CPlayerBullet() = default;

public:
	void Set_Type(const EWeaponType& _eBulletType);
public:
	// CBullet을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
	virtual HRESULT Fire() override;
private:
public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	static  CPlayerBullet* Create();

	virtual void OnCollision(CGameObject * _pGameObject) override;
private:
	virtual void Free();

};
#define __PLYAERBULLET_H__
#endif

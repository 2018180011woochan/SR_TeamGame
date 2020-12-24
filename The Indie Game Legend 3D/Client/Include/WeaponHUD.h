#pragma once
#ifndef __WEAPON_HUD_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CWeaponHUD final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nMaxFrame;
private:
	explicit CWeaponHUD();
	explicit CWeaponHUD(const CWeaponHUD& _rOther);
	virtual ~CWeaponHUD() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CWeaponHUD* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject* Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	void ChangeWeapon(const UINT _nWeaponID);
};
#define __WEAPON_HUD_H__
#endif // !__WEAPON_HUD_H__

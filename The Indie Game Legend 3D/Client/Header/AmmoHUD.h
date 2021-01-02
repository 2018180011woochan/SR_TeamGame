#pragma once
#ifndef __AMMO_HUD_H__
#include "GameObject.h"
USING(Engine)
class CAmmoFrame;
class CAmmoGauge;
class CAmmoIcon;
class CSkillGauge;
class CAmmoIconHighlight;
class CAmmoHUD final : public CGameObject
{
private:
	CAmmoIconHighlight*	m_pAmmoIconHighlight;
	CAmmoFrame*			m_pAmmoFrame;
	CAmmoGauge*			m_pAmmoGauge;
	CAmmoIcon*			m_pAmmoIcon;
	CSkillGauge*		m_pSkillGauge;
private:
	explicit CAmmoHUD();
	explicit CAmmoHUD(const CAmmoHUD& _rOther);
	virtual ~CAmmoHUD() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CAmmoHUD* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void SetAmmoLevel(const UINT _nLevel);
	void SetAmmoCount(const float _fCount, const float _fMaxCount);
	void SetAmmoIcon(const UINT _nWeaponID);
	void SetSkillGauge(const float _fValue);
	void SetActive(const bool _bActive);
};
#define __AMMO_HUD_H__
#endif // !__AMMO_HUD_H__

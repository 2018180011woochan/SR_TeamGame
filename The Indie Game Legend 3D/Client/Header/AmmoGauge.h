#pragma once
#ifndef __AMMO_GAUGE_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CAmmoGauge final : public CGameObject
{
private:
	CTexturePool* m_pTexturePool;
	Image*		  m_pImage = nullptr;
	TSTRING		  m_sTextureKey;
private:
	explicit CAmmoGauge();
	explicit CAmmoGauge(const CAmmoGauge& _rOther);
	virtual ~CAmmoGauge() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CAmmoGauge*	Create();
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
	void SetAmmoCount(const float _fFillAmount);
};
#define __AMMO_GAUGE_H__
#endif // !__AMMO_GAUGE_H__

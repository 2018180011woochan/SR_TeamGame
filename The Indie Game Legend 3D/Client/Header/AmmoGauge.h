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
	UINT		  m_nIndex;
private:
	explicit CAmmoGauge();
	explicit CAmmoGauge(const CAmmoGauge& _rOther);
	virtual ~CAmmoGauge() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void			Free() override;
public:
	static CAmmoGauge*	Create();
	virtual CGameObject * Clone() override;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void SetAmmoLevel(const UINT _nLevel);
	void SetAmmoCount(const float _fFillAmount);
	void SetActive(const bool _bActive);
};
#define __AMMO_GAUGE_H__
#endif // !__AMMO_GAUGE_H__

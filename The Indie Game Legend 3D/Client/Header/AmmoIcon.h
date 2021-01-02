#pragma once
#ifndef __AMMO_ICON_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CAmmoIcon final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nMaxFrame;
private:
	explicit CAmmoIcon();
	explicit CAmmoIcon(const CAmmoIcon& _rOther);
	virtual ~CAmmoIcon() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CAmmoIcon* Create();
	// CGameObject��(��) ���� ��ӵ�
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
	void SetAmmoIcon(const UINT _nWeaponID);
};
#define __AMMO_ICON_H__
#endif // !__AMMO_ICON_H__

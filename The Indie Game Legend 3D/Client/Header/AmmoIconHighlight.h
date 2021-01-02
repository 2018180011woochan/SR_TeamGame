#pragma once
#ifndef __AMMO_ICON_HIGHLIGHT_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CAmmoIconHighlight : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
private:
	explicit CAmmoIconHighlight();
	explicit CAmmoIconHighlight(const CAmmoIconHighlight& _rOther);
	virtual ~CAmmoIconHighlight() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CAmmoIconHighlight* Create();
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
};
#define __AMMO_ICON_HIGHLIGHT_H__
#endif // !__AMMO_ICON_HIGHLIGHT_H__

#pragma once
#ifndef __AMMO_FRAME_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CAmmoFrame final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
private:
	explicit CAmmoFrame();
	explicit CAmmoFrame(const CAmmoFrame& _rOther);
	virtual ~CAmmoFrame() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void			Free() override;
public:
	static	CAmmoFrame* Create();
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
#define __AMMO_FRAME_H__
#endif // !__AMMO_FRAME_H__

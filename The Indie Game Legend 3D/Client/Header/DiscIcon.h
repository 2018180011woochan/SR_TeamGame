#pragma once
#ifndef __DISC_ICON_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CDiscIcon final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
	UINT			m_nIndex;
	UINT			m_nMaxFrame;
	Image*			m_pImage;
	float			m_fTime;
	float			m_fAnimationSpeed;
private:
	explicit CDiscIcon();
	explicit CDiscIcon(const CDiscIcon& _rOther);
	virtual ~CDiscIcon() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void			Free() override;
public:
	static CDiscIcon* Create();
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
#define __DISC_ICON_H__
#endif // !__DISC_ICON_H__

#pragma once
#ifndef __DIALOG_TEXT_ARROW_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CDialogTextArrow final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;
private:
	explicit CDialogTextArrow();
	explicit CDialogTextArrow(const CDialogTextArrow& _rOther);
	virtual ~CDialogTextArrow() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CDialogTextArrow* Create();
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
	void SetArrow(const D3DXVECTOR3 _vPosition);
private:
	void AnimateArrow(const float _fDeltaTime);
};
#define __DIALOG_TEXT_ARROW_H__
#endif // !__DIALOG_TEXT_ARROW_H__

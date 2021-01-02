#pragma once
#ifndef __FOCUS_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CFocus final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;

	float			m_fTime;
	float			m_fAnimationSpeed;

	float			m_fMaxScale;
	float			m_fMinScale;
	float			m_fLerpAmount;
	float			m_fLerpSpeed;
	bool			m_bIncrease;
public:
	explicit CFocus();
	explicit CFocus(const CFocus& _rOther);
	virtual ~CFocus() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	static CFocus* Create();
public:
	void SetFocusPos(const _vector& _vPos);

private:
	void LerpScale();
	virtual void			Free() override;

};
#define __FOCUS_H__
#endif

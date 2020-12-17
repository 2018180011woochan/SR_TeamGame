#pragma once
#ifndef __CROSSHAIR_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CCrossHair final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;

	float			m_fMaxScale;
	float			m_fMinScale;
	float			m_fLerpAmount;
	float			m_fLerpSpeed;
	bool			m_bIncrease;
private:
	explicit CCrossHair();
	explicit CCrossHair(const CCrossHair& _rOther);
	virtual ~CCrossHair() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CCrossHair* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	void LerpScale();
};
#define __CROSSHAIR_H__
#endif // !__CROSSHAIR_H__

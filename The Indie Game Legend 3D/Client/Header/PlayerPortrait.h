#pragma once
#ifndef __PLAYER_PORTRAIT_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CPlayerPortrait final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;

	bool			m_bPlaying;

private:
	explicit CPlayerPortrait();
	explicit CPlayerPortrait(const CPlayerPortrait& _rOther);
	virtual ~CPlayerPortrait() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CPlayerPortrait* Create();
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
	void OnAnimation(bool _bPlaying);

private:
	void AnimatePortrait(const float _fDeltaTime);
};
#define __PLAYER_PORTRAIT_H__
#endif // !__PLAYER_PORTRAIT_H__


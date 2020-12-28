#pragma once
#ifndef __NPC_PORTRAIT_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CNPCPortrait final : public CGameObject
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
	explicit CNPCPortrait();
	explicit CNPCPortrait(const CNPCPortrait& _rOther);
	virtual ~CNPCPortrait() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CNPCPortrait* Create();
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
	void SetPortrait(const TSTRING& _sPortrait);
private:
	void AnimatePortrait(const float _fDeltaTime);
};
#define __NPC_PORTRAIT_H__
#endif // !__NPC_PORTRAIT_H__

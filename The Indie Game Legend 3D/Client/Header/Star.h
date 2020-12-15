#pragma once
#ifndef __STAR_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CStar final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;

	UINT			m_nIndex;
	UINT			m_nMaxFrame;

	float			m_fTime;
	float			m_fAnimationSpeed;
private:
	explicit CStar();
	explicit CStar(const CStar& _rOther);
	virtual ~CStar() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static	CStar*			Create();
	virtual CGameObject*	Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};

#define __STAR_H__
#endif // !__STAR_H__

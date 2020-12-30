#pragma once
#ifndef __LOADING_BACKGROUND_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CLoadingBackGround final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	Image*			m_pImage;
private:
	explicit CLoadingBackGround();
	explicit CLoadingBackGround(const CLoadingBackGround& _rOther);
	virtual ~CLoadingBackGround() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CLoadingBackGround* Create();
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
};
#define __LOADING_BACKGROUND_H__
#endif // !__LOADING_BACKGROUND_H__

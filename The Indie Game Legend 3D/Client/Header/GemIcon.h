#pragma once
#ifndef __GEM_ICON_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CGemIcon final : public CGameObject
{
private:
	Image* m_pImage;
	TSTRING m_sTextureKey;
	UINT m_nIndex;
	UINT m_nMaxFrame;
	CTexturePool* m_pTexturePool;

	float			m_fTime;
	float			m_fAnimationSpeed;
private:
	explicit CGemIcon();
	explicit CGemIcon(const CGemIcon& _rOther);
	virtual ~CGemIcon() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CGemIcon* Create();
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
#define __GEM_ICON_H__
#endif // !__GEM_ICON_H__

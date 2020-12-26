#pragma once
#ifndef __MINIMAP_PARTIAL_H__
#include "GameObject.h"
class CMiniMapRenderer;
USING(Engine)
class CMiniMapPartial final : public CGameObject
{
private:
	CMiniMapRenderer* m_pMiniMapRenderer;
	Image*	m_pImage;
private:
	explicit CMiniMapPartial();
	explicit CMiniMapPartial(const CMiniMapPartial& _rOther);
	virtual ~CMiniMapPartial() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CMiniMapPartial* Create();
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
#define __MINIMAP_PARTIAL_H__
#endif // !__MINIMAP_PARTIAL_H__

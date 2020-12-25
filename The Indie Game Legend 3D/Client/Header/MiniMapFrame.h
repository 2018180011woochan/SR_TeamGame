#pragma once
#ifndef __MINIMAP_FRAME_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CMiniMapFrame final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
private:
	explicit CMiniMapFrame();
	explicit CMiniMapFrame(const CMiniMapFrame& _rOther);
	virtual ~CMiniMapFrame() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CMiniMapFrame* Create();
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
#define __MINIMAP_FRAME_H__
#endif // !__MINIMAP_FRAME_H__

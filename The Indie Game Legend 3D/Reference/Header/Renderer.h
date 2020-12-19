#pragma once
#ifndef __RENDERER_H__
#include "Base.h"
BEGIN(Engine)
class CGameObject;
class CRenderer final : public CBase
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	/*맴버 추가*/
	typedef list<class CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects[(_uint)ERenderID::End];

	GAMEOBJECTS m_ColliderObjects;
	bool m_bDrawCollider;
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CRenderer() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

private:
	HRESULT RenderPriority();
	HRESULT	RenderNoAlpha();
	HRESULT RenderAlpha();
	HRESULT RenderUI();
	HRESULT RenderCollider();
public:
	HRESULT AddInRenderer(const ERenderID _eRenderID, CGameObject* const _pGameObject);
	HRESULT Render(HWND _hWnd = NULL);
	static CRenderer* Create(LPDIRECT3DDEVICE9 _pDevice);
};
END
#define __RENDERER_H__
#endif // !__RENDERER_H__

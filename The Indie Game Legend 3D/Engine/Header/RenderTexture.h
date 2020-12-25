#pragma once
#ifndef __RENDERTEXTURE_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CRenderTexture final : public CComponent
{
private:
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DSURFACE9	m_pSurface;
	LPDIRECT3DSURFACE9	m_pZBuffer;
	D3DSURFACE_DESC		m_tSurfaceDesc;

	LPDIRECT3DSURFACE9	m_pOrigSurface;
	LPDIRECT3DSURFACE9	m_pOrigZBuffer;
private:
	explicit CRenderTexture(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CRenderTexture() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CRenderTexture* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
};
END
#define __RENDERTEXTURE_H__
#endif // !__RENDERTEXTURE_H__

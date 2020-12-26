#pragma once
#ifndef __RENDERTEXTURE_H__
#include "Texture.h"
BEGIN(Engine)
class ENGINE_DLL CRenderTexture final : public CTexture
{
private:
	LPDIRECT3DSURFACE9	m_pSurface;
	LPDIRECT3DSURFACE9	m_pZBuffer;
	D3DSURFACE_DESC		m_tSurfaceDesc;

	LPDIRECT3DSURFACE9	m_pOrigSurface;
	LPDIRECT3DSURFACE9	m_pOrigZBuffer;

	LPD3DXSPRITE		m_pSprite;

private:
	explicit CRenderTexture();
	virtual ~CRenderTexture() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CRenderTexture* Create();
public:
	HRESULT Initialize(const UINT _nCX, const UINT _nCY);
	HRESULT DrawOnRenderTexture(LPDIRECT3DTEXTURE9 _pTexture, const RECT * _pRect, const D3DXVECTOR3 & _vCenter, const D3DXMATRIX& _matWorld, const bool _bClear = true);
};
END
#define __RENDERTEXTURE_H__
#endif // !__RENDERTEXTURE_H__

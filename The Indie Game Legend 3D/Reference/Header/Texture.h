#pragma once
#ifndef __TEXTURE_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CTexture final : public CBase
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_tImageInfo;
private:
	explicit CTexture();
	virtual ~CTexture() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	static CTexture* Create();
	HRESULT Load(const TSTRING& _sFilePath);

	LPDIRECT3DTEXTURE9	GetTexture();
	D3DXIMAGE_INFO		GetDesc();
};
END
#define __TEXTURE_H__
#endif // !__TEXTURE_H__

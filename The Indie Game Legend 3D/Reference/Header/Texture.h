#pragma once
#ifndef __TEXTURE_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CTexture : public CBase
{
protected:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DXIMAGE_INFO		m_tImageInfo;
protected:
	explicit CTexture();
	virtual ~CTexture() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	static CTexture* Create();
	HRESULT Load(const TSTRING& _sFilePath);

	virtual LPDIRECT3DTEXTURE9	GetTexture();
	virtual D3DXIMAGE_INFO		GetDesc();
};
END
#define __TEXTURE_H__
#endif // !__TEXTURE_H__

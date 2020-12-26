#pragma once
#ifndef __RAWIMAGE_H__
#include "Component.h"
BEGIN(Engine)
class CTexture;
class ENGINE_DLL CRawImage final : public CComponent
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;
	UINT					m_nVertexCount;
	UINT					m_nIndexCount;
	PIVOT					m_tPivot;
	CTexture*	m_pTexture;
	float		m_fWidth;
	float		m_fHeight;
	D3DXVECTOR2 m_vUV;

private:
	explicit CRawImage(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CRawImage() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CRawImage* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;

	HRESULT SetTexture(CTexture* const _pTexture);

	void	SetWidth(UINT _nWidth);
	void	SetHeight(UINT _nHeight);
	void	SetUV(D3DXVECTOR2 _vUV);

private:
	HRESULT CreateBuffer();

};
END
#define __RAWIMAGE_H__
#endif // !__RAWIMAGE_H__

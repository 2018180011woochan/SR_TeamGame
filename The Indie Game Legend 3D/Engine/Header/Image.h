#pragma once
#ifndef __IMAGE_H__
#include "Component.h"
BEGIN(Engine)
class CTexture;
class ENGINE_DLL Image final : public CComponent
{
public:
	enum ImageType { Simple, Fill};
	enum FillMethod { Horizontal};
	enum FillOrigin { Left, Right};
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;
	UINT					m_nVertexCount;
	UINT					m_nIndexCount;
	
	CTexture*				m_pTexture;

	UINT					m_nScreenWidth;
	UINT					m_nScreenHeight;

	ImageType				m_eImageType;
	FillMethod				m_eFillMethod;
	FillOrigin				m_eFillOrigin;
	float					m_fFillAmount;

	PIVOT					m_tPivot;

	float					m_fWidth;
	float					m_fHeight;

	D3DXVECTOR2				m_vOffset;
	D3DXVECTOR2				m_vTiling;

	D3DXMATERIAL			m_tMaterial;
private:
	explicit Image(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~Image() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static Image* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	HRESULT Render();

	HRESULT SetTexture(CTexture* const _pTexture);
	//설정된 텍스쳐의 크기로 정점 재배치.
	HRESULT SetNativeSize();

	HRESULT SetImageType(const ImageType _eImageType);
	HRESULT SetFillAmount(const float _fValue);

	//
	void	SetPivot(const float _fX, const float _fY);
	void	SetOffset(const D3DXVECTOR2 _vOffset);
	void	SetTiling(const D3DXVECTOR2 _vTiling);

	void	SetWidth(const float _fWidth);
	void	SetHeight(const float _fHeihgt);
private:
	HRESULT CreateBuffer();
	HRESULT UpdateBuffer();
	HRESULT FillHorizontal();
};
END
#define __IMAGE_H__
#endif // !__IMAGE_H__

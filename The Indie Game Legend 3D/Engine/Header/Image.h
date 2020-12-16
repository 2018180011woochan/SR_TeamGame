#pragma once
#ifndef __IMAGE_H__
#include "Component.h"
BEGIN(Engine)
class CTexture;
typedef struct tagVertexRect
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 UV;
	static const DWORD FVF;
}VERTEXRECT, *LPVERTEXRECT;
typedef struct tagIndex16
{
	WORD _1;
	WORD _2;
	WORD _3;
}INDEX16, *LPINDEX16;
typedef struct tagPivot
{
	float fX;
	float fY;
}PIVOT, *LPPIVOT;
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
private:
	HRESULT CreateBuffer();

	HRESULT FillHorizontal();
};
END
#define __IMAGE_H__
#endif // !__IMAGE_H__

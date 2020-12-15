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

class ENGINE_DLL Image final : public CComponent
{
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;
	UINT					m_nVertexCount;
	UINT					m_nIndexCount;
	
	CTexture*				m_pTexture;

	UINT					m_nScreenWidth;
	UINT					m_nScreenHeight;

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
private:
	HRESULT CreateBuffer();
	//텍스처 변경에 따른 버퍼 업데이트.
	HRESULT UpdateBuffer();
};
END
#define __IMAGE_H__
#endif // !__IMAGE_H__

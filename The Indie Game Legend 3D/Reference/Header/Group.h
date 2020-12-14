#pragma once
#ifndef __GROUP_H__
#include "Base.h"
#include "Texture.h"
BEGIN(Engine)

typedef struct tagVertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
	static const DWORD FVF;
}VERTEX;

typedef struct tagMaterial
{
	D3DMATERIAL9		tMaterial;
	CTexture*			pTexture;
}MATERIAL;

class CGroup final : public CBase
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	MATERIAL				m_tMaterial;
	UINT					m_nFaceCount;
	LPDIRECT3DDEVICE9		m_pDevice;
private:
	explicit CGroup(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, const UINT _nFaceCount);
	explicit CGroup(const CGroup& _rOther);
	virtual ~CGroup() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	HRESULT Render();
	
	static CGroup* Create(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, const UINT _nFaceCount);
	CGroup*	Clone();
	
	HRESULT SetTexture(CTexture* const _pTexture);	
};
END
#define __GROUP_H__
#endif // !__GROUP_H__

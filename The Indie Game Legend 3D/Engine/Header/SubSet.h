#pragma once
#ifndef __SUBSET_H__
#include "Base.h"
#include "Texture.h"
BEGIN(Engine)

typedef struct tagMaterial
{
	D3DMATERIAL9		tMaterial;
	CTexture*			pTexture;
}MATERIAL;

class CSubSet final : public CBase
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	MATERIAL				m_tMaterial;
	UINT					m_nStartVertex;
	UINT					m_nFaceCount;
private:
	explicit CSubSet(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, const UINT _nStartVertex, const UINT _nFaceCount);
	explicit CSubSet(const CSubSet& _rOther);
	virtual ~CSubSet() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CSubSet* Create(LPDIRECT3DDEVICE9 const _pDevice, const MATERIAL _tMaterial, const UINT _nStartVertex, const UINT _nFaceCount);
	CSubSet* Clone();

public:
	HRESULT DrawSubSet();

public:
	HRESULT SetTexture(CTexture* const _pTexture);
	HRESULT SetMaterial(const D3DMATERIAL9 _tMaterial);
	D3DMATERIAL9 GetMaterial();
};
END
#define __SUBSET_H__
#endif // !__SUBSET_H__

#pragma once
#ifndef __RECTTEXTURE_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
public:
	explicit CVIBuffer_Rect(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& _rOther);
public:
	virtual ~CVIBuffer_Rect()= default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT RenderVIBuffer() override;
public:
	static CVIBuffer_Rect* Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual void Free() override;

};
END
#define __RECTTEXTURE_H__
#endif

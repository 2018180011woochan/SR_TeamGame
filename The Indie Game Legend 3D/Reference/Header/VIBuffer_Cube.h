#pragma once
#ifndef __CUBETEXTURE_H__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	explicit CVIBuffer_Cube(const CVIBuffer_Cube& _rOther);
public:
	virtual ~CVIBuffer_Cube() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT RenderVIBuffer() override;

public:
	static CVIBuffer_Cube* Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual void Free() override;
};
END
#define __CUBETEXTURE_H__
#endif

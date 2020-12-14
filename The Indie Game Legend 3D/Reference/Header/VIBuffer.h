#pragma once
#ifndef __VIBUFFER_H__
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	_uint m_iVertexSize = 0;
	_uint m_iVertexCount = 0;
	_uint m_iTriCount = 0;
	_uint m_iFVF = 0;

	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
	_uint		m_iIndexSize = 0;
	D3DFORMAT	m_IndexFormat;

	//버텍스 정보 보관용
	void*	m_pVertices = nullptr;
	_bool	m_IsClone = false;

	_vector	m_vRayPivot;
	_vector m_vRayDirection;

//---------------------------
protected:
	explicit CVIBuffer(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	explicit CVIBuffer(const CVIBuffer& _rOther);
public:
	virtual ~CVIBuffer() = default;

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() PURE;
	virtual void Free() override;
public:
	virtual HRESULT RenderVIBuffer();
	virtual _bool IsRayPicking(HWND _hwnd,_uint _iWinCX, _uint _iWinCY,
		const CAMERA_DESC& _rCameraDecs, const _matrix* _pWorldMatrix, _vector* _pOut = nullptr);
};
END
#define __VIBUFFER_H__
#endif

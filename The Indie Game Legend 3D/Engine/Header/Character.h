#pragma once
#ifndef __CHARACTER_H__
#include "Base.h"
BEGIN(Engine)
class CCharacter : public CBase
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuffer;
	UINT					m_nWidth;
	UINT					m_nHeight;
private:
	explicit CCharacter(LPDIRECT3DDEVICE9 const _pDevice, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, LPDIRECT3DINDEXBUFFER9 const _pIndexBuffer, const UINT _nWidth, const UINT _nHeight);
	explicit CCharacter(const CCharacter& _rOther);
	virtual ~CCharacter() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CCharacter* Create(LPDIRECT3DDEVICE9 const _pDevice, LPDIRECT3DVERTEXBUFFER9 const _pVertexBuffer, LPDIRECT3DINDEXBUFFER9 const _pIndexBuffer, const UINT _nWidth, const UINT _nHeight);
	CCharacter* Clone();
public:
	HRESULT Render();

public:
	UINT	GetWidth();
	UINT	GetHeight();
};
END
#define __CHARACTER_H__
#endif // !__CHARACTER_H__

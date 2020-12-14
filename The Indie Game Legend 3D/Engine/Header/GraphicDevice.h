#pragma once
#ifndef __GRAPHICDEVICE_H__
#include "Base.h"
BEGIN(Engine)
class CGraphicDevice final: public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)
private:
	LPDIRECT3D9			m_pD3D9;
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	explicit CGraphicDevice();
	virtual ~CGraphicDevice() = default;
public:
	HRESULT Initialize(const HWND _hWnd, const UINT _nWinCX, const UINT _nWinCY, const EDisplayMode _eDisplayMode);

	// CBase을(를) 통해 상속됨
	virtual void Free() override;

	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
};
END
#define __GRAPHICDEVICE_H__
#endif // !__GRAPHICDEVICE_H__

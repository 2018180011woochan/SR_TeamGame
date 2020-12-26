#pragma once
#ifndef __TEXT_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CText final : public CComponent
{
private:
	TSTRING m_sFont;
	UINT	m_nSize;
private:
	explicit CText(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CText() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CText* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
};
END
#define __TEXT_H__
#endif // !__TEXT_H__

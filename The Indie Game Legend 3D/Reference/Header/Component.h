#pragma once
#ifndef __COMPONENT_H__
#include "Base.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CComponent abstract : public CBase
{
private:
	CGameObject*		m_pGameObject;
protected:
	LPDIRECT3DDEVICE9	m_pDevice;
protected:
	explicit CComponent(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CComponent() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	virtual HRESULT Initialize() PURE;
	CGameObject*	GetGameObject() { return m_pGameObject; }
};
END
#define __COMPONENT_H__
#endif // !__COMPONENT_H__
#include "..\Header\Component.h"
#include "GameObject.h"
USING(Engine)

CComponent::CComponent(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: m_pGameObject(_pGameObject)
	, m_pDevice(_pDevice)
{
	//SafeAddRef(m_pGameObject);
	SafeAddRef(m_pDevice);
}

void CComponent::Free()
{
	//SafeRelease(m_pGameObject);
	SafeRelease(m_pDevice);
}



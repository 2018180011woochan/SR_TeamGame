#include "..\Header\Text.h"

USING(Engine)

CText::CText(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_nSize(1)
	, m_sFont(TEXT(""))
{
}

void CText::Free()
{
	CComponent::Free();
}

CText * CText::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CText* pInstance = new CText(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

HRESULT CText::Initialize()
{
	return S_OK;
}

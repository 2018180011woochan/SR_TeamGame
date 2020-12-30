#include "stdafx.h"
#include "..\Header\LoadingText.h"
#include "TexturePoolManager.h"

CLoadingText::CLoadingText()
{
	m_pTransform = (CTransform*)AddComponent<CTransform>();
}

CLoadingText::CLoadingText(const CLoadingText & _rOther)
	: CGameObject(_rOther)
{
}

void CLoadingText::Free()
{
	CGameObject::Free();
}

CLoadingText * CLoadingText::Create()
{
	CLoadingText* pInstance = new CLoadingText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CLoadingText::Clone()
{
	CLoadingText* pClone = new CLoadingText(*this);
	return pClone;
}

HRESULT CLoadingText::InitializePrototype()
{
	return S_OK;
}

HRESULT CLoadingText::Awake()
{
	m_pText = (CText*)AddComponent<CText>();
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, -310.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

HRESULT CLoadingText::Start()
{
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetHorizon(CText::HORIZON::CENTER);
	m_pText->SetText(TEXT("LOADING"));
	m_pText->SetSize(2.f);
	m_pText->SetWidth(550.f);
	m_pText->SetHeight(66.f);
	return S_OK;
}

UINT CLoadingText::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CLoadingText::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CLoadingText::Render()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}

void CLoadingText::SetText(const TSTRING & _sText)
{
	m_pText->SetText(_sText);
}

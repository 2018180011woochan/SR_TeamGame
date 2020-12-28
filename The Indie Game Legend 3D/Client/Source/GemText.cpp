#include "stdafx.h"
#include "..\Header\GemText.h"


CGemText::CGemText()
	: m_pText(nullptr)
	, m_nCount(0)
{
}

CGemText::CGemText(const CGemText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
	, m_nCount(0)
{
}

void CGemText::Free()
{
	CGameObject::Free();
}

CGemText * CGemText::Create()
{
	CGemText* pInstance = new CGemText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CGemText::Clone()
{
	CGemText* pClone = new CGemText(*this);
	return pClone;
}

HRESULT CGemText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CGemText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)(AddComponent<CText>());
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CGemText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetText(TEXT("000"));
	m_pText->SetHorizon(CText::HORIZON::RIGHT);
	m_pText->SetSize(2.5f);
	m_pText->SetWidth(95.f);
	m_pText->SetHeight(35.f);
	m_pTransform->Set_Position(D3DXVECTOR3(580.f, 305.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CGemText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CGemText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CGemText::Render()
{
	CGameObject::Render();
	m_pText->Render();
	return S_OK;
}

void CGemText::SetCount(const UINT _nCount)
{
	m_nCount = _nCount;

	TCHAR szBuf[128] = TEXT("");
	_stprintf_s(szBuf, 128, TEXT("%03d"), m_nCount);
	m_pText->SetText(szBuf);	
}


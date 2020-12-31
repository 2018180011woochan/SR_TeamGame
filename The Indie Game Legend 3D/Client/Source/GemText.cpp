#include "stdafx.h"
#include "..\Header\GemText.h"


CGemText::CGemText()
	: m_pText(nullptr)
{
}

CGemText::CGemText(const CGemText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
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
	m_pText->SetText(TEXT(""));
	m_pTransform->Set_Position(D3DXVECTOR3(-200.f, 0.f, 0.f));
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


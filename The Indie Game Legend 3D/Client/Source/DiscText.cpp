#include "stdafx.h"
#include "..\Header\DiscText.h"


CDiscText::CDiscText()
	: m_pText(nullptr)

{
}

CDiscText::CDiscText(const CDiscText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
{
}


HRESULT CDiscText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDiscText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)(AddComponent<CText>());
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDiscText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetText(TEXT(""));
	//m_pTransform->Set_Position(D3DXVECTOR3(-200.f, 0.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CDiscText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return OBJ_NOENVET;
}

UINT CDiscText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;

}

HRESULT CDiscText::Render()
{
	CGameObject::Render();
	m_pText->Render();
	return S_OK;
}

CGameObject * CDiscText::Clone()
{
	CGameObject* pClone = new CDiscText(*this);
	return pClone;
}

void CDiscText::Free()
{
	CGameObject::Free();
}

CDiscText * CDiscText::Create()
{
	CDiscText* pInstance = new CDiscText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

#include "stdafx.h"
#include "..\Header\BossText.h"


CBossText::CBossText()
	: m_pText(nullptr)
{
}

CBossText::CBossText(const CBossText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
{
}

void CBossText::Free()
{
	CGameObject::Free();
}

CBossText * CBossText::Create()
{
	CBossText* pInstance = new CBossText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBossText::Clone()
{
	CBossText* pClone = new CBossText(*this);
	return pClone;
}

HRESULT CBossText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBossText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CBossText::Start()
{
	CGameObject::Start();

	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetText(TEXT("BOSS"));
	m_pText->SetHorizon(CText::HORIZON::LEFT);
	m_pText->SetSize(3);
	m_pText->SetWidth(160.f);
	m_pText->SetHeight(50.f);

	m_pTransform->Set_Position(D3DXVECTOR3(-127.f, 302.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CBossText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CBossText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CBossText::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}


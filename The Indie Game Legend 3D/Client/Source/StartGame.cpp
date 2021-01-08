#include "stdafx.h"
#include "..\Header\StartGame.h"


CStartGame::CStartGame()
{
}

CStartGame::CStartGame(const CStartGame & _rOther)
	: CGameObject(_rOther)
{
}

void CStartGame::Free()
{
	CGameObject::Free();
}

CStartGame * CStartGame::Create()
{
	CStartGame* pInstance = new CStartGame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CStartGame::Clone()
{
	CStartGame* pClone = new CStartGame(*this);
	return pClone;
}

HRESULT CStartGame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CStartGame::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, -125.f, 0.f));
	m_pTransform->UpdateTransform();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CStartGame::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetHorizon(CText::HORIZON::CENTER);
	m_pText->SetText(TEXT("START GAME"));
	m_pText->SetSize(2.f);
	m_pText->SetWidth(550.f);
	m_pText->SetHeight(66.f);
	return S_OK;
}

UINT CStartGame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CStartGame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CStartGame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}



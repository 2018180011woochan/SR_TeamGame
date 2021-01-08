#include "stdafx.h"
#include "..\Header\ExitGame.h"


CExitGame::CExitGame()
{
}

CExitGame::CExitGame(const CExitGame & _rOther)
	: CGameObject(_rOther)
{
}

void CExitGame::Free()
{
	CGameObject::Free();
}

CExitGame * CExitGame::Create()
{
	CExitGame* pInstance = new CExitGame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CExitGame::Clone()
{
	CExitGame* pClone = new CExitGame(*this);
	return pClone;
}

HRESULT CExitGame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CExitGame::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, -175.f, 0.f));
	m_pTransform->UpdateTransform();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CExitGame::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetHorizon(CText::HORIZON::CENTER);
	m_pText->SetText(TEXT("EXIT"));
	m_pText->SetSize(2.f);
	m_pText->SetWidth(550.f);
	m_pText->SetHeight(66.f);
	return S_OK;
}

UINT CExitGame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CExitGame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CExitGame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}



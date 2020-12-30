#include "stdafx.h"
#include "..\Header\DialogHUD.h"
#include "DialogFrame.h"
#include "DialogPortrait.h"
#include "DialogText.h"
#include "DialogTextArrow.h"

CDialogHUD::CDialogHUD()
	: m_pDialogFrame(nullptr)
	, m_pDialogPortrait(nullptr)
	, m_pDialogText(nullptr)
	, m_pDialogTextArrow(nullptr)
	, m_vPortraitPos{ D3DXVECTOR3(471.f, 189.f, 0.f), D3DXVECTOR3(-309.f, 189.f, 0.f) }
	, m_vTextPos{ D3DXVECTOR3(-37.f, 147.f, 0.f), D3DXVECTOR3(81.f, 147.f, 0.f) }
	, m_vArrowPos{ D3DXVECTOR3(265.f, 68.f, 0.f), D3DXVECTOR3(425.f, 68.f, 0.f) }
{
}

CDialogHUD::CDialogHUD(const CDialogHUD & _rOther)
	: CGameObject(_rOther)
	, m_pDialogFrame(nullptr)
	, m_pDialogPortrait(nullptr)
	, m_pDialogText(nullptr)
	, m_pDialogTextArrow(nullptr)
	, m_vPortraitPos{ D3DXVECTOR3(471.f, 189.f, 0.f), D3DXVECTOR3(-309.f, 189.f, 0.f) }
	, m_vTextPos{ D3DXVECTOR3(-37.f, 147.f, 0.f), D3DXVECTOR3(81.f, 147.f, 0.f) }
	, m_vArrowPos{ D3DXVECTOR3(265.f, 68.f, 0.f), D3DXVECTOR3(425.f, 68.f, 0.f) }
{
}

void CDialogHUD::Free()
{
	CGameObject::Free();
	SafeRelease(m_pDialogFrame);
	SafeRelease(m_pDialogPortrait);
	SafeRelease(m_pDialogText);
	SafeRelease(m_pDialogTextArrow);
}

CDialogHUD * CDialogHUD::Create()
{
	CDialogHUD* pInstance = new CDialogHUD;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDialogHUD::Clone()
{
	CDialogHUD* pClone = new CDialogHUD(*this);
	return pClone;
}

HRESULT CDialogHUD::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDialogHUD::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CDialogHUD::Start()
{
	CGameObject::Start();
	m_pDialogFrame = (CDialogFrame*)FindGameObjectOfType<CDialogFrame>();
	m_pDialogPortrait = (CDialogPortrait*)FindGameObjectOfType<CDialogPortrait>();
	m_pDialogText = (CDialogText*)FindGameObjectOfType<CDialogText>();
	m_pDialogTextArrow = (CDialogTextArrow*)FindGameObjectOfType<CDialogTextArrow>();
	SafeAddRef(m_pDialogFrame);
	SafeAddRef(m_pDialogPortrait);
	SafeAddRef(m_pDialogText);
	SafeAddRef(m_pDialogTextArrow);
	return S_OK;
}

UINT CDialogHUD::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	if (true == m_pDialogText->IsTypingEnd())
		m_pDialogPortrait->OnAnimation(false);

	if (GetAsyncKeyState('2') & 0x8000)
	{
		SetDialog(SPEAKER::PLAYER, TEXT("Looks like you found the \nrapid shot."), TEXT("Player"));
	}
	else if (GetAsyncKeyState('3') & 0x8000)
	{
		SetDialog(SPEAKER::NPC, TEXT("Looks like you found the \nrapid shot."), TEXT("Bean"));
	}
	return 0;
}

UINT CDialogHUD::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CDialogHUD::Render()
{
	return S_OK;
}

void CDialogHUD::OnEnable()
{
	if (nullptr != m_pDialogFrame)
		m_pDialogFrame->SetEnable(true);
	if (nullptr != m_pDialogPortrait)
		m_pDialogPortrait->SetEnable(true);
	if (nullptr != m_pDialogText)
		m_pDialogText->SetEnable(true);
	if (nullptr != m_pDialogTextArrow)
		m_pDialogTextArrow->SetEnable(true);
}

void CDialogHUD::OnDisable()
{
	if (nullptr != m_pDialogFrame)
		m_pDialogFrame->SetEnable(false);
	if (nullptr != m_pDialogPortrait)
		m_pDialogPortrait->SetEnable(false);
	if (nullptr != m_pDialogText)
		m_pDialogText->SetEnable(false);
	if (nullptr != m_pDialogTextArrow)
		m_pDialogTextArrow->SetEnable(false);
}

void CDialogHUD::SetDialog(const SPEAKER _eSpeaker, const TSTRING _sText, const TSTRING _sPortrait)
{
	//if (false == m_bEnable)
		SetEnable(true);

	m_pDialogPortrait->SetPortrait(_sPortrait, m_vPortraitPos[UINT(_eSpeaker)]);
	m_pDialogText->SetText(_sText, m_vTextPos[UINT(_eSpeaker)]);
	m_pDialogTextArrow->SetArrow(m_vArrowPos[UINT(_eSpeaker)]);
}



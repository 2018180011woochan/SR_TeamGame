#include "stdafx.h"
#include "..\Header\NPCDialog.h"
#include "DialogFrame.h"
#include "NPCPortrait.h"
#include "NPCText.h"

CNPCDialog::CNPCDialog()
	: m_pDialogFrame(nullptr)
	, m_pNPCPortrait(nullptr)
	, m_pNPCText(nullptr)
{
}

CNPCDialog::CNPCDialog(const CNPCDialog & _rOther)
	: CGameObject(_rOther)
	, m_pDialogFrame(nullptr)
	, m_pNPCPortrait(nullptr)
	, m_pNPCText(nullptr)
{
}

void CNPCDialog::Free()
{
	CGameObject::Free();
	SafeRelease(m_pDialogFrame);
	SafeRelease(m_pNPCPortrait);
	SafeRelease(m_pNPCText);
}

CNPCDialog * CNPCDialog::Create()
{
	CNPCDialog* pInstance = new CNPCDialog;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CNPCDialog::Clone()
{
	CNPCDialog* pClone = new CNPCDialog(*this);
	return pClone;
}

HRESULT CNPCDialog::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CNPCDialog::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CNPCDialog::Start()
{
	CGameObject::Start();
	m_pDialogFrame = (CDialogFrame*)(FindGameObjectOfType<CDialogFrame>());
	m_pNPCText = (CNPCText*)(FindGameObjectOfType<CNPCText>());
	m_pNPCPortrait = (CNPCPortrait*)(FindGameObjectOfType<CNPCPortrait>());

	SafeAddRef(m_pDialogFrame);
	SafeAddRef(m_pNPCText);
	SafeAddRef(m_pNPCPortrait);
	return S_OK;
}

UINT CNPCDialog::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_pNPCText->IsTypingEnd())
		m_pNPCPortrait->OnAnimation(false);
	return 0;
}

UINT CNPCDialog::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CNPCDialog::Render()
{
	return S_OK;
}

void CNPCDialog::OnEnable()
{
	if (nullptr != m_pDialogFrame)
		m_pDialogFrame->SetEnable(true);
	if (nullptr != m_pNPCPortrait)
		m_pNPCPortrait->SetEnable(true);
	if (nullptr != m_pNPCText)
		m_pNPCText->SetEnable(true);
}

void CNPCDialog::OnDisable()
{
	if (nullptr != m_pDialogFrame)
		m_pDialogFrame->SetEnable(false);
	if (nullptr != m_pNPCPortrait)
		m_pNPCPortrait->SetEnable(false);
	if (nullptr != m_pNPCText)
		m_pNPCText->SetEnable(false);
}

void CNPCDialog::SetPortrait(const TSTRING _sPortrait)
{
	m_pNPCPortrait->SetPortrait(_sPortrait);
}

void CNPCDialog::SetText(const TSTRING _sText)
{
	m_pNPCText->SetText(_sText);
	m_pNPCPortrait->OnAnimation(true);
}


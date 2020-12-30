#include "stdafx.h"
#include "..\Header\DialogText.h"


CDialogText::CDialogText()
	: m_pText(nullptr)
	, m_sText(TEXT(""))
	, m_sBuffer(TEXT(""))
	, m_nIndex(0)
	, m_nLength(0)
	, m_fTime(0.f)
	, m_fTypingSpeed(0.1f)
	, m_bIsEnd(false)
{
}

CDialogText::CDialogText(const CDialogText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
	, m_sText(TEXT(""))
	, m_sBuffer(TEXT(""))
	, m_nIndex(0)
	, m_nLength(0)
	, m_fTime(0.f)
	, m_fTypingSpeed(0.1f)
	, m_bIsEnd(false)
{
}

void CDialogText::Free()
{
	CGameObject::Free();
}

CDialogText * CDialogText::Create()
{
	CDialogText* pInstance = new CDialogText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeAddRef(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDialogText::Clone()
{
	CDialogText* pClone = new CDialogText(*this);
	return pClone;
}

HRESULT CDialogText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDialogText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDialogText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetHorizon(CText::HORIZON::LEFT);
	m_pText->SetSize(1.2f);
	m_pText->SetWidth(550.f);
	m_pText->SetHeight(66.f);
	return S_OK;
}

UINT CDialogText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Typing(_fDeltaTime);
	return 0;
}

UINT CDialogText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDialogText::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}

void CDialogText::SetText(const TSTRING & _sText, const D3DXVECTOR3 _vPosition)
{
	m_sText = _sText;
	m_sBuffer = TEXT("");
	m_nIndex = 0;
	m_nLength = m_sText.length();
	m_bIsEnd = false;
	m_pText->SetText(TEXT(" "));
	m_pTransform->Set_Position(_vPosition);
	m_pTransform->UpdateTransform();
}

bool CDialogText::IsTypingEnd()
{
	return m_bIsEnd;
}

void CDialogText::Typing(float _fDeltaTime)
{
	if (true == m_bIsEnd)
		return;

	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fTypingSpeed)
	{
		m_sBuffer += m_sText[m_nIndex];
		//누적 시간 초기화.
		m_fTime -= m_fTypingSpeed;
		//인덱스 증가
		++m_nIndex;
		//인덱스 예외 처리
		if (m_nLength <= m_nIndex)
			m_nIndex = 0;

		m_pText->SetText(m_sBuffer);
	}

	if (m_sBuffer == m_sText)
	{
		m_bIsEnd = true;
	}
}
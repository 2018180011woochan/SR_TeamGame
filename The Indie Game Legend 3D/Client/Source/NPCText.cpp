#include "stdafx.h"
#include "..\Header\NPCText.h"


CNPCText::CNPCText()
	: m_pText(nullptr)
	, m_sText(TEXT(""))
	, m_sBuffer(TEXT(""))
	, m_nIndex(0)
	, m_nLength(0)
	, m_fTime(0.f)
	, m_fTypingSpeed(0.2f)
	, m_bIsEnd(false)
{
}

CNPCText::CNPCText(const CNPCText & _rOther)
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

void CNPCText::Free()
{
	CGameObject::Free();
}

CNPCText * CNPCText::Create()
{
	CNPCText* pInstance = new CNPCText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CNPCText::Clone()
{
	CNPCText* pClone = new CNPCText(*this);
	return pClone;
}

HRESULT CNPCText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CNPCText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();
	
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CNPCText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetHorizon(CText::HORIZON::LEFT);
	m_pText->SetSize(1.2f);
	m_pText->SetWidth(438.f);
	m_pText->SetHeight(200.f);
	SetText(TEXT(""));
	//SetText(TEXT("This is commander bean from hq\nand i've been assgined to you\nfor this mission"));
	m_pTransform->Set_Position(D3DXVECTOR3(-0.f, 150.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CNPCText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Typing(_fDeltaTime);
	return 0;
}

UINT CNPCText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CNPCText::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}

void CNPCText::SetText(const TSTRING & _sText)
{
	m_sText = _sText;
	m_sBuffer = TEXT("");
	m_nIndex = 0;
	m_nLength = m_sText.length();
	m_bIsEnd = false;
}

bool CNPCText::IsTypingEnd()
{
	return m_bIsEnd;
}

void CNPCText::Typing(float _fDeltaTime)
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


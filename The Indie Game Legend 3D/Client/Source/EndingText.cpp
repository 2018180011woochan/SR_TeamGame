#include "stdafx.h"
#include "..\Header\EndingText.h"


CEndingText::CEndingText()
{
}

CEndingText::CEndingText(const CEndingText & _rOther)
	: CGameObject(_rOther)
	, m_sBuffer(TEXT(""))
	, m_nIndex(0)
	, m_nLength(0)
	, m_fTime(0.f)
	, m_fTypingSpeed(0.08f)
{
}

void CEndingText::Free()
{
	CGameObject::Free();
}

CEndingText * CEndingText::Create()
{
	CEndingText* pInstance = new CEndingText;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CEndingText::Clone()
{
	CEndingText* pClone = new CEndingText(*this);
	return pClone;
}

HRESULT CEndingText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CEndingText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)(AddComponent<CText>());
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CEndingText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));
	m_pText->SetText(TEXT(""));
	m_pText->SetHorizon(CText::HORIZON::CENTER);
	m_pText->SetSize(3.f);
	m_pText->SetWidth(95.f);
	m_pText->SetHeight(35.f);
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pTransform->UpdateTransform();

	m_sText.push_back(TEXT("JUSIN 111TH"));
	m_sText.push_back(TEXT("THE INDIE GAME LEGEND 3D"));
	m_sText.push_back(TEXT("Woo jong min"));
	m_sText.push_back(TEXT("Kim woo chan"));
	m_sText.push_back(TEXT("Kwon hyeon jae"));
	m_sText.push_back(TEXT("THANK YOU FOR WATCHING!"));

	m_sBuffer = TEXT("");
	m_nLineIndex = 0;
	m_nLength = m_sText[0].length();

	m_fDelayTime = 0.f;
	return S_OK;
}

UINT CEndingText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	static float fDelay = 0.f;
	fDelay += _fDeltaTime;
	if (fDelay > 0.5f)
	Typing(_fDeltaTime);
	return 0;
}

UINT CEndingText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CEndingText::Render()
{
	CGameObject::Render();
	m_pText->Render();
	return S_OK;
}

void CEndingText::Typing(const float _fDeltaTime)
{
	/*if (true == m_bIsEnd)
		return;*/
	if (m_nLineIndex >= m_sText.size())
		return;

	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fTypingSpeed)
	{
		if(m_sBuffer.length () != m_nLength)
		m_sBuffer += m_sText[m_nLineIndex][m_nIndex];
		//누적 시간 초기화.
		m_fTime -= m_fTypingSpeed;
		//인덱스 증가
		++m_nIndex;
		//인덱스 예외 처리
		if (m_nLength <= m_nIndex)
		{
			m_nIndex = m_nLength - 1;
			m_fDelayTime += _fDeltaTime;
			if (m_fDelayTime > 0.1f)
			{
				m_fDelayTime = 0.f;
				m_nIndex = 0;
				m_nLineIndex++;
				
				if (m_nLineIndex < m_sText.size())
				{
					m_sBuffer = TEXT("");
					m_nLength = m_sText[m_nLineIndex].length();
				}
			}
		}

		m_pText->SetText(m_sBuffer);
	}

	//if (m_sBuffer == m_sText)
	//{
	//	m_bIsEnd = true;
	//}
}

#include "stdafx.h"
#include "..\Header\DiscText.h"


CDiscText::CDiscText()
	: m_pText(nullptr)
	, m_nMaxCount(1)
	, m_nCount(0)
{
}

CDiscText::CDiscText(const CDiscText & _rOther)
	: CGameObject(_rOther)
	, m_pText(nullptr)
	, m_nMaxCount(1)
	, m_nCount(0)
{
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

CGameObject * CDiscText::Clone()
{
	CDiscText* pClone = new CDiscText(*this);
	return pClone;
}

HRESULT CDiscText::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDiscText::Awake()
{
	CGameObject::Awake();
	m_pText = (CText*)AddComponent<CText>();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDiscText::Start()
{
	CGameObject::Start();
	m_pText->SetFont(TEXT("Squarem-OL"));

	m_pText->SetHorizon(CText::HORIZON::RIGHT);
	m_pText->SetSize(2.5f);
	m_pText->SetWidth(95.f);
	m_pText->SetHeight(35.f);

	m_pTransform->Set_Position(D3DXVECTOR3(580.f, 255.f, 0.f));
	m_pTransform->UpdateTransform();

	SetMaxCount(m_nMaxCount);
	SetCount(m_nCount);
	return S_OK;
}

UINT CDiscText::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CDiscText::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDiscText::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pText->Render();
	return S_OK;
}


void CDiscText::SetMaxCount(const UINT _nMaxCount)
{
	m_nMaxCount = _nMaxCount;

	TCHAR szBuf[128] = TEXT("");
	_stprintf_s(szBuf, 128, TEXT("%d/%d"), m_nCount, m_nMaxCount);
	m_pText->SetText(szBuf);
}

void CDiscText::SetCount(const UINT _nCount)
{
	m_nCount = _nCount;

	TCHAR szBuf[128] = TEXT("");
	_stprintf_s(szBuf, 128, TEXT("%d/%d"), m_nCount, m_nMaxCount);
	m_pText->SetText(szBuf);
}


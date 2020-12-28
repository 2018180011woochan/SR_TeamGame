#include "stdafx.h"
#include "..\Header\BossHP.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include "BossText.h"

CBossHP::CBossHP()
	: m_pBossHpBar(nullptr)
	, m_pBossHpFrame(nullptr)
	, m_pBossText(nullptr)
{
}

CBossHP::CBossHP(const CBossHP & _rOther)
	: CGameObject(_rOther)
	, m_pBossHpBar(nullptr)
	, m_pBossHpFrame(nullptr)
	, m_pBossText(nullptr)
{
}

void CBossHP::Free()
{
	CGameObject::Free();
	SafeRelease(m_pBossHpBar);
	SafeRelease(m_pBossHpFrame);
	SafeRelease(m_pBossText);
}

CBossHP * CBossHP::Create()
{
	CBossHP* pInstance = new CBossHP;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBossHP::Clone()
{
	CBossHP* pClone = new CBossHP(*this);
	return pClone;
}

HRESULT CBossHP::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBossHP::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CBossHP::Start()
{
	CGameObject::Start();

	m_pBossHpBar = (CBossHPBar*)FindGameObjectOfType<CBossHPBar>();
	m_pBossHpFrame = (CBossHPFrame*)FindGameObjectOfType<CBossHPFrame>();
	m_pBossText = (CBossText*)FindGameObjectOfType<CBossText>();

	SafeAddRef(m_pBossHpBar);
	SafeAddRef(m_pBossHpFrame);
	SafeAddRef(m_pBossText);
	return S_OK;
}

UINT CBossHP::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CBossHP::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CBossHP::Render()
{
	return S_OK;
}

void CBossHP::OnEnable()
{
	if (nullptr != m_pBossHpBar)
		m_pBossHpBar->SetEnable(true);
	if (nullptr != m_pBossHpFrame)
		m_pBossHpFrame->SetEnable(true);
	if (nullptr != m_pBossText)
		m_pBossText->SetEnable(true);
}

void CBossHP::OnDisable()
{
	if (nullptr != m_pBossHpBar)
		m_pBossHpBar->SetEnable(false);
	if (nullptr != m_pBossHpFrame)
		m_pBossHpFrame->SetEnable(false);
	if (nullptr != m_pBossText)
		m_pBossText->SetEnable(false);
}

void CBossHP::SetHPBar(float _fRatio)
{
	m_pBossHpBar->SetHPBar(_fRatio);
}


#include "stdafx.h"
#include "..\Header\AmmoHUD.h"
#include "AmmoFrame.h"
#include "AmmoGauge.h"
#include "AmmoIcon.h"
#include "SkillGauge.h"
#include "AmmoIconHighlight.h"

CAmmoHUD::CAmmoHUD()
	: m_pAmmoFrame(nullptr)
	, m_pAmmoGauge(nullptr)
	, m_pAmmoIcon(nullptr)
	, m_pSkillGauge(nullptr)
	, m_pAmmoIconHighlight(nullptr)
{
}

CAmmoHUD::CAmmoHUD(const CAmmoHUD & _rOther)
	: CGameObject(_rOther)
	, m_pAmmoFrame(nullptr)
	, m_pAmmoGauge(nullptr)
	, m_pAmmoIcon(nullptr)
	, m_pSkillGauge(nullptr)
	, m_pAmmoIconHighlight(nullptr)
{
}

void CAmmoHUD::Free()
{
	CGameObject::Free();
	SafeRelease(m_pAmmoFrame);
	SafeRelease(m_pAmmoGauge);
	SafeRelease(m_pAmmoIcon);
	SafeRelease(m_pSkillGauge);
	SafeRelease(m_pAmmoIconHighlight);
}

CAmmoHUD * CAmmoHUD::Create()
{
	CAmmoHUD* pInstance = new CAmmoHUD;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CAmmoHUD::Clone()
{
	CAmmoHUD* pClone = new CAmmoHUD(*this);
	return pClone;
}

HRESULT CAmmoHUD::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CAmmoHUD::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CAmmoHUD::Start()
{
	CGameObject::Start();
	m_pAmmoFrame = (CAmmoFrame*)FindGameObjectOfType<CAmmoFrame>();
	m_pAmmoGauge = (CAmmoGauge*)FindGameObjectOfType<CAmmoGauge>();
	m_pAmmoIcon = (CAmmoIcon*)FindGameObjectOfType<CAmmoIcon>();
	m_pSkillGauge = (CSkillGauge*)FindGameObjectOfType<CSkillGauge>();
	m_pSkillGauge->SetSkillGauge(0.f);
	m_pAmmoIconHighlight = (CAmmoIconHighlight*)FindGameObjectOfType<CAmmoIconHighlight>();
	m_pAmmoIconHighlight->SetEnable(false);
	SafeAddRef(m_pAmmoFrame);
	SafeAddRef(m_pAmmoGauge);
	SafeAddRef(m_pAmmoIcon);
	SafeAddRef(m_pSkillGauge);
	SafeAddRef(m_pAmmoIconHighlight);

	return S_OK;
}

UINT CAmmoHUD::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CAmmoHUD::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CAmmoHUD::Render()
{
	return S_OK;
}

void CAmmoHUD::SetAmmoLevel(const UINT _nLevel)
{
	UINT nLevel = _nLevel;
	if (nLevel >= 4)
		nLevel = 3;

	m_pAmmoGauge->SetAmmoLevel(nLevel);

	nLevel = nLevel / 2;

	m_pAmmoFrame->SetAmmoLevel(nLevel);
}

void CAmmoHUD::SetAmmoCount(const float _fCount, const float _fMaxCount)
{
	float fFillAmount = _fCount / _fMaxCount;

	if (0.f > fFillAmount)
		fFillAmount = 0.f;
	if (1.f < fFillAmount)
		fFillAmount = 1.f;

	m_pAmmoGauge->SetAmmoCount(fFillAmount);
}

void CAmmoHUD::SetAmmoIcon(const UINT _nWeaponID)
{
	m_pAmmoIcon->SetAmmoIcon(_nWeaponID);
}

void CAmmoHUD::SetSkillGauge(const float _fValue)
{
	m_pSkillGauge->SetSkillGauge(_fValue);
}

void CAmmoHUD::SetActive(const bool _bActive)
{
	if (true == _bActive)
	{
		m_pAmmoIconHighlight->SetEnable(true);
		m_pAmmoIcon->SetActive(true);
		m_pAmmoGauge->SetActive(true);
	}
	else
	{
		m_pAmmoIconHighlight->SetEnable(false);
		m_pAmmoIcon->SetActive(false);
		m_pAmmoGauge->SetActive(false);
	}
}
